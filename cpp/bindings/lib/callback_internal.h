// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_MDL_CPP_BINDINGS_LIB_CALLBACK_INTERNAL_H_
#define LIB_MDL_CPP_BINDINGS_LIB_CALLBACK_INTERNAL_H_

#include "lib/mdl/cpp/bindings/lib/template_util.h"

namespace mdl {
class String;

namespace internal {

template <typename T>
struct Callback_ParamTraits {
  typedef T ForwardType;
};

template <>
struct Callback_ParamTraits<String> {
  typedef const String& ForwardType;
};

}  // namespace internal
}  // namespace mdl

#endif  // LIB_MDL_CPP_BINDINGS_LIB_CALLBACK_INTERNAL_H_
