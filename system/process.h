// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <utility>

#include "mx/system/handle.h"

namespace mx {

class Process : public internal::HandleHolder<Process> {
 public:
  Process() = default;

  explicit Process(Handle handle) : internal::HandleHolder(std::move(handle)) {}

  Process(Process&& other) : internal::HandleHolder(other) {}

  Process(const char* name, uint32_t name_len, uint32_t flags);
};

}  // namespace mx
