// Copyright (C) 2024 Fred Emmott <fred@fredemmott.com>
// SPDX-License-Identifier: MIT

#include <FredEmmott/bindline/detail/meta/parameter_pack_slice.hpp>

#include <catch2/catch_test_macros.hpp>
using namespace FredEmmott::bindline_detail::meta;

struct a_t {
  constexpr bool operator==(const a_t&) {
    return true;
  }
};
struct b_t {
  constexpr bool operator==(const b_t&) {
    return true;
  }
};
struct c_t {
  constexpr bool operator==(const c_t&) {
    return true;
  }
};

TEST_CASE("parameter_pack_slice") {
  STATIC_CHECK(parameter_pack_slice<0, 0>() == std::tuple {});

  STATIC_CHECK(
    parameter_pack_slice<0, 3>(a_t {}, b_t {}, c_t {})
    == std::tuple {a_t {}, b_t {}, c_t {}});
  STATIC_CHECK(
    parameter_pack_slice<0, 0>(a_t {}, b_t {}, c_t {}) == std::tuple {});
  STATIC_CHECK(
    parameter_pack_slice<2, 0>(a_t {}, b_t {}, c_t {}) == std::tuple {});
  STATIC_CHECK(
    parameter_pack_slice<0, 2>(a_t {}, b_t {}, c_t {})
    == std::tuple {a_t {}, b_t {}});
  STATIC_CHECK(
    parameter_pack_slice<1, 2>(a_t {}, b_t {}, c_t {})
    == std::tuple {b_t {}, c_t {}});
  STATIC_CHECK(
    parameter_pack_slice<0, 1>(a_t {}, b_t {}, c_t {}) == std::tuple {a_t {}});
  STATIC_CHECK(
    parameter_pack_slice<2, 1>(a_t {}, b_t {}, c_t {}) == std::tuple {c_t {}});
}

TEST_CASE("instantiate_from_parameter_pack_slice_t") {
  STATIC_CHECK(
    std::same_as<
      instantiate_from_parameter_pack_slice_t<0, 3, std::tuple, a_t, b_t, c_t>,
      std::tuple<a_t, b_t, c_t>>);
  STATIC_CHECK(
    std::same_as<
      instantiate_from_parameter_pack_slice_t<0, 0, std::tuple, a_t, b_t, c_t>,
      std::tuple<>>);
  STATIC_CHECK(
    std::same_as<
      instantiate_from_parameter_pack_slice_t<2, 0, std::tuple, a_t, b_t, c_t>,
      std::tuple<>>);
  STATIC_CHECK(
    std::same_as<
      instantiate_from_parameter_pack_slice_t<0, 2, std::tuple, a_t, b_t, c_t>,
      std::tuple<a_t, b_t>>);
  STATIC_CHECK(
    std::same_as<
      instantiate_from_parameter_pack_slice_t<1, 2, std::tuple, a_t, b_t, c_t>,
      std::tuple<b_t, c_t>>);
  STATIC_CHECK(
    std::same_as<
      instantiate_from_parameter_pack_slice_t<0, 1, std::tuple, a_t, b_t, c_t>,
      std::tuple<a_t>>);
  STATIC_CHECK(
    std::same_as<
      instantiate_from_parameter_pack_slice_t<2, 1, std::tuple, a_t, b_t, c_t>,
      std::tuple<c_t>>);
}