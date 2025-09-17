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
#include "Literals.hpp"

using ArduinoJson::detail::addPadding;

TEST_CASE("JsonDocument constructor") {
  SpyingAllocator spyingAllocator;

  SECTION("JsonDocument(size_t)") {
    { JsonDocument doc(&spyingAllocator); }
    REQUIRE(spyingAllocator.log() == AllocatorLog{});
  }

  SECTION("JsonDocument(const JsonDocument&)") {
    {
      JsonDocument doc1(&spyingAllocator);
      doc1.set("The size of this string is 32!!"_s);

      JsonDocument doc2(doc1);

      REQUIRE(doc1.as<std::string>() == "The size of this string is 32!!");
      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofStringBuffer()),
                                         Allocate(sizeofStringBuffer()),
                                         Deallocate(sizeofStringBuffer()),
                                         Deallocate(sizeofStringBuffer()),
                                     });
  }

  SECTION("JsonDocument(JsonDocument&&)") {
    {
      JsonDocument doc1(&spyingAllocator);
      doc1.set("The size of this string is 32!!"_s);

      JsonDocument doc2(std::move(doc1));

      REQUIRE(doc2.as<std::string>() == "The size of this string is 32!!");

      // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
      REQUIRE(doc1.as<std::string>() == "null");
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofStringBuffer()),
                                         Deallocate(sizeofStringBuffer()),
                                     });
  }

  SECTION("JsonDocument(JsonObject, Allocator*)") {
    JsonDocument doc1;
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2(obj, &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofPool()),
                                         Allocate(sizeofString("hello")),
                                         Allocate(sizeofString("world")),
                                     });
  }

  SECTION("JsonDocument(JsonObject)") {
    JsonDocument doc1;
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2(obj);

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("JsonDocument(JsonArray, Allocator*)") {
    JsonDocument doc1;
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2(arr, &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofPool()),
                                         Allocate(sizeofString("hello")),
                                     });
  }

  SECTION("JsonDocument(JsonArray)") {
    JsonDocument doc1;
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2(arr);

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
  }

  SECTION("JsonDocument(JsonVariant, Allocator*)") {
    JsonDocument doc1;
    deserializeJson(doc1, "\"hello\"");

    JsonDocument doc2(doc1.as<JsonVariant>(), &spyingAllocator);

    REQUIRE(doc2.as<std::string>() == "hello");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofString("hello")),
                                     });
  }

  SECTION("JsonDocument(JsonVariant)") {
    JsonDocument doc1;
    deserializeJson(doc1, "\"hello\"");

    JsonDocument doc2(doc1.as<JsonVariant>());

    REQUIRE(doc2.as<std::string>() == "hello");
  }

  SECTION("JsonDocument(JsonVariantConst)") {
    JsonDocument doc1;
    deserializeJson(doc1, "\"hello\"");

    JsonDocument doc2(doc1.as<JsonVariantConst>());

    REQUIRE(doc2.as<std::string>() == "hello");
  }

  SECTION("JsonDocument(ElementProxy)") {
    JsonDocument doc1;
    deserializeJson(doc1, "[\"hello\",\"world\"]");

    JsonDocument doc2(doc1[1]);

    REQUIRE(doc2.as<std::string>() == "world");
  }

  SECTION("JsonDocument(MemberProxy)") {
    JsonDocument doc1;
    deserializeJson(doc1, "{\"hello\":\"world\"}");

    JsonDocument doc2(doc1["hello"]);

    REQUIRE(doc2.as<std::string>() == "world");
  }
}
