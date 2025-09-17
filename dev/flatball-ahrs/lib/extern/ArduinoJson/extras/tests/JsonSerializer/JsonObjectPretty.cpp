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

#include <ArduinoJson.h>
#include <catch.hpp>
#include <string>

static void checkObjectPretty(const JsonObject obj,
                              const std::string expected) {
  char json[256];

  size_t actualLen = serializeJsonPretty(obj, json);
  size_t measuredLen = measureJsonPretty(obj);

  REQUIRE(json == expected);
  REQUIRE(expected.size() == actualLen);
  REQUIRE(expected.size() == measuredLen);
}

TEST_CASE("serializeJsonPretty(JsonObject)") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();

  SECTION("EmptyObject") {
    checkObjectPretty(obj, "{}");
  }

  SECTION("OneMember") {
    obj["key"] = "value";

    checkObjectPretty(obj,
                      "{\r\n"
                      "  \"key\": \"value\"\r\n"
                      "}");
  }

  SECTION("TwoMembers") {
    obj["key1"] = "value1";
    obj["key2"] = "value2";

    checkObjectPretty(obj,
                      "{\r\n"
                      "  \"key1\": \"value1\",\r\n"
                      "  \"key2\": \"value2\"\r\n"
                      "}");
  }

  SECTION("EmptyNestedContainers") {
    obj["key1"].to<JsonObject>();
    obj["key2"].to<JsonArray>();

    checkObjectPretty(obj,
                      "{\r\n"
                      "  \"key1\": {},\r\n"
                      "  \"key2\": []\r\n"
                      "}");
  }

  SECTION("NestedContainers") {
    JsonObject nested1 = obj["key1"].to<JsonObject>();
    nested1["a"] = 1;

    JsonArray nested2 = obj["key2"].to<JsonArray>();
    nested2.add(2);

    checkObjectPretty(obj,
                      "{\r\n"
                      "  \"key1\": {\r\n"
                      "    \"a\": 1\r\n"
                      "  },\r\n"
                      "  \"key2\": [\r\n"
                      "    2\r\n"
                      "  ]\r\n"
                      "}");
  }
}
