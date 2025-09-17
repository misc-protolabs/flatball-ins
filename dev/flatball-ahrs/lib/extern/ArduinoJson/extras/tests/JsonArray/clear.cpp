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

TEST_CASE("JsonArray::clear()") {
  SECTION("No-op on null JsonArray") {
    JsonArray array;
    array.clear();
    REQUIRE(array.isNull() == true);
    REQUIRE(array.size() == 0);
  }

  SECTION("Removes all elements") {
    JsonDocument doc;
    JsonArray array = doc.to<JsonArray>();
    array.add(1);
    array.add(2);
    array.clear();
    REQUIRE(array.size() == 0);
    REQUIRE(array.isNull() == false);
  }

  SECTION("Removed elements are recycled") {
    SpyingAllocator spy;
    JsonDocument doc(&spy);
    JsonArray array = doc.to<JsonArray>();

    // fill the pool entirely
    for (int i = 0; i < ARDUINOJSON_POOL_CAPACITY; i++)
      array.add(i);

    // clear and fill again
    array.clear();
    for (int i = 0; i < ARDUINOJSON_POOL_CAPACITY; i++)
      array.add(i);

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()),
                         });
  }
}
