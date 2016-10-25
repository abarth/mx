// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/mx/system/process.h"

#include <magenta/syscalls.h>

#include "lib/mx/system/thread.h"

namespace mx {

mx_status_t process::create(process* result,
                            const char* name,
                            uint32_t name_len,
                            uint32_t flags) {
  mx_handle_t h = mx_process_create(name, name_len, flags);
  result->reset((h < 0) ? MX_HANDLE_INVALID : h);
  return h;
}

mx_status_t process::start(const thread& thread_handle,
                           uintptr_t entry,
                           uintptr_t stack,
                           handle arg_handle,
                           uintptr_t arg2) const {
  mx_handle_t arg_h = arg_handle.release();
  mx_status_t result =
      mx_process_start(get(), thread_handle.get(), entry, stack, arg_h, arg2);
  if (result < 0)
    mx_handle_close(arg_h);
  return result;
}

}  // namespace mx
