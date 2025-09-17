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

#include <ArduinoJson/Memory/Alignment.hpp>
#include <ArduinoJson/Memory/ResourceManager.hpp>
#include <ArduinoJson/Memory/ResourceManagerImpl.hpp>
#include <catch.hpp>

#include "Allocators.hpp"

using namespace ArduinoJson::detail;

static void fullPreallocatedPools(ResourceManager& resources) {
  for (int i = 0;
       i < ARDUINOJSON_INITIAL_POOL_COUNT * ARDUINOJSON_POOL_CAPACITY; i++)
    resources.allocVariant();
}

TEST_CASE("ResourceManager::swap()") {
  SECTION("Both using preallocated pool list") {
    SpyingAllocator spy;
    ResourceManager a(&spy);
    ResourceManager b(&spy);

    auto a1 = a.allocVariant();
    auto b1 = b.allocVariant();

    swap(a, b);

    REQUIRE(a1.ptr() == b.getVariant(a1.id()));
    REQUIRE(b1.ptr() == a.getVariant(b1.id()));

    REQUIRE(spy.log() == AllocatorLog{
                             Allocate(sizeofPool()) * 2,
                         });
  }

  SECTION("Only left using preallocated pool list") {
    SpyingAllocator spy;
    ResourceManager a(&spy);
    ResourceManager b(&spy);
    fullPreallocatedPools(b);

    auto a1 = a.allocVariant();
    auto b1 = b.allocVariant();
    swap(a, b);

    REQUIRE(a1.ptr() == b.getVariant(a1.id()));
    REQUIRE(b1.ptr() == a.getVariant(b1.id()));

    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofPool()) * (ARDUINOJSON_INITIAL_POOL_COUNT + 1),
                Allocate(sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT * 2)),
                Allocate(sizeofPool()),
            });
  }

  SECTION("Only right using preallocated pool list") {
    SpyingAllocator spy;
    ResourceManager a(&spy);
    fullPreallocatedPools(a);
    ResourceManager b(&spy);

    auto a1 = a.allocVariant();
    auto b1 = b.allocVariant();
    swap(a, b);

    REQUIRE(a1.ptr() == b.getVariant(a1.id()));
    REQUIRE(b1.ptr() == a.getVariant(b1.id()));

    REQUIRE(spy.log() ==
            AllocatorLog{
                Allocate(sizeofPool()) * ARDUINOJSON_INITIAL_POOL_COUNT,
                Allocate(sizeofPoolList(ARDUINOJSON_INITIAL_POOL_COUNT * 2)),
                Allocate(sizeofPool()) * 2,
            });
  }

  SECTION("None is using preallocated pool list") {
    SpyingAllocator spy;
    ResourceManager a(&spy);
    fullPreallocatedPools(a);
    ResourceManager b(&spy);
    fullPreallocatedPools(b);

    auto a1 = a.allocVariant();
    auto b1 = b.allocVariant();

    swap(a, b);

    REQUIRE(a1.ptr() == b.getVariant(a1.id()));
    REQUIRE(b1.ptr() == a.getVariant(b1.id()));
  }
}
