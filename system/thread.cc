// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mx/system/thread.h"

#include <magenta/syscalls.h>

namespace mx {

Thread::Thread(const Process& process,
               const char* name,
               uint32_t name_len,
               uint32_t flags)
    : handle_(mx_thread_create(process.handle().get(), name, name_len, flags));

}  // namespace mx
