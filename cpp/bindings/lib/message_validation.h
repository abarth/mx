// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_MDL_CPP_BINDINGS_LIB_MESSAGE_VALIDATION_H_
#define LIB_MDL_CPP_BINDINGS_LIB_MESSAGE_VALIDATION_H_

#include <string>

#include "lib/mdl/cpp/bindings/lib/bounds_checker.h"
#include "lib/mdl/cpp/bindings/lib/validation_errors.h"
#include "lib/mdl/cpp/bindings/message.h"

namespace mdl {
namespace internal {

// Validates that the message is a request which doesn't expect a response.
ValidationError ValidateMessageIsRequestWithoutResponse(const Message* message,
                                                        std::string* err);
// Validates that the message is a request expecting a response.
ValidationError ValidateMessageIsRequestExpectingResponse(
    const Message* message,
    std::string* err);
// Validates that the message is a response.
ValidationError ValidateMessageIsResponse(const Message* message,
                                          std::string* err);

// Validates that the message payload is a valid struct of type ParamsType.
template <typename ParamsType>
ValidationError ValidateMessagePayload(const Message* message,
                                       std::string* err) {
  BoundsChecker bounds_checker(message->payload(), message->payload_num_bytes(),
                               message->handles()->size());
  return ParamsType::Validate(message->payload(), &bounds_checker, err);
}

}  // namespace internal
}  // namespace mdl

#endif  // LIB_MDL_CPP_BINDINGS_LIB_MESSAGE_VALIDATION_H_
