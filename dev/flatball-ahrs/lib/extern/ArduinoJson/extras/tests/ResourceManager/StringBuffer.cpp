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

#include <ArduinoJson/Memory/StringBuffer.hpp>
#include <catch.hpp>

#include "Allocators.hpp"
#include "Literals.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("StringBuffer") {
  SpyingAllocator spy;
  ResourceManager resources(&spy);
  StringBuffer sb(&resources);
  VariantData variant;

  SECTION("Tiny string") {
    auto ptr = sb.reserve(3);
    strcpy(ptr, "hi!");
    sb.save(&variant);

    REQUIRE(variant.type() == VariantType::TinyString);
    REQUIRE(variant.asString() == "hi!");
  }

  SECTION("Tiny string can't contain NUL") {
    auto ptr = sb.reserve(3);
    memcpy(ptr, "a\0b", 3);
    sb.save(&variant);

    REQUIRE(variant.type() == VariantType::LongString);

    auto str = variant.asString();
    REQUIRE(str.size() == 3);
    REQUIRE(str.c_str()[0] == 'a');
    REQUIRE(str.c_str()[1] == 0);
    REQUIRE(str.c_str()[2] == 'b');
  }

  SECTION("Tiny string can't have 4 characters") {
    auto ptr = sb.reserve(4);
    strcpy(ptr, "alfa");
    sb.save(&variant);

    REQUIRE(variant.type() == VariantType::LongString);
    REQUIRE(variant.asString() == "alfa");
  }
}
