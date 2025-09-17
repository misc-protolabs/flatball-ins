// Copyright 2025 Michael V. Schaefer
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
// 
//     http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// ArduinoJson - https://arduinojson.org
// Copyright © 2014-2025, Benoit BLANCHON
// MIT License

#include <ArduinoJson/Numbers/FloatParts.hpp>
#include <catch.hpp>

using namespace ArduinoJson::detail;

TEST_CASE("decomposeFloat()") {
  SECTION("1.7976931348623157E+308") {
    auto parts = decomposeFloat(1.7976931348623157E+308, 9);
    REQUIRE(parts.integral == 1);
    REQUIRE(parts.decimal == 797693135);
    REQUIRE(parts.decimalPlaces == 9);
    REQUIRE(parts.exponent == 308);
  }

  SECTION("4.94065645841247e-324") {
    auto parts = decomposeFloat(4.94065645841247e-324, 9);
    REQUIRE(parts.integral == 4);
    REQUIRE(parts.decimal == 940656458);
    REQUIRE(parts.decimalPlaces == 9);
    REQUIRE(parts.exponent == -324);
  }

  SECTION("3.4E+38") {
    auto parts = decomposeFloat(3.4E+38f, 6);
    REQUIRE(parts.integral == 3);
    REQUIRE(parts.decimal == 4);
    REQUIRE(parts.decimalPlaces == 1);
    REQUIRE(parts.exponent == 38);
  }

  SECTION("1.17549435e−38") {
    auto parts = decomposeFloat(1.17549435e-38f, 6);
    REQUIRE(parts.integral == 1);
    REQUIRE(parts.decimal == 175494);
    REQUIRE(parts.decimalPlaces == 6);
    REQUIRE(parts.exponent == -38);
  }
}
