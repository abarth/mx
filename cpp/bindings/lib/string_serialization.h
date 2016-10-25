// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_MDL_CPP_BINDINGS_LIB_STRING_SERIALIZATION_H_
#define LIB_MDL_CPP_BINDINGS_LIB_STRING_SERIALIZATION_H_

#include "lib/mdl/cpp/bindings/lib/array_internal.h"
#include "lib/mdl/cpp/bindings/string.h"

namespace mdl {

size_t GetSerializedSize_(const String& input);
void SerializeString_(const String& input,
                      internal::Buffer* buffer,
                      internal::String_Data** output);

void Deserialize_(internal::String_Data* input, String* output);

}  // namespace mdl

#endif  // LIB_MDL_CPP_BINDINGS_LIB_STRING_SERIALIZATION_H_
