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

#pragma once

#include <string>

// Reproduces Arduino's String class
class String {
 public:
  String() = default;
  String(const char* s) {
    if (s)
      str_.assign(s);
  }

  void limitCapacityTo(size_t maxCapacity) {
    maxCapacity_ = maxCapacity;
  }

  unsigned char concat(const char* s) {
    return concat(s, strlen(s));
  }

  size_t length() const {
    return str_.size();
  }

  const char* c_str() const {
    return str_.c_str();
  }

  bool operator==(const char* s) const {
    return str_ == s;
  }

  String& operator=(const char* s) {
    if (s)
      str_.assign(s);
    else
      str_.clear();
    return *this;
  }

  char operator[](unsigned int index) const {
    if (index >= str_.size())
      return 0;
    return str_[index];
  }

  friend std::ostream& operator<<(std::ostream& lhs, const ::String& rhs) {
    lhs << rhs.str_;
    return lhs;
  }

 protected:
  // This function is protected in most Arduino cores
  unsigned char concat(const char* s, size_t n) {
    if (str_.size() + n > maxCapacity_)
      return 0;
    str_.append(s, n);
    return 1;
  }

 private:
  std::string str_;
  size_t maxCapacity_ = 1024;
};

class StringSumHelper : public ::String {};

inline bool operator==(const std::string& lhs, const ::String& rhs) {
  return lhs == rhs.c_str();
}
