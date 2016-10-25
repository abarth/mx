// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <magenta/syscalls.h>
#include <magenta/types.h>

#include "lib/mx/system/handle_traits.h"

namespace mx {

template <typename T = void>
class handle {
 public:
  handle() : value_(MX_HANDLE_INVALID) {}

  explicit handle(mx_handle_t value) : value_(value) {}

  template <typename U>
  handle(handle<U>&& other) : value_(other.release()) {
    static_assert(is_same<T, void>::value, "Receiver must be compatible.");
  }

  ~handle() { close(); }

  template <typename U>
  handle<T>& operator=(handle<U>&& other) {
    static_assert(is_same<T, void>::value, "Receiver must be compatible.");
    reset(other.release());
    return *this;
  }

  void reset(mx_handle_t value = MX_HANDLE_INVALID) {
    close();
    value_ = value;
  }

  void swap(handle<T>& other) {
    mx_handle_t tmp = value_;
    value_ = other.value_;
    other.value_ = tmp;
  }

  handle<T> duplicate(mx_rights_t rights) {
    static_assert(handle_traits<T>::is_duplicatable,
                  "Receiver must be duplicatable.");
    return handle<T>(mx_handle_duplicate(value_), rights);
  }

  explicit operator bool() { return value_ != MX_HANDLE_INVALID; }

  mx_handle_t get() const { return value_; }

  __attribute__((warn_unused_result)) mx_handle_t release() {
    mx_handle_t result = value_;
    value_ = MX_HANDLE_INVALID;
    return result;
  }

 private:
  template <typename A, typename B>
  struct is_same {
    static const bool value = false;
  };

  template <typename A>
  struct is_same<A, A> {
    static const bool value = true;
  };

  handle(const handle<T>&) = delete;

  void operator=(const handle<T>&) = delete;

  void close() {
    if (value_ != MX_HANDLE_INVALID) {
      mx_handle_close(value_);
      value_ = MX_HANDLE_INVALID;
    }
  }

  mx_handle_t value_;
};

}  // namespace mx
