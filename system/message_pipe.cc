// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mx/system/message_pipe.h"

#include <magenta/syscalls.h>

namespace mx {

MessagePipe::MessagePipe(uint32_t flags) {
  mx_handle_t endpoints[2] = {
      MX_HANDLE_INVALID, MX_HANDLE_INVALID,
  };
  mx_status_t status = mx_msgpipe_create(endpoints, flags);
  if (status != NO_ERROR)
    return;
  endpoint0 = MessagePipeEndpoint(Handle(endpoint[0]));
  endpoint1 = MessagePipeEndpoint(Handle(endpoint[0]));
}

}  // namespace mx
