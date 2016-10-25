// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "lib/mx/system/handle.h"

namespace mx {

class port : public handle<port> {
 public:
  port() = default;

  explicit port(handle<void> h) : handle(h.release()) {}

  port(port&& other) : handle(other.release()) {}

  port& operator=(port&& other) {
    reset(other.release());
    return *this;
  }

  static mx_status_t create(port* result, msgpipe* endpoint1, uint32_t flags);

  mx_status_t read(void* bytes,
                   uint32_t* num_bytes,
                   mx_handle_t* handles,
                   uint32_t* num_handles,
                   uint32_t flags) const {
    return mx_msgpipe_read(get(), bytes, num_bytes, handles, num_handles,
                           flags);
  }

  mx_status_t write(const void* bytes,
                    uint32_t num_bytes,
                    const mx_handle_t* handles,
                    uint32_t num_handles,
                    uint32_t flags) const {
    return mx_msgpipe_write(get(), bytes, num_bytes, handles, num_handles,
                            flags);
  }
};

}  // namespace mx
