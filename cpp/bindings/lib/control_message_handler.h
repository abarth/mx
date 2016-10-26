// Copyright 2015 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_FIDL_CPP_BINDINGS_LIB_CONTROL_MESSAGE_HANDLER_H_
#define LIB_FIDL_CPP_BINDINGS_LIB_CONTROL_MESSAGE_HANDLER_H_

#include <stdint.h>

#include "lib/fidl/cpp/bindings/message.h"
#include "lib/ftl/macros.h"

namespace fidl {
namespace internal {

// Handlers for request messages defined in interface_control_messages.mojom.
class ControlMessageHandler : public MessageReceiverWithResponderStatus {
 public:
  static bool IsControlMessage(const Message* message);

  explicit ControlMessageHandler(uint32_t interface_version);
  ~ControlMessageHandler() override;

  // Call the following methods only if IsControlMessage() returned true.
  bool Accept(Message* message) override;
  // Takes ownership of |responder|.
  bool AcceptWithResponder(Message* message,
                           MessageReceiverWithStatus* responder) override;

 private:
  bool Run(Message* message, MessageReceiverWithStatus* responder);
  bool RunOrClosePipe(Message* message);

  uint32_t interface_version_;

  FTL_DISALLOW_COPY_AND_ASSIGN(ControlMessageHandler);
};

}  // namespace internal
}  // namespace fidl

#endif  // LIB_FIDL_CPP_BINDINGS_LIB_CONTROL_MESSAGE_HANDLER_H_
