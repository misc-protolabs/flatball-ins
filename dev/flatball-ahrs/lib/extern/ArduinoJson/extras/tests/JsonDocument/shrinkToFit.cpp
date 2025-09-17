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

using ArduinoJson::detail::sizeofArray;
using ArduinoJson::detail::sizeofObject;

class ArmoredAllocator : public Allocator {
 public:
  virtual ~ArmoredAllocator() {}

  void* allocate(size_t size) override {
    return malloc(size);
  }

  void deallocate(void* ptr) override {
    free(ptr);
  }

  void* reallocate(void* ptr, size_t new_size) override {
    // don't call realloc, instead alloc a new buffer and erase the old one
    // this way we make sure we support relocation
    void* new_ptr = malloc(new_size);
    memset(new_ptr, '#', new_size);  // erase
    if (ptr) {
      memcpy(new_ptr, ptr, std::min(new_size, new_size));
      free(ptr);
    }
    return new_ptr;
  }
};

TEST_CASE("JsonDocument::shrinkToFit()") {
  ArmoredAllocator armoredAllocator;
  SpyingAllocator spyingAllocator(&armoredAllocator);
  JsonDocument doc(&spyingAllocator);

  SECTION("null") {
    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "null");
    REQUIRE(spyingAllocator.log() == AllocatorLog{});
  }

  SECTION("empty object") {
    deserializeJson(doc, "{}");

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "{}");
    REQUIRE(spyingAllocator.log() == AllocatorLog{});
  }

  SECTION("empty array") {
    deserializeJson(doc, "[]");

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "[]");
    REQUIRE(spyingAllocator.log() == AllocatorLog{});
  }

  SECTION("string") {
    doc.set("abcdefg");
    REQUIRE(doc.as<std::string>() == "abcdefg");

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "abcdefg");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofString("abcdefg")),
                                     });
  }

  SECTION("raw string") {
    doc.set(serialized("[{},12]"));

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "[{},12]");
    REQUIRE(spyingAllocator.log() == AllocatorLog{
                                         Allocate(sizeofString("[{},12]")),
                                     });
  }

  SECTION("object key") {
    doc["abcdefg"_s] = 42;

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "{\"abcdefg\":42}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Allocate(sizeofString("abcdefg")),
                Reallocate(sizeofPool(), sizeofObject(1)),
            });
  }

  SECTION("string in array") {
    doc.add("abcdefg"_s);

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "[\"abcdefg\"]");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Allocate(sizeofString("abcdefg")),
                Reallocate(sizeofPool(), sizeofArray(1)),
            });
  }

  SECTION("string in object") {
    doc["key"] = "abcdefg"_s;

    doc.shrinkToFit();

    REQUIRE(doc.as<std::string>() == "{\"key\":\"abcdefg\"}");
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Allocate(sizeofString("abcdefg")),
                Reallocate(sizeofPool(), sizeofPool(2)),
            });
  }
}
