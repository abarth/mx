// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_MDL_CPP_BINDINGS_LIB_SHARED_PTR_H_
#define LIB_MDL_CPP_BINDINGS_LIB_SHARED_PTR_H_

#include "lib/mdl/cpp/bindings/lib/shared_data.h"

namespace mdl {
namespace internal {

// Used to manage a heap-allocated instance of P that can be shared via
// reference counting. When the last reference is dropped, the instance is
// deleted.
template <typename P>
class SharedPtr {
 public:
  SharedPtr() {}

  explicit SharedPtr(P* ptr) { impl_.mutable_value()->ptr = ptr; }

  // Default copy-constructor and assignment operator are OK.

  P* get() { return impl_.value().ptr; }
  const P* get() const { return impl_.value().ptr; }

  void reset() { impl_.reset(); }

  P* operator->() { return get(); }
  const P* operator->() const { return get(); }

 private:
  class Impl {
   public:
    ~Impl() {
      if (ptr)
        delete ptr;
    }

    Impl() : ptr(nullptr) {}

    Impl(P* ptr) : ptr(ptr) {}

    P* ptr;

   private:
    FTL_DISALLOW_COPY_AND_ASSIGN(Impl);
  };

  SharedData<Impl> impl_;
};

}  // namespace mdl
}  // namespace internal

#endif  // LIB_MDL_CPP_BINDINGS_LIB_SHARED_PTR_H_
