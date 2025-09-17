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

#define ARDUINOJSON_USE_DOUBLE 0
#include <ArduinoJson.h>

#include <catch.hpp>

namespace my {
using ArduinoJson::detail::isinf;
}  // namespace my

void checkFloat(const char* input, float expected) {
  using ArduinoJson::detail::NumberType;
  using ArduinoJson::detail::parseNumber;
  CAPTURE(input);
  auto result = parseNumber(input);
  REQUIRE(result.type() == NumberType::Float);
  REQUIRE(result.asFloat() == Approx(expected));
}

TEST_CASE("ARDUINOJSON_USE_DOUBLE == 0") {
  SECTION("serializeJson()") {
    JsonDocument doc;
    JsonObject root = doc.to<JsonObject>();

    root["pi"] = 3.14;
    root["e"] = 2.72;

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"pi\":3.14,\"e\":2.72}");
  }

  SECTION("parseNumber()") {
    using ArduinoJson::detail::NumberType;
    using ArduinoJson::detail::parseNumber;

    SECTION("Large positive number") {
      auto result = parseNumber("1e300");
      REQUIRE(result.type() == NumberType::Float);
      REQUIRE(result.asFloat() > 0);
      REQUIRE(my::isinf(result.asFloat()));
    }

    SECTION("Large negative number") {
      auto result = parseNumber("-1e300");
      REQUIRE(result.type() == NumberType::Float);
      REQUIRE(result.asFloat() < 0);
      REQUIRE(my::isinf(result.asFloat()));
    }

    SECTION("Too small to be represented") {
      auto result = parseNumber("1e-300");
      REQUIRE(result.type() == NumberType::Float);
      REQUIRE(result.asFloat() == 0);
    }

    SECTION("MantissaTooLongToFit") {
      checkFloat("0.340282346638528861111111111111", 0.34028234663852886f);
      checkFloat("34028234663852886.11111111111111", 34028234663852886.0f);
      checkFloat("34028234.66385288611111111111111", 34028234.663852886f);

      checkFloat("-0.340282346638528861111111111111", -0.34028234663852886f);
      checkFloat("-34028234663852886.11111111111111", -34028234663852886.0f);
      checkFloat("-34028234.66385288611111111111111", -34028234.663852886f);
    }
  }
}
