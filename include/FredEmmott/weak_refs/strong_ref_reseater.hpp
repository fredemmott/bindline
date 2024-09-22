// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT
#pragma once

#include <utility>

#include "lock_weak_ref.hpp"
#include "make_weak_ref.hpp"

namespace FredEmmott::weak_refs::inline weak_refs_reseaters {

template <strong_ref... Ts>
struct strong_ref_reseater {
  strong_ref_reseater(const strong_ref_reseater&) = delete;
  strong_ref_reseater& operator=(const strong_ref_reseater&) = delete;

  strong_ref_reseater(Ts*... init)
    : mStrongs(init...), mWeaks(make_weak_ref(*init)...) {
    ((*init = {nullptr}), ...);
  }

  ~strong_ref_reseater() {
    reseat();
  }

  bool reseat() {
    return this->reseat(std::make_index_sequence<sizeof...(Ts)>());
  }

 private:
  template <std::size_t... I>
  bool reseat(std::index_sequence<I...>) {
    return (reseat(get<I>(mStrongs), get<I>(mWeaks)) && ...);
  }

  bool reseat(strong_ref auto* strong, weak_ref auto& weak) {
    if (*strong) {
      return true;
    }
    *strong = lock_weak_ref(weak);
    return static_cast<bool>(*strong);
  }

  std::tuple<std::decay_t<Ts>*...> mStrongs;
  std::tuple<weak_ref_t<std::decay_t<Ts>>...> mWeaks;
};

}// namespace FredEmmott::weak_refs
