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
#include <string>

TEST_CASE("JsonObject::remove()") {
  JsonDocument doc;
  JsonObject obj = doc.to<JsonObject>();
  obj["a"] = 0;
  obj["b"] = 1;
  obj["c"] = 2;
  std::string result;

  SECTION("remove(key)") {
    SECTION("Remove first") {
      obj.remove("a");
      serializeJson(obj, result);
      REQUIRE("{\"b\":1,\"c\":2}" == result);
    }

    SECTION("Remove middle") {
      obj.remove("b");
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"c\":2}" == result);
    }

    SECTION("Remove last") {
      obj.remove("c");
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"b\":1}" == result);
    }
  }

  SECTION("remove(iterator)") {
    JsonObject::iterator it = obj.begin();

    SECTION("Remove first") {
      obj.remove(it);
      serializeJson(obj, result);
      REQUIRE("{\"b\":1,\"c\":2}" == result);
    }

    SECTION("Remove middle") {
      ++it;
      obj.remove(it);
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"c\":2}" == result);
    }

    SECTION("Remove last") {
      ++it;
      ++it;
      obj.remove(it);
      serializeJson(obj, result);
      REQUIRE("{\"a\":0,\"b\":1}" == result);
    }
  }

#ifdef HAS_VARIABLE_LENGTH_ARRAY
  SECTION("key is a vla") {
    size_t i = 16;
    char vla[i];
    strcpy(vla, "b");
    obj.remove(vla);

    serializeJson(obj, result);
    REQUIRE("{\"a\":0,\"c\":2}" == result);
  }
#endif

  SECTION("remove by key on unbound reference") {
    JsonObject unboundObject;
    unboundObject.remove("key");
  }

  SECTION("remove by iterator on unbound reference") {
    JsonObject unboundObject;
    unboundObject.remove(unboundObject.begin());
  }

  SECTION("remove(JsonVariant)") {
    obj["key"] = "b";
    obj.remove(obj["key"]);
    REQUIRE("{\"a\":0,\"c\":2,\"key\":\"b\"}" == doc.as<std::string>());
  }
}
