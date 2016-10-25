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

  handle<T> duplicate(mx_rights_t rights) const {
    static_assert(handle_traits<T>::is_duplicatable,
                  "Receiver must be duplicatable.");
    return handle<T>(mx_handle_duplicate(value_), rights);
  }

  mx_status_t wait(mx_signals_t signals,
                   mx_time_t timeout,
                   mx_signals_state_t* signals_state) const {
    return mx_wait_one(value_, signals, timeout, signals_state);
  }

  mx_status_t replace(handle<T>* result, mx_rights_t rights) const {
    mx_handle_t h = mx_handle_replace(value_, rights);
    result->reset(h < 0 ? MX_HANDLE_INVALID : h);
    return h;
  }

  mx_status_t signal(uint32_t clear_mask, uint32_t set_mask) const {
    return mx_object_signal(get(), clear_mask, set_mask);
  }

  mx_ssize_t get_info(uint32_t topic,
                      uint16_t topic_size,
                      void* buffer,
                      mx_size_t buffer_size) const {
    return mx_object_get_info(get(), topic, topic_size, buffer, buffer_size);
  }

  // TODO(abarth): mx_object_get_child
  // TODO(abarth): mx_object_bind_exception_port

  mx_status_t get_property(uint32_t property, void* value, mx_size_t size) {
    return get_property(get(), property, value, size);
  }

  mx_status_t set_property(uint32_t property,
                           const void* value,
                           mx_size_t size) {
    return set_property(get(), property, value, size);
  }

  explicit operator bool() const { return value_ != MX_HANDLE_INVALID; }

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
