// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_MDL_CPP_BINDINGS_TESTS_CONTAINER_TEST_UTIL_H_
#define LIB_MDL_CPP_BINDINGS_TESTS_CONTAINER_TEST_UTIL_H_

#include "mojo/public/cpp/system/macros.h"

namespace mdl {

class CopyableType {
 public:
  CopyableType();
  CopyableType(const CopyableType& other);
  CopyableType& operator=(const CopyableType& other);
  ~CopyableType();

  bool copied() const { return copied_; }
  static size_t num_instances() { return num_instances_; }
  CopyableType* ptr() const { return ptr_; }
  void ResetCopied() { copied_ = false; }

 private:
  bool copied_;
  static size_t num_instances_;
  CopyableType* ptr_;
};

class MoveOnlyType {
 public:
  typedef MoveOnlyType Data_;
  MoveOnlyType();
  MoveOnlyType(MoveOnlyType&& other);
  MoveOnlyType& operator=(MoveOnlyType&& other);
  ~MoveOnlyType();

  bool moved() const { return moved_; }
  static size_t num_instances() { return num_instances_; }
  MoveOnlyType* ptr() const { return ptr_; }
  void ResetMoved() { moved_ = false; }

 private:
  bool moved_;
  static size_t num_instances_;
  MoveOnlyType* ptr_;

  MOJO_MOVE_ONLY_TYPE(MoveOnlyType);
};

}  // namespace mdl

#endif  // LIB_MDL_CPP_BINDINGS_TESTS_CONTAINER_TEST_UTIL_H_
