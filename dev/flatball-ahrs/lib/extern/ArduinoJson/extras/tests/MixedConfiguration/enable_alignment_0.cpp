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

#define ARDUINOJSON_VERSION_NAMESPACE NoAlignment
#define ARDUINOJSON_ENABLE_ALIGNMENT 0
#include <ArduinoJson.h>

#include <catch.hpp>

TEST_CASE("ARDUINOJSON_ENABLE_ALIGNMENT == 0") {
  using namespace ArduinoJson::detail;

  const size_t N = sizeof(void*);

  SECTION("isAligned()") {
    CHECK(isAligned(0) == true);
    CHECK(isAligned(1) == true);
    CHECK(isAligned(N) == true);
    CHECK(isAligned(N + 1) == true);
    CHECK(isAligned(2 * N) == true);
    CHECK(isAligned(2 * N + 1) == true);
  }

  SECTION("addPadding()") {
    CHECK(addPadding(0) == 0);
    CHECK(addPadding(1) == 1);
    CHECK(addPadding(N) == N);
    CHECK(addPadding(N + 1) == N + 1);
  }

  SECTION("AddPadding<>") {
    const size_t a = AddPadding<0>::value;
    CHECK(a == 0);

    const size_t b = AddPadding<1>::value;
    CHECK(b == 1);

    const size_t c = AddPadding<N>::value;
    CHECK(c == N);

    const size_t d = AddPadding<N + 1>::value;
    CHECK(d == N + 1);
  }
}
