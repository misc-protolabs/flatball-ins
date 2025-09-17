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

TEST_CASE("JsonDocument::overflowed()") {
  TimebombAllocator timebomb(10);
  JsonDocument doc(&timebomb);

  SECTION("returns false on a fresh object") {
    timebomb.setCountdown(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed insertion") {
    timebomb.setCountdown(0);
    doc.add(0);
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after successful insertion") {
    timebomb.setCountdown(2);
    doc.add(0);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed string copy") {
    timebomb.setCountdown(0);
    doc.add("example"_s);
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful string copy") {
    timebomb.setCountdown(3);
    doc.add("example"_s);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true after a failed member add") {
    timebomb.setCountdown(0);
    doc["example"] = true;
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns true after a failed deserialization") {
    timebomb.setCountdown(0);
    deserializeJson(doc, "[1, 2]");
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false after a successful deserialization") {
    timebomb.setCountdown(3);
    deserializeJson(doc, "[\"example\"]");
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns false after clear()") {
    timebomb.setCountdown(0);
    doc.add(0);
    doc.clear();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains false after shrinkToFit()") {
    timebomb.setCountdown(2);
    doc.add(0);
    timebomb.setCountdown(2);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == false);
  }

  SECTION("remains true after shrinkToFit()") {
    timebomb.setCountdown(0);
    doc.add(0);
    timebomb.setCountdown(2);
    doc.shrinkToFit();
    CHECK(doc.overflowed() == true);
  }

  SECTION("returns false when string length doesn't overflow") {
    auto maxLength = ArduinoJson::detail::StringNode::maxLength;
    CHECK(doc.set(std::string(maxLength, 'a')) == true);
    CHECK(doc.overflowed() == false);
  }

  SECTION("returns true when string length overflows") {
    auto maxLength = ArduinoJson::detail::StringNode::maxLength;
    CHECK(doc.set(std::string(maxLength + 1, 'a')) == false);
    CHECK(doc.overflowed() == true);
  }
}
