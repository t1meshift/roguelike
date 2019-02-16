// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <catch2/catch.hpp>
#include "../utils/PRNG.h"

TEST_CASE("PRNG constructs", "[prng]") {
  PRNG p;
}

TEMPLATE_TEST_CASE("PRNG works with integral types", "[prng]", int, char, long, short, unsigned short) {
  PRNG p(228);
  REQUIRE(p.get<TestType>(0, 120) == 51);
}

TEMPLATE_TEST_CASE("PRNG doesn't work with non-integral types", "[prng]", float, double, long double) {
  PRNG p(228);
  REQUIRE(std::is_same_v<decltype(p.get<TestType>(2.3, 4.5)), void>);
}