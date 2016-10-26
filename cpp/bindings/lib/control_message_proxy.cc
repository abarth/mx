// Copyright 2015 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "lib/fidl/cpp/bindings/lib/control_message_proxy.h"

#include "lib/fidl/cpp/bindings/lib/message_builder.h"
#include "lib/fidl/cpp/bindings/message.h"
#include "lib/ftl/macros.h"
#include "mojo/public/interfaces/bindings/interface_control_messages.mojom.h"

namespace fidl {
namespace internal {

namespace {

using RunCallback = Callback<void(QueryVersionResultPtr)>;

class RunResponseForwardToCallback : public MessageReceiver {
 public:
  RunResponseForwardToCallback(const RunCallback& callback)
      : callback_(callback) {}
  bool Accept(Message* message) override;

 private:
  RunCallback callback_;
  FTL_DISALLOW_COPY_AND_ASSIGN(RunResponseForwardToCallback);
};

bool RunResponseForwardToCallback::Accept(Message* message) {
  RunResponseMessageParams_Data* params =
      reinterpret_cast<RunResponseMessageParams_Data*>(
          message->mutable_payload());
  params->DecodePointersAndHandles(message->mutable_handles());

  RunResponseMessageParamsPtr params_ptr(RunResponseMessageParams::New());
  Deserialize_(params, params_ptr.get());

  callback_.Run(params_ptr->query_version_result.Pass());
  return true;
}

void SendRunMessage(MessageReceiverWithResponder* receiver,
                    QueryVersionPtr query_version,
                    const RunCallback& callback) {
  RunMessageParamsPtr params_ptr(RunMessageParams::New());
  params_ptr->reserved0 = 16u;
  params_ptr->reserved1 = 0u;
  params_ptr->query_version = query_version.Pass();

  size_t size = GetSerializedSize_(*params_ptr);
  RequestMessageBuilder builder(mojo::kRunMessageId, size);

  RunMessageParams_Data* params = nullptr;
  auto result = Serialize_(params_ptr.get(), builder.buffer(), &params);
  FTL_DCHECK(result == ValidationError::NONE);

  params->EncodePointersAndHandles(builder.message()->mutable_handles());
  MessageReceiver* responder = new RunResponseForwardToCallback(callback);
  if (!receiver->AcceptWithResponder(builder.message(), responder))
    delete responder;
}

void SendRunOrClosePipeMessage(MessageReceiverWithResponder* receiver,
                               RequireVersionPtr require_version) {
  RunOrClosePipeMessageParamsPtr params_ptr(RunOrClosePipeMessageParams::New());
  params_ptr->reserved0 = 16u;
  params_ptr->reserved1 = 0u;
  params_ptr->require_version = require_version.Pass();

  size_t size = GetSerializedSize_(*params_ptr);
  MessageBuilder builder(mojo::kRunOrClosePipeMessageId, size);

  RunOrClosePipeMessageParams_Data* params = nullptr;
  auto result = Serialize_(params_ptr.get(), builder.buffer(), &params);
  FTL_DCHECK(result == ValidationError::NONE);

  params->EncodePointersAndHandles(builder.message()->mutable_handles());
  bool ok = receiver->Accept(builder.message());
  MOJO_ALLOW_UNUSED_LOCAL(ok);
}

}  // namespace

ControlMessageProxy::ControlMessageProxy(MessageReceiverWithResponder* receiver)
    : receiver_(receiver) {}

void ControlMessageProxy::QueryVersion(
    const Callback<void(uint32_t)>& callback) {
  auto run_callback = [callback](QueryVersionResultPtr query_version_result) {
    callback.Run(query_version_result->version);
  };
  SendRunMessage(receiver_, QueryVersion::New(), run_callback);
}

void ControlMessageProxy::RequireVersion(uint32_t version) {
  RequireVersionPtr require_version(RequireVersion::New());
  require_version->version = version;
  SendRunOrClosePipeMessage(receiver_, require_version.Pass());
}

}  // namespace internal
}  // namespace fidl
