// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/mx/system/thread.h"

#include <magenta/syscalls.h>

#include "lib/mx/system/process.h"

namespace mx {

mx_status_t thread::create(thread* result,
                           const process& process,
                           const char* name,
                           uint32_t name_len,
                           uint32_t flags) {
  mx_handle_t h = mx_thread_create(process.get(), name, name_len, flags);
  result->reset((h < 0) ? MX_HANDLE_INVALID : h);
  return h;
}

}  // namespace mx
