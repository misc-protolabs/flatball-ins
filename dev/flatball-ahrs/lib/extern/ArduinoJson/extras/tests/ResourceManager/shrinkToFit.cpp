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

#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Memory/ResourceManagerImpl.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

TEST_CASE("ResourceManager::shrinkToFit()") {
  SpyingAllocator spyingAllocator;
  ResourceManager resources(&spyingAllocator);

  SECTION("empty") {
    resources.shrinkToFit();

    REQUIRE(spyingAllocator.log() == AllocatorLog{});
  }

  SECTION("only one pool") {
    resources.allocVariant();

    resources.shrinkToFit();

    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofPool()),
                Reallocate(sizeofPool(), sizeofPool(1)),
            });
  }

  SECTION("more pools than initial count") {
    for (size_t i = 0;
         i < ARDUINOJSON_POOL_CAPACITY * ARDUINOJSON_INITIAL_POOL_COUNT + 1;
         i++)
      resources.allocVariant();
    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Allocate(sizeofPool()) * ARDUINOJSON_INITIAL_POOL_COUNT,
                Allocate(sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT * 2)),
                Allocate(sizeofPool()),
            });

    spyingAllocator.clearLog();
    resources.shrinkToFit();

    REQUIRE(spyingAllocator.log() ==
            AllocatorLog{
                Reallocate(sizeofPool(), sizeofPool(1)),
                Reallocate(sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT * 2),
                           sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT + 1)),
            });
  }
}
