// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/mx/system/waitset.h"

#include <magenta/syscalls.h>

namespace mx {

mx_status_t waitset::create(waitset* result) {
  mx_handle_t h = mx_waitset_create();
  result->reset(h < 0 ? MX_HANDLE_INVALID : h);
  return h;
}

}  // namespace mx
