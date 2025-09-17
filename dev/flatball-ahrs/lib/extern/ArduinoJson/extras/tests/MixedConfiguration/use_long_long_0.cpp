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

#define ARDUINOJSON_USE_LONG_LONG 0
#include <ArduinoJson.h>

#include <catch.hpp>

#include "Literals.hpp"

TEST_CASE("ARDUINOJSON_USE_LONG_LONG == 0") {
  JsonDocument doc;

  SECTION("smoke test") {
    doc["A"] = 42;
    doc["B"] = 84;

    std::string json;
    serializeJson(doc, json);

    REQUIRE(json == "{\"A\":42,\"B\":84}");
  }

  SECTION("deserializeMsgPack()") {
    SECTION("cf 00 00 00 00 ff ff ff ff") {
      auto err =
          deserializeMsgPack(doc, "\xcf\x00\x00\x00\x00\xff\xff\xff\xff"_s);

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.as<uint32_t>() == 0xFFFFFFFF);
    }

    SECTION("cf 00 00 00 01 00 00 00 00") {
      auto err =
          deserializeMsgPack(doc, "\xcf\x00\x00\x00\x01\x00\x00\x00\x00"_s);

      REQUIRE(err == DeserializationError::Ok);
      REQUIRE(doc.isNull());
    }
  }
}
