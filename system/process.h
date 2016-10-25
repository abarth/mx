// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "lib/mx/system/handle.h"

namespace mx {

class process : public handle<process> {
 public:
  process() = default;

  explicit process(handle<void> h) : handle(h.release()) {}

  process(process&& other) : handle(other.release()) {}

  process& operator=(process&& other) {
    reset(other.release());
    return *this;
  }

  static mx_status_t create(process* result,
                            const char* name,
                            uint32_t name_len,
                            uint32_t flags);
};

}  // namespace mx
