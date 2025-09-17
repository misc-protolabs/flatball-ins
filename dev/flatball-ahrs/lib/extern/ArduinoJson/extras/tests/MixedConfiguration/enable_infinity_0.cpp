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

#define ARDUINOJSON_ENABLE_INFINITY 0
#include <ArduinoJson.h>

#include <catch.hpp>
#include <limits>

static void assertParseFails(const char* json) {
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, json);

  REQUIRE(err == DeserializationError::InvalidInput);
}

static void assertJsonEquals(const JsonDocument& doc,
                             std::string expectedJson) {
  std::string actualJson;
  serializeJson(doc, actualJson);
  REQUIRE(actualJson == expectedJson);
}

TEST_CASE("ARDUINOJSON_ENABLE_INFINITY == 0") {
  SECTION("serializeJson()") {
    JsonDocument doc;
    doc.add(std::numeric_limits<double>::infinity());
    doc.add(-std::numeric_limits<double>::infinity());

    assertJsonEquals(doc, "[null,null]");
  }

  SECTION("deserializeJson()") {
    assertParseFails("{\"X\":Infinity}");
    assertParseFails("{\"X\":-Infinity}");
    assertParseFails("{\"X\":+Infinity}");
  }
}
