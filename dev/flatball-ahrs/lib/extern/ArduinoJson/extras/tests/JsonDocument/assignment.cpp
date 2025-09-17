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

TEST_CASE("JsonDocument assignment") {
  SpyingAllocator spyingAllocator;

  SECTION("Copy assignment same capacity") {
    JsonDocument doc1(&spyingAllocator);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    JsonDocument doc2(&spyingAllocator);
    spyingAllocator.clearLog();

    doc2 = doc1;

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");

    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofPool()),
                                         Allocate(sizeofString("hello")),
                                         Allocate(sizeofString("world")),
                                     });
  }

  SECTION("Copy assignment reallocates when capacity is smaller") {
    JsonDocument doc1(&spyingAllocator);
    deserializeJson(doc1, "[{\"hello\":\"world\"}]");
    JsonDocument doc2(&spyingAllocator);
    spyingAllocator.clearLog();

    doc2 = doc1;

    REQUIRE(doc2.as<std::string>() == "[{\"hello\":\"world\"}]");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofPool()),
                                         Allocate(sizeofString("hello")),
                                         Allocate(sizeofString("world")),
                                     });
  }

  SECTION("Copy assignment reallocates when capacity is larger") {
    JsonDocument doc1(&spyingAllocator);
    deserializeJson(doc1, "{\"hello\":\"world\"}");
    JsonDocument doc2(&spyingAllocator);
    spyingAllocator.clearLog();

    doc2 = doc1;

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofPool()),
                                         Allocate(sizeofString("hello")),
                                         Allocate(sizeofString("world")),
                                     });
  }

  SECTION("Move assign") {
    {
      JsonDocument doc1(&spyingAllocator);
      doc1["hello"_s] = "world"_s;
      JsonDocument doc2(&spyingAllocator);

      doc2 = std::move(doc1);

      REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");

      // NOLINTNEXTLINE(clang-analyzer-cplusplus.Move)
      REQUIRE(doc1.as<std::string>() == "null");
    }
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofPool()),
                                         Allocate(sizeofString("hello")),
                                         Allocate(sizeofString("world")),
                                         Deallocate(sizeofString("hello")),
                                         Deallocate(sizeofString("world")),
                                         Deallocate(sizeofPool()),
                                     });
  }

  SECTION("Assign from JsonObject") {
    JsonDocument doc1;
    JsonObject obj = doc1.to<JsonObject>();
    obj["hello"] = "world";

    JsonDocument doc2;
    doc2 = obj;

    REQUIRE(doc2.as<std::string>() == "{\"hello\":\"world\"}");
  }

  SECTION("Assign from JsonArray") {
    JsonDocument doc1;
    JsonArray arr = doc1.to<JsonArray>();
    arr.add("hello");

    JsonDocument doc2;
    doc2 = arr;

    REQUIRE(doc2.as<std::string>() == "[\"hello\"]");
  }

  SECTION("Assign from JsonVariant") {
    JsonDocument doc1;
    deserializeJson(doc1, "42");

    JsonDocument doc2;
    doc2 = doc1.as<JsonVariant>();

    REQUIRE(doc2.as<std::string>() == "42");
  }

  SECTION("Assign from MemberProxy") {
    JsonDocument doc1;
    doc1["value"] = 42;

    JsonDocument doc2;
    doc2 = doc1["value"];

    REQUIRE(doc2.as<std::string>() == "42");
  }

  SECTION("Assign from ElementProxy") {
    JsonDocument doc1;
    doc1[0] = 42;

    JsonDocument doc2;
    doc2 = doc1[0];

    REQUIRE(doc2.as<std::string>() == "42");
  }
}
