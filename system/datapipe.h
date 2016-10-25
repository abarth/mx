// Copyright 2016 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include "lib/mx/system/handle.h"

namespace mx {
class datapipe_producer;
class datapipe_consumer;

template <typename T = void>
class datapipe : public handle<T> {
 public:
  datapipe() = default;

  explicit datapipe(mx_handle_t h) : handle<T>(h) {}

  explicit datapipe(handle<void> h) : handle<T>(h.release()) {}

  datapipe(datapipe&& other) : handle<T>(other.release()) {}

  static mx_status_t create(datapipe_producer* producer,
                            datapipe_consumer* consumer,
                            uint32_t options,
                            mx_size_t element_size,
                            mx_size_t capacity) {
    mx_handle_t consumer_handle = MX_HANDLE_INVALID;
    mx_handle_t h = mx_datapipe_create(options, mx_size_t element_size,
                                       capacity, &consumer_handle);
    producer->reset(h < 0 ? MX_HANDLE_INVALID : h);
    consumer->reset(consumer_handle);
    return h;
  }
};

class datapipe_producer : public datapipe<datapipe_producer> {
 public:
  datapipe_producer() = default;

  explicit datapipe_producer(handle<void> h)
      : datapipe<datapipe_producer>(h.release()) {}

  datapipe_producer(datapipe_producer&& other)
      : datapipe<datapipe_producer>(other.release()) {}

  datapipe_producer& operator=(datapipe_producer&& other) {
    reset(other.release());
    return *this;
  }

  mx_ssize_t write(uint32_t flags, mx_size_t requested, const void* buffer) {
    return mx_datapipe_write(get(), flags, requested, buffer);
  }

  mx_ssize_t begin_write(uint32_t flags, uintptr_t* buffer) {
    return mx_datapipe_begin_write(get(), flags, buffer);
  }

  mx_status_t end_write(mx_size_t written) {
    return mx_datapipe_end_write(get(), written);
  }
};

class datapipe_consumer : public datapipe<datapipe_consumer> {
 public:
  datapipe_consumer() = default;

  explicit datapipe_consumer(handle<void> h)
      : datapipe<datapipe_consumer>(h.release()) {}

  datapipe_consumer(datapipe_consumer&& other)
      : datapipe<datapipe_consumer>(other.release()) {}

  datapipe_consumer& operator=(datapipe_consumer&& other) {
    reset(other.release());
    return *this;
  }

  mx_ssize_t read(uint32_t flags, mx_size_t requested, void* buffer) {
    return mx_datapipe_read(get(), flags, requested, buffer);
  }

  mx_ssize_t begin_read(uint32_t flags, uintptr_t* buffer) {
    return mx_datapipe_begin_read(get(), flags, buffer);
  }

  mx_status_t end_read(mx_size_t read) {
    return mx_datapipe_end_read(get(), read);
  }
};

}  // namespace mx
