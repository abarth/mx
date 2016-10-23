// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "mx/system/handle.h"

namespace mx {

Handle Handle::Duplicate(mx_rights_t rights) {
  return Handle(mx_handle_duplicate(value_, rights));
}

void Handle::CloseIfNecessary() {
  if (value_ != MX_HANDLE_INVALID) {
    mx_handle_close(value_);
    value_ = MX_HANDLE_INVALID;
  }
}

}  // namespace mx
