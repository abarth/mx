// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <utility>

#include "mx/system/handle.h"
#include "mx/system/process.h"

namespace mx {

class Thread : public internal::HandleHolder<Thread> {
 public:
  Thread() = default;

  explicit Thread(Handle handle) : internal::HandleHolder(std::move(handle)) {}

  Thread(Thread&& other) : internal::HandleHolder(other) {}

  Thread(const Process& process,
         const char* name,
         uint32_t name_len,
         uint32_t flags);
};

}  // namespace mx
