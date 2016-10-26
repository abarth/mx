// Copyright 2014 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_FIDL_CPP_BINDINGS_LIB_ROUTER_H_
#define LIB_FIDL_CPP_BINDINGS_LIB_ROUTER_H_

#include <map>

#include "lib/fidl/cpp/bindings/lib/connector.h"
#include "lib/fidl/cpp/bindings/lib/shared_data.h"
#include "lib/fidl/cpp/bindings/lib/validation_errors.h"
#include "lib/fidl/cpp/bindings/message_validator.h"
#include "lib/fidl/cpp/waiter/default.h"

namespace fidl {
namespace internal {

// Router provides a way for sending messages over a MessagePipe, and re-routing
// response messages back to the sender.
class Router : public MessageReceiverWithResponder {
 public:
  Router(mx::msgpipe message_pipe,
         MessageValidatorList validators,
         const MojoAsyncWaiter* waiter = GetDefaultAsyncWaiter());
  ~Router() override;

  // Sets the receiver to handle messages read from the message pipe that do
  // not have the kMessageIsResponse flag set.
  void set_incoming_receiver(MessageReceiverWithResponderStatus* receiver) {
    incoming_receiver_ = receiver;
  }

  // Sets the error handler to receive notifications when an error is
  // encountered while reading from the pipe or waiting to read from the pipe.
  void set_connection_error_handler(const ftl::Closure& error_handler) {
    connector_.set_connection_error_handler(error_handler);
  }

  // Returns true if an error was encountered while reading from the pipe or
  // waiting to read from the pipe.
  bool encountered_error() const { return connector_.encountered_error(); }

  // Is the router bound to a MessagePipe handle?
  bool is_valid() const { return connector_.is_valid(); }

  void CloseMessagePipe() { connector_.CloseMessagePipe(); }

  mx::msgpipe PassMessagePipe() { return connector_.PassMessagePipe(); }

  // MessageReceiver implementation:
  bool Accept(Message* message) override;
  bool AcceptWithResponder(Message* message,
                           MessageReceiver* responder) override;

  // Blocks the current thread until the first incoming method call, i.e.,
  // either a call to a client method or a callback method, or |deadline|.
  // When returning |false| closes the message pipe, unless the reason for
  // for returning |false| was |MOJO_SYSTEM_RESULT_SHOULD_WAIT| or
  // |MOJO_SYSTEM_RESULT_DEADLINE_EXCEEDED|.
  // Use |encountered_error| to see if an error occurred.
  bool WaitForIncomingMessage(mx_time_t deadline) {
    return connector_.WaitForIncomingMessage(deadline);
  }

  // Sets this object to testing mode.
  // In testing mode:
  // - the object is more tolerant of unrecognized response messages;
  // - the connector continues working after seeing errors from its incoming
  //   receiver.
  void EnableTestingMode();

  MessagePipeHandle handle() const { return connector_.handle(); }

 private:
  typedef std::map<uint64_t, MessageReceiver*> ResponderMap;

  // This class is registered for incoming messages from the |Connector|.  It
  // simply forwards them to |Router::HandleIncomingMessages|.
  class HandleIncomingMessageThunk : public MessageReceiver {
   public:
    explicit HandleIncomingMessageThunk(Router* router);
    ~HandleIncomingMessageThunk() override;

    // MessageReceiver implementation:
    bool Accept(Message* message) override;

   private:
    Router* router_;
  };

  bool HandleIncomingMessage(Message* message);

  HandleIncomingMessageThunk thunk_;
  MessageValidatorList validators_;
  Connector connector_;
  SharedData<Router*> weak_self_;
  MessageReceiverWithResponderStatus* incoming_receiver_;
  ResponderMap responders_;
  uint64_t next_request_id_;
  bool testing_mode_;
};

}  // namespace internal
}  // namespace fidl

#endif  // LIB_FIDL_CPP_BINDINGS_LIB_ROUTER_H_
