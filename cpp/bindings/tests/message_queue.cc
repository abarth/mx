// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/mdl/cpp/bindings/tests/message_queue.h"

#include "lib/mdl/cpp/bindings/message.h"
#include "mojo/public/cpp/environment/logging.h"

namespace mdl {
namespace test {

MessageQueue::MessageQueue() {}

MessageQueue::~MessageQueue() {
  while (!queue_.empty())
    Pop();
}

bool MessageQueue::IsEmpty() const {
  return queue_.empty();
}

void MessageQueue::Push(Message* message) {
  queue_.push(new Message());
  message->MoveTo(queue_.back());
}

void MessageQueue::Pop(Message* message) {
  MOJO_DCHECK(!queue_.empty());
  queue_.front()->MoveTo(message);
  Pop();
}

void MessageQueue::Pop() {
  MOJO_DCHECK(!queue_.empty());
  delete queue_.front();
  queue_.pop();
}

}  // namespace test
}  // namespace mdl
