/*******************************************************************************

 * Copyright 2019 AMADEUS. All rights reserved.

 * Author: Paolo Iannino

 *******************************************************************************/

#ifndef CPMML_STRING_VIEW_H
#define CPMML_STRING_VIEW_H

#include <stdexcept>
#include <string>

/**
 * @class string_view
 *
 * An implementation of a non-owning reference to std::string.
 */
namespace {
constexpr size_t lenR(const char *s, size_t result = 0) { return *s == '\0' ? result : lenR(s + 1, result + 1); }
}  // namespace

class string_view {
 public:
  constexpr string_view() noexcept : _data(nullptr), _size(0) {}
  constexpr string_view(const string_view &other) = default;
  constexpr string_view(const char *s, size_t s_size) : _data(s), _size(s_size) {}
  explicit constexpr string_view(const char *s) : _data(s), _size(lenR(s)) {}
  explicit inline string_view(const std::string &s) : _data(s.data()), _size(s.size()) {}

  string_view &operator=(const string_view &other) = default;
  constexpr const char *data() { return _data; }
  constexpr size_t size() const { return _size; }
  constexpr size_t length() const { return _size; }
  constexpr const char &operator[](size_t pos) const { return _data[pos]; }

  inline const char &at(size_t position) const {
    if (position >= _size) throw std::out_of_range("string_view: out of bound access");

    return _data[position];
  }

  inline int compare(const string_view &other) const {
    if (_size == other._size) return std::char_traits<char>::compare(_data, other._data, _size);

    return _size < other._size ? -1 : 1;
  }

  inline int compare(const char *other, size_t other_size) const {
    if (_size == other_size) return std::char_traits<char>::compare(_data, other, _size);

    return _size < other_size ? -1 : 1;
  }

  inline int compare(const char *other) const { return compare(string_view(other)); }

  inline int compare(const std::string &other) const {
    if (_size == other.size()) return std::char_traits<char>::compare(_data, other.data(), _size);

    return _size < other.size() ? -1 : 1;
  }

  inline bool operator==(const string_view &other) const { return compare(other) == 0; }
  inline bool operator==(const char *other) const { return compare(other) == 0; }
  inline bool operator==(const std::string &other) const { return compare(other) == 0; }
  inline bool operator!=(const string_view &other) const { return compare(other) != 0; }
  inline bool operator!=(const char *other) const { return compare(other) != 0; }
  inline bool operator!=(const std::string &other) const { return compare(other) != 0; }
  inline bool operator>(const string_view &other) const { return compare(other) > 0; }
  inline bool operator>(const char *other) const { return compare(other) > 0; }
  inline bool operator>(const std::string &other) const { return compare(other) > 0; }
  inline bool operator<(const string_view &other) const { return compare(other) < 0; }
  inline bool operator<(const char *other) const { return compare(other) < 0; }
  inline bool operator<(const std::string &other) const { return compare(other) < 0; }
  inline bool operator>=(const string_view &other) const { return compare(other) >= 0; }
  inline bool operator>=(const char *other) const { return compare(other) >= 0; }
  inline bool operator>=(const std::string &other) const { return compare(other) >= 0; }
  inline bool operator<=(const string_view &other) const { return compare(other) <= 0; }
  inline bool operator<=(const char *other) const { return compare(other) <= 0; }
  inline bool operator<=(const std::string &other) const { return compare(other) <= 0; }

  inline std::string to_string() const { return std::string(_data, _data + _size); }

 private:
  const char *_data;
  size_t _size;
};

#endif  // CPMML_STRING_VIEW_H_
