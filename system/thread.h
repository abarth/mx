// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "lib/mx/system/handle.h"
#include "lib/mx/system/process.h"

namespace mx {

class thread : public handle<thread> {
 public:
  thread() = default;

  explicit thread(handle<void> h) : handle(h.release()) {}

  thread(thread&& other) : handle(other.release()) {}

  thread& operator=(thread&& other) {
    reset(other.release());
    return *this;
  }
};

}  // namespace mx
