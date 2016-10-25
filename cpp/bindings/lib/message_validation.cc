// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/mdl/cpp/bindings/lib/message_validation.h"

#include <string>

#include "lib/mdl/cpp/bindings/lib/validation_errors.h"
#include "lib/mdl/cpp/bindings/message.h"

namespace mdl {
namespace internal {

ValidationError ValidateMessageIsRequestWithoutResponse(const Message* message,
                                                        std::string* err) {
  if (message->has_flag(kMessageIsResponse)) {
    MOJO_INTERNAL_DEBUG_SET_ERROR_MSG(err)
        << "message should be a request, not a response";
    return ValidationError::MESSAGE_HEADER_INVALID_FLAGS;
  }
  if (message->has_flag(kMessageExpectsResponse)) {
    MOJO_INTERNAL_DEBUG_SET_ERROR_MSG(err)
        << "message should not expect a response";
    return ValidationError::MESSAGE_HEADER_INVALID_FLAGS;
  }
  return ValidationError::NONE;
}

ValidationError ValidateMessageIsRequestExpectingResponse(
    const Message* message,
    std::string* err) {
  if (message->has_flag(kMessageIsResponse)) {
    MOJO_INTERNAL_DEBUG_SET_ERROR_MSG(err)
        << "message should be a request, not a response";
    return ValidationError::MESSAGE_HEADER_INVALID_FLAGS;
  }
  if (!message->has_flag(kMessageExpectsResponse)) {
    MOJO_INTERNAL_DEBUG_SET_ERROR_MSG(err)
        << "message should expect a response";
    return ValidationError::MESSAGE_HEADER_INVALID_FLAGS;
  }
  return ValidationError::NONE;
}

ValidationError ValidateMessageIsResponse(const Message* message,
                                          std::string* err) {
  if (message->has_flag(kMessageExpectsResponse) ||
      !message->has_flag(kMessageIsResponse)) {
    MOJO_INTERNAL_DEBUG_SET_ERROR_MSG(err) << "message should be a response";
    return ValidationError::MESSAGE_HEADER_INVALID_FLAGS;
  }
  return ValidationError::NONE;
}

}  // namespace internal
}  // namespace mdl
