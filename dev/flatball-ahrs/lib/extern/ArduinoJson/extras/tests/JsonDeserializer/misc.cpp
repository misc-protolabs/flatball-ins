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

#include "Allocators.hpp"

using ArduinoJson::detail::sizeofArray;

TEST_CASE("deserializeJson() misc cases") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("null") {
    DeserializationError err = deserializeJson(doc, "null");
    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<float>() == false);
  }

  SECTION("true") {
    DeserializationError err = deserializeJson(doc, "true");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == true);
  }

  SECTION("false") {
    DeserializationError err = deserializeJson(doc, "false");

    REQUIRE(err == DeserializationError::Ok);
    REQUIRE(doc.is<bool>());
    REQUIRE(doc.as<bool>() == false);
  }

  SECTION("Should clear the JsonVariant") {
    deserializeJson(doc, "[1,2,3]");
    spy.clearLog();

    deserializeJson(doc, "{}");

    REQUIRE(doc.is<JsonObject>());
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofArray(3)),
                         });
  }
}
