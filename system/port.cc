// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/mx/system/port.h"

#include <magenta/syscalls.h>

namespace mx {

mx_status_t port::create(port* result, uint32_t options) {
  mx_handle_t h = mx_port_create(options);
  result->reset(h < 0 ? MX_HANDLE_INVALID : h);
  return h;
}

}  // namespace mx
