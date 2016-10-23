// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <magenta/syscalls.h>
#include <magenta/types.h>

#include <utility>

namespace mx {

class Handle {
 public:
  Handle() : value_(MX_HANDLE_INVALID) {}

  explicit Handle(mx_handle_t value) : value_(value) {}

  Handle(Handle&& other) : value_(other.release()) {}

  ~Handle() { CloseIfNecessary(); }

  Handle& operator=(Handle&& other) {
    reset(other.release());
    return *this;
  }

  void reset(mx_handle_t value = MX_HANDLE_INVALID) {
    CloseIfNecessary();
    value_ = value;
  }

  void swap(Handle& other) {
    using std::swap;
    swap(value_, other.value_);
  }

  explicit operator bool() { return value_ != MX_HANDLE_INVALID; }

  mx_handle_t get() const { return value_; }

  __attribute__((warn_unused_result)) mx_handle_t release() {
    mx_handle_t result = value_;
    value_ = MX_HANDLE_INVALID;
    return result;
  }

  Handle Duplicate(mx_rights_t rights);

 private:
  Handle(const Handle&) = delete;

  void operator=(const Handle&) = delete;

  void CloseIfNecessary() {
    if (value_ != MX_HANDLE_INVALID) {
      mx_handle_close(value_);
      value_ = MX_HANDLE_INVALID;
    }
  }

  mx_handle_t value_;
};

namespace internal {

template <typename T>
class HandleHolder {
  HandleHolder() = default;

  explicit HandleHolder(Handle handle) : handle_(std::move(handle)) {}

  HandleHolder(T&& other) : handle_(std::move(other.handle_)) {}

  T& operator=(T&& other) {
    handle_(std::move(other.handle_));
    return *this;
  }

  void swap(T& other) {
    using std::swap;
    swap(handle_, other.handle_);
  }

  explicit operator bool() { return handle_; }

  const Handle& handle() const { return handle_; }

  Handle ReleaseHandle() { return std::move(handle_); }

 private:
  HandleHolder(const HandleHolder&) = delete;

  void operator=(const HandleHolder&) = delete;

  Handle handle_;
}

}  // namespace internal
}  // namespace mx
