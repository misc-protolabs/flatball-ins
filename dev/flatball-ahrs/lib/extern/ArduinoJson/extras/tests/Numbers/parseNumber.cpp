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

#include <ArduinoJson.hpp>
#include <catch.hpp>

using namespace ArduinoJson;
using namespace ArduinoJson::detail;

TEST_CASE("Test unsigned integer overflow") {
  Number first, second;

  // Avoids MSVC warning C4127 (conditional expression is constant)
  size_t integerSize = sizeof(JsonInteger);

  if (integerSize == 8) {
    first = parseNumber("18446744073709551615");
    second = parseNumber("18446744073709551616");
  } else {
    first = parseNumber("4294967295");
    second = parseNumber("4294967296");
  }

  REQUIRE(first.type() == NumberType::UnsignedInteger);
  REQUIRE(second.type() == NumberType::Double);
}

TEST_CASE("Test signed integer overflow") {
  Number first, second;

  // Avoids MSVC warning C4127 (conditional expression is constant)
  size_t integerSize = sizeof(JsonInteger);

  if (integerSize == 8) {
    first = parseNumber("-9223372036854775808");
    second = parseNumber("-9223372036854775809");
  } else {
    first = parseNumber("-2147483648");
    second = parseNumber("-2147483649");
  }

  REQUIRE(first.type() == NumberType::SignedInteger);
  REQUIRE(second.type() == NumberType::Double);
}

TEST_CASE("Invalid value") {
  auto result = parseNumber("6a3");

  REQUIRE(result.type() == NumberType::Invalid);
}

TEST_CASE("float") {
  auto result = parseNumber("3.402823e38");

  REQUIRE(result.type() == NumberType::Float);
}

TEST_CASE("double") {
  auto result = parseNumber("1.7976931348623157e308");

  REQUIRE(result.type() == NumberType::Double);
}
