// Copyright 2013 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_FIDL_CPP_BINDINGS_LIB_CONNECTOR_H_
#define LIB_FIDL_CPP_BINDINGS_LIB_CONNECTOR_H_

#include <mx/msgpipe.h>

#include "lib/fidl/cpp/bindings/message.h"
#include "lib/fidl/cpp/waiter/default.h"
#include "lib/fidl/cpp/waiter/default.h"
#include "lib/ftl/compiler_specific.h"
#include "lib/ftl/macros.h"

namespace fidl {
namespace internal {

// The Connector class is responsible for performing read/write operations on a
// MessagePipe. It writes messages it receives through the MessageReceiver
// interface that it subclasses, and it forwards messages it reads through the
// MessageReceiver interface assigned as its incoming receiver.
//
// NOTE: MessagePipe I/O is non-blocking.
//
class Connector : public MessageReceiver {
 public:
  // The Connector takes ownership of |message_pipe|.
  explicit Connector(mx::msgpipe message_pipe,
                     const FidlAsyncWaiter* waiter = GetDefaultAsyncWaiter());
  ~Connector() override;

  // Sets the receiver to handle messages read from the message pipe.  The
  // Connector will read messages from the pipe regardless of whether or not an
  // incoming receiver has been set.
  void set_incoming_receiver(MessageReceiver* receiver) {
    incoming_receiver_ = receiver;
  }

  // Errors from incoming receivers will force the connector into an error
  // state, where no more messages will be processed. This method is used
  // during testing to prevent that from happening.
  void set_enforce_errors_from_incoming_receiver(bool enforce) {
    enforce_errors_from_incoming_receiver_ = enforce;
  }

  // Sets the error handler to receive notifications when an error is
  // encountered while reading from the pipe or waiting to read from the pipe.
  void set_connection_error_handler(const ftl::Closure& error_handler) {
    connection_error_handler_ = error_handler;
  }

  // Returns true if an error was encountered while reading from the pipe or
  // waiting to read from the pipe.
  bool encountered_error() const { return error_; }

  // Closes the pipe, triggering the error state. Connector is put into a
  // quiescent state.
  void CloseMessagePipe();

  // Releases the pipe, not triggering the error state. Connector is put into
  // a quiescent state.
  mx::msgpipe PassMessagePipe();

  // Is the connector bound to a MessagePipe handle?
  bool is_valid() const { return message_pipe_; }

  // Waits for the next message on the pipe, blocking until one arrives,
  // |deadline| elapses, or an error happens. Returns |true| if a message has
  // been delivered, |false| otherwise.
  // When returning |false| closes the message pipe, unless the reason for
  // for returning |false| was |MOJO_SYSTEM_RESULT_SHOULD_WAIT| or
  // |MOJO_SYSTEM_RESULT_DEADLINE_EXCEEDED|.
  // Use |encountered_error| to see if an error occurred.
  bool WaitForIncomingMessage(mx_time_t deadline);

  // MessageReceiver implementation:
  bool Accept(Message* message) override;

  mx_handle_t handle() const { return message_pipe_.get(); }

 private:
  static void CallOnHandleReady(void* ftl::Closure, mx_status_t result);
  void OnHandleReady(mx_status_t result);

  void WaitToReadMore();

  // Returns false if |this| was destroyed during message dispatch.
  FTL_WARN_UNUSED_RESULT bool ReadSingleMessage(mx_status_t* read_result);

  // |this| can be destroyed during message dispatch.
  void ReadAllAvailableMessages();

  void NotifyError();

  // Cancels any calls made to |waiter_|.
  void CancelWait();

  ftl::Closure connection_error_handler_;
  const FidlAsyncWaiter* waiter_;

  mx::msgpipe message_pipe_;
  MessageReceiver* incoming_receiver_;

  MojoAsyncWaitID async_wait_id_;
  bool error_;
  bool drop_writes_;
  bool enforce_errors_from_incoming_receiver_;

  // If non-null, this will be set to true when the Connector is destroyed.  We
  // use this flag to allow for the Connector to be destroyed as a side-effect
  // of dispatching an incoming message.
  bool* destroyed_flag_;

  FTL_DISALLOW_COPY_AND_ASSIGN(Connector);
};

}  // namespace internal
}  // namespace fidl

#endif  // LIB_FIDL_CPP_BINDINGS_LIB_CONNECTOR_H_
