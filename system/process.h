// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "lib/mx/system/handle.h"
#include "lib/mx/system/task.h"
#include "lib/mx/system/vmo.h"

namespace mx {
class thread;

class process : public task<process> {
 public:
  process() = default;

  explicit process(handle<void> h) : task<process>(h.release()) {}

  process(process&& other) : task<process>(other.release()) {}

  process& operator=(process&& other) {
    reset(other.release());
    return *this;
  }

  static mx_status_t create(process* result,
                            const char* name,
                            uint32_t name_len,
                            uint32_t flags);

  mx_status_t start(const thread& thread_handle,
                    uintptr_t entry,
                    uintptr_t stack,
                    handle arg_handle,
                    uintptr_t arg2) const;

  mx_status_t map_vm(const vmo& vmo_handle,
                     uint64_t offset,
                     mx_size_t len,
                     uintptr_t* ptr,
                     uint32_t flags) const {
    return mx_process_map_vm(get(), vmo_handle.get(), offset, len, ptr, flags);
  }

  mx_status_t unmap_vm(uintptr_t address, mx_size_t len) const {
    return mx_process_unmap_vm(get(), address, len);
  }

  mx_status_t protect_vm(uintptr_t address,
                         mx_size_t len,
                         uint32_t prot) const {
    return mx_process_protect_vm(get(), address, len, prot);
  }
};

}  // namespace mx
