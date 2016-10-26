// Copyright 2015 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_FIDL_CPP_BINDINGS_INTERFACE_PTR_INFO_H_
#define LIB_FIDL_CPP_BINDINGS_INTERFACE_PTR_INFO_H_

#include <mx/msgpipe.h>

#include <cstddef>
#include <utility>

#include "lib/fidl/cpp/bindings/macros.h"
#include "lib/ftl/macros.h"

namespace fidl {

template <typename Interface>
class InterfacePtr;

template <typename Interface>
class SynchronousInterfacePtr;

// InterfaceHandle stores necessary information to communicate with a remote
// interface implementation, which could be used to construct an InterfacePtr.
template <typename Interface>
class InterfaceHandle {
 public:
  InterfaceHandle() : version_(0u) {}
  InterfaceHandle(std::nullptr_t) : version_(0u) {}

  InterfaceHandle(mx::msgpipe handle, uint32_t version)
      : handle_(std::move(handle)), version_(version) {}

  InterfaceHandle(InterfaceHandle&& other)
      : handle_(std::move(other.handle_)), version_(other.version_) {
    other.version_ = 0u;
  }

  // Making this constructor templated ensures that it is not type-instantiated
  // unless it is used, making the InterfacePtr<->InterfaceHandle codependency
  // less fragile.
  template <typename SameInterfaceAsAbove = Interface>
  InterfaceHandle(InterfacePtr<SameInterfaceAsAbove>&& ptr) {
    *this = ptr.PassInterfaceHandle();
  }

  // Making this constructor templated ensures that it is not type-instantiated
  // unless it is used, making the SynchronousInterfacePtr<->InterfaceHandle
  // codependency less fragile.
  template <typename SameInterfaceAsAbove = Interface>
  InterfaceHandle(SynchronousInterfacePtr<SameInterfaceAsAbove>&& ptr) {
    *this = ptr.PassInterfaceHandle();
  }

  ~InterfaceHandle() {}

  InterfaceHandle& operator=(InterfaceHandle&& other) {
    if (this != &other) {
      handle_ = std::move(other.handle_);
      version_ = other.version_;
      other.version_ = 0u;
    }

    return *this;
  }

  // Tests as true if we have a valid handle.
  explicit operator bool() const { return is_valid(); }
  bool is_valid() const { return handle_; }

  mx::msgpipe PassHandle() { return std::move(handle_); }
  const mx::msgpipe& handle() const { return handle_; }
  void set_handle(mx::msgpipe handle) { handle_ = std::move(handle); }

  uint32_t version() const { return version_; }
  void set_version(uint32_t version) { version_ = version; }

 private:
  mx::msgpipe handle_;
  uint32_t version_;

  FIDL_MOVE_ONLY_TYPE(InterfaceHandle);
};

}  // namespace fidl

#endif  // LIB_FIDL_CPP_BINDINGS_INTERFACE_PTR_INFO_H_
