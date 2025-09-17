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

#include <stdlib.h>  // malloc, free
#include <string>

#include "Allocators.hpp"
#include "Literals.hpp"

TEST_CASE("JsonDocument::clear()") {
  SpyingAllocator spy;
  JsonDocument doc(&spy);

  SECTION("null") {
    doc.clear();

    REQUIRE(doc.isNull());
    REQUIRE(spy.log() == AllocatorLog{});
  }

  SECTION("releases resources") {
    doc["hello"_s] = "world"_s;
    spy.clearLog();

    doc.clear();

    REQUIRE(doc.isNull());
    REQUIRE(spy.log() == AllocatorLog{
                             Deallocate(sizeofPool()),
                             Deallocate(sizeofString("hello")),
                             Deallocate(sizeofString("world")),
                         });
  }

  SECTION("clear free list") {  // issue #2034
    JsonObject obj = doc.to<JsonObject>();
    obj["a"] = 1;
    obj.clear();  // puts the slot in the free list

    doc.clear();

    doc["b"] = 2;  // will it pick from the free list?
  }
}
