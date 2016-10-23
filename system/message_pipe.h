// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <utility>

#include "mx/system/handle.h"

namespace mx {

class MessagePipeEndpoint : public internal::HandleHolder<MessagePipeEndpoint> {
 public:
  MessagePipeEndpoint() = default;

  explicit MessagePipeEndpoint(Handle handle)
      : internal::HandleHolder(std::move(handle)) {}

  MessagePipeEndpoint(MessagePipeEndpoint&& other)
      : internal::HandleHolder(other) {}
};

class MessagePipe {
 public:
  explicit MessagePipe(uint32_t flags);

  MessagePipeEndpoint endpoint0;
  MessagePipeEndpoint endpoint1;
};

}  // namespace mx
