// Copyright 2014 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef LIB_FIDL_CPP_BINDINGS_BINDING_SET_H_
#define LIB_FIDL_CPP_BINDINGS_BINDING_SET_H_

#include <assert.h>

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "lib/ftl/macros.h"
#include "lib/fidl/cpp/bindings/binding.h"

namespace fidl {

// Use this class to manage a set of bindings each of which is
// owned by the pipe it is bound to.
template <typename Interface>
class BindingSet {
 public:
  BindingSet() {}
  ~BindingSet() { CloseAllBindings(); }

  // Adds a binding to the list and arranges for it to be removed when
  // a connection error occurs.  Does not take ownership of |impl|, which
  // must outlive the binding set.
  void AddBinding(Interface* impl, InterfaceRequest<Interface> request) {
    bindings_.emplace_back(new Binding<Interface>(impl, std::move(request)));
    auto* binding = bindings_.back().get();
    // Set the connection error handler for the newly added Binding to be a
    // function that will erase it from the vector.
    binding->set_connection_error_handler(
        std::bind(&BindingSet<Interface>::RemoveOnError, this, binding));
  }

  // Adds a binding to the list and arranges for it to be removed when
  // a connection error occurs.  Does not take ownership of |impl|, which
  // must outlive the binding set.
  InterfaceHandle<Interface> AddBinding(Interface* impl) {
    bindings_.emplace_back(new Binding<Interface>(impl));
    auto* binding = bindings_.back().get();
    InterfaceHandle<Interface> interface;
    binding->Bind(&interface);
    // Set the connection error handler for the newly added Binding to be a
    // function that will erase it from the vector.
    binding->set_connection_error_handler(
        std::bind(&BindingSet<Interface>::RemoveOnError, this, binding));

    return std::move(interface);
  }

  void CloseAllBindings() { bindings_.clear(); }

  size_t size() const { return bindings_.size(); }

 private:
  void RemoveOnError(Binding<Interface>* binding) {
    auto it =
        std::find_if(bindings_.begin(), bindings_.end(),
                     [binding](const std::unique_ptr<Binding<Interface>>& b) {
                       return (b.get() == binding);
                     });
    assert(it != bindings_.end());
    bindings_.erase(it);
  }

  std::vector<std::unique_ptr<Binding<Interface>>> bindings_;

  FTL_DISALLOW_COPY_AND_ASSIGN(BindingSet);
};

}  // namespace fidl

#endif  // LIB_FIDL_CPP_BINDINGS_BINDING_SET_H_
