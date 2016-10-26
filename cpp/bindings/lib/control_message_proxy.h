// Copyright 2015 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_FIDL_CPP_BINDINGS_LIB_CONTROL_MESSAGE_PROXY_H_
#define LIB_FIDL_CPP_BINDINGS_LIB_CONTROL_MESSAGE_PROXY_H_

#include <stdint.h>

#include <functional>

#include "lib/ftl/macros.h"

namespace fidl {

class MessageReceiverWithResponder;

namespace internal {

// Proxy for request messages defined in interface_control_messages.mojom.
class ControlMessageProxy {
 public:
  // Doesn't take ownership of |receiver|. It must outlive this object.
  explicit ControlMessageProxy(MessageReceiverWithResponder* receiver);

  void QueryVersion(const std::function<void(uint32_t)>& callback);
  void RequireVersion(uint32_t version);

 protected:
  // Not owned.
  MessageReceiverWithResponder* receiver_;

  FTL_DISALLOW_COPY_AND_ASSIGN(ControlMessageProxy);
};

}  // namespace internal
}  // namespace fidl

#endif  // LIB_FIDL_CPP_BINDINGS_LIB_CONTROL_MESSAGE_PROXY_H_
