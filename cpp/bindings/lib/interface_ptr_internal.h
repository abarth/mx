// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_MDL_CPP_BINDINGS_LIB_INTERFACE_PTR_INTERNAL_H_
#define LIB_MDL_CPP_BINDINGS_LIB_INTERFACE_PTR_INTERNAL_H_

#include <algorithm>  // For |std::swap()|.
#include <memory>
#include <utility>

#include "lib/mdl/cpp/bindings/callback.h"
#include "lib/mdl/cpp/bindings/interface_handle.h"
#include "lib/mdl/cpp/bindings/lib/control_message_proxy.h"
#include "lib/mdl/cpp/bindings/lib/message_header_validator.h"
#include "lib/mdl/cpp/bindings/lib/router.h"
#include "lib/ftl/logging.h"

struct MojoAsyncWaiter;

namespace mdl {
namespace internal {

template <typename Interface>
class InterfacePtrState {
 public:
  InterfacePtrState()
      : proxy_(nullptr), router_(nullptr), waiter_(nullptr), version_(0u) {}

  ~InterfacePtrState() {
    // Destruction order matters here. We delete |proxy_| first, even though
    // |router_| may have a reference to it, so that destructors for any request
    // callbacks still pending can interact with the InterfacePtr.
    delete proxy_;
    delete router_;
  }

  Interface* instance() {
    ConfigureProxyIfNecessary();

    // This will be null if the object is not bound.
    return proxy_;
  }

  uint32_t version() const { return version_; }

  void QueryVersion(const Callback<void(uint32_t)>& callback) {
    ConfigureProxyIfNecessary();

    // It is safe to capture |this| because the callback won't be run after this
    // object goes away.
    auto callback_wrapper = [this, callback](uint32_t version) {
      this->version_ = version;
      callback.Run(version);
    };

    // Do a static cast in case the interface contains methods with the same
    // name.
    static_cast<ControlMessageProxy*>(proxy_)->QueryVersion(callback_wrapper);
  }

  void RequireVersion(uint32_t version) {
    ConfigureProxyIfNecessary();

    if (version <= version_)
      return;

    version_ = version;
    // Do a static cast in case the interface contains methods with the same
    // name.
    static_cast<ControlMessageProxy*>(proxy_)->RequireVersion(version);
  }

  void Swap(InterfacePtrState* other) {
    using std::swap;
    swap(other->proxy_, proxy_);
    swap(other->router_, router_);
    handle_.swap(other->handle_);
    swap(other->waiter_, waiter_);
    swap(other->version_, version_);
  }

  void Bind(InterfaceHandle<Interface> info, const MojoAsyncWaiter* waiter) {
    FTL_DCHECK(!proxy_);
    FTL_DCHECK(!router_);
    FTL_DCHECK(!handle_.is_valid());
    FTL_DCHECK(!waiter_);
    FTL_DCHECK(version_ == 0u);
    FTL_DCHECK(info.is_valid());

    handle_ = info.PassHandle();
    waiter_ = waiter;
    version_ = info.version();
  }

  bool WaitForIncomingResponse(mx_time_t deadline = MX_TIME_INFINITE) {
    ConfigureProxyIfNecessary();

    FTL_DCHECK(router_);
    return router_->WaitForIncomingMessage(deadline);
  }

  // After this method is called, the object is in an invalid state and
  // shouldn't be reused.
  InterfaceHandle<Interface> PassInterfaceHandle() {
    return InterfaceHandle<Interface>(
        router_ ? router_->PassMessagePipe() : handle_.Pass(), version_);
  }

  bool is_bound() const { return handle_.is_valid() || router_; }

  bool encountered_error() const {
    return router_ ? router_->encountered_error() : false;
  }

  void set_connection_error_handler(const ftl::Closure& error_handler) {
    ConfigureProxyIfNecessary();

    FTL_DCHECK(router_);
    router_->set_connection_error_handler(error_handler);
  }

  Router* router_for_testing() {
    ConfigureProxyIfNecessary();
    return router_;
  }

 private:
  using Proxy = typename Interface::Proxy_;

  void ConfigureProxyIfNecessary() {
    // The proxy has been configured.
    if (proxy_) {
      FTL_DCHECK(router_);
      return;
    }
    // The object hasn't been bound.
    if (!waiter_) {
      FTL_DCHECK(!handle_.is_valid());
      return;
    }

    MessageValidatorList validators;
    validators.push_back(
        std::unique_ptr<MessageValidator>(new MessageHeaderValidator));
    validators.push_back(std::unique_ptr<MessageValidator>(
        new typename Interface::ResponseValidator_));

    router_ = new Router(std::move(handle_), std::move(validators), waiter_);
    waiter_ = nullptr;

    proxy_ = new Proxy(router_);
  }

  Proxy* proxy_;
  Router* router_;

  // |proxy_| and |router_| are not initialized until read/write with the
  // message pipe handle is needed. |handle_| and |waiter_| are valid between
  // the Bind() call and the initialization of |proxy_| and |router_|.
  mx::msgpipe handle_;
  const MojoAsyncWaiter* waiter_;

  uint32_t version_;

  FTL_DISALLOW_COPY_AND_ASSIGN(InterfacePtrState);
};

}  // namespace internal
}  // namespace mdl

#endif  // LIB_MDL_CPP_BINDINGS_LIB_INTERFACE_PTR_INTERNAL_H_
