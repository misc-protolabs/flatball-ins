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

#define ARDUINOJSON_ENABLE_INFINITY 1
#include <ArduinoJson.h>

#include <catch.hpp>
#include <limits>

namespace my {
using ArduinoJson::detail::isinf;
}  // namespace my

TEST_CASE("ARDUINOJSON_ENABLE_INFINITY == 1") {
  JsonDocument doc;

  SECTION("serializeJson()") {
    doc.add(std::numeric_limits<double>::infinity());
    doc.add(-std::numeric_limits<double>::infinity());

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "[Infinity,-Infinity]");
  }

  SECTION("deserializeJson()") {
    DeserializationError err =
        deserializeJson(doc, "[Infinity,-Infinity,+Infinity]");
    float a = doc[0];
    float b = doc[1];
    float c = doc[2];

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(my::isinf(a));
    REQUIRE(a > 0);
    REQUIRE(my::isinf(b));
    REQUIRE(b < 0);
    REQUIRE(my::isinf(c));
    REQUIRE(c > 0);
  }
}
