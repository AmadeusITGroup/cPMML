
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_UTILS_H
#define CPMML_UTILS_H

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "options.h"

#include "miniz/miniz.h"

/**
 * @defgroup Utils
 *
 * Various utility functions, used also during model scoring.
 */
//@{
inline std::string to_lower(std::string value) {
  std::transform(value.begin(), value.end(), value.begin(), ::tolower);

  return value;
}

inline static double to_double(const std::string &value) {
  try {
    return std::stod(value);
  } catch (const std::invalid_argument &exception) {
    throw cpmml::ParsingException(value + " cannot be converted to double (invalid argument)");
  } catch (const std::out_of_range &exception) {
    throw cpmml::ParsingException(value + " cannot be converted to double (overflow)");
  }
}

template <class T>
T parse_string(const std::string &value) {
  std::istringstream is(to_lower(value));
  T parsed_value;
  is >> parsed_value;
  return parsed_value;
}

template <class T>
std::string to_string(const T &value) {
  std::stringstream buffer;

  buffer << value;

  return buffer.str();
}

template <class T>
std::string mkstring(const std::vector<T> &values, const std::string &separator = ",") {
  std::stringstream buffer;

  buffer << "[";
  auto it = values.cbegin();
  auto jt = it;
  jt++;
  for (; it != values.cend(); it++, jt++) buffer << *it << (jt == values.cend() ? "" : separator);

  buffer << "]";

  return buffer.str();
}

template <class T>
std::string mkstring(const std::set<T> &values, const std::string &separator = ",") {
  std::stringstream buffer;

  buffer << "[";
  auto it = values.cbegin();
  auto jt = it;
  jt++;
  for (; it != values.cend(); it++, jt++) buffer << *it << (jt == values.cend() ? "" : separator);

  buffer << "]";

  return buffer.str();
}

template <class T, class H>
std::string mkstring(const std::unordered_set<T, H> &values, const std::string &separator = ",") {
  std::stringstream buffer;

  buffer << "[";
  auto it = values.cbegin();
  auto jt = it;
  jt++;
  for (; it != values.cend(); it++, jt++) buffer << *it << (jt == values.cend() ? "" : separator);

  buffer << "]";

  return buffer.str();
}

template <class K, class V, class H>
std::string to_string(const std::unordered_map<K, V, H> &values, const std::string &separator = ",") {
  std::stringstream buffer;

  buffer << "{";
  auto it = values.cbegin();
  size_t j = 1;
  for (; it != values.cend(); it++, j++)
    buffer << "\"" << it->first << "\":\"" << it->second << (j == values.size() ? "\"" : "\"" + separator);

  buffer << "}";

  return buffer.str();
}

template <class T, class U>
std::string mkstring(const std::vector<std::pair<T, U>> &values, const std::string &separator = ",") {
  std::stringstream buffer;

  buffer << "[";
  auto it = values.cbegin();
  auto jt = it;
  jt++;
  for (; it != values.cend(); it++, jt++) buffer << to_string<T, U>(*it) << (jt == values.cend() ? "" : separator);

  buffer << "]";

  return buffer.str();
}

template <class T, class V>
std::string to_string(const std::pair<T, V> &value) {
  std::stringstream buffer;

  buffer << "{";
  buffer << "\"" << value.first << "\":\"" << value.second << "\"";
  buffer << "}";

  return buffer.str();
}

template <class K, class V>
std::vector<V> to_values(const std::unordered_map<K, V> &values) {
  std::vector<V> result;
  result.reserve(values.size());
  for (const auto &value : values) result.push_back(value.second);

  return result;
}

template <class K, class V>
std::vector<V> to_values(const std::map<K, V> &values) {
  std::vector<V> result;
  result.reserve(values.size());
  for (const auto &value : values) result.push_back(value.second);

  return result;
}

template <class K, class V>
std::vector<K> to_keys(const std::unordered_map<K, V> &values) {
  std::vector<K> result;
  result.reserve(values.size());
  for (const auto &value : values) result.push_back(value.first);

  return result;
}

inline std::vector<std::string> split(std::string value, const std::string &separator) {
  size_t pos = 0;
  std::string token;
  std::vector<std::string> tokens;

  while ((pos = value.find(separator)) != std::string::npos) {
    token = value.substr(0, pos);
    if (token.size() > 0) tokens.push_back(token);
    value.erase(0, pos + separator.length());
  }

  tokens.push_back(value);

  return tokens;
}

inline std::string remove_all(std::string value, char to_remove) {
  value.erase(std::remove(value.begin(), value.end(), to_remove), value.end());

  return value;
}

inline bool parse_boolstring(const std::string &value) { return value == "true"; }

inline double double_min() { return std::numeric_limits<double>::min(); }

static inline std::string &ltrim(std::string &s) {
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) { return ltrim(rtrim(s)); }

static inline std::vector<char> read_xml(const std::string &filepath) {
  std::ifstream file(filepath);
  std::vector<char> data((std::istreambuf_iterator<char>(file)), {});
  data.push_back('\0');

  return data;
}

static inline void mz_reader_error(mz_zip_archive *zip_archive, const std::string &message) {
  mz_zip_reader_end(zip_archive);
  throw cpmml::ParsingException("unzip - err: " + message);
}

static inline std::vector<char> read_zip(const std::string &filepath) {
  mz_zip_archive zip_archive;
  size_t inflated_size;
  void *p;

  memset(&zip_archive, 0, sizeof(zip_archive));
  if (mz_zip_reader_init_file(&zip_archive, filepath.c_str(), 0) != MZ_STREAM_END)
    throw cpmml::ParsingException("unzip - err: reading archive");
  if (mz_zip_reader_get_num_files(&zip_archive) < 1) mz_reader_error(&zip_archive, "no file in archive");
  if (mz_zip_reader_is_file_a_directory(&zip_archive, 0)) mz_reader_error(&zip_archive, "directory in archive");
  if ((p = mz_zip_reader_extract_to_heap(&zip_archive, 0, &inflated_size, 0)) == NULL)
    mz_reader_error(&zip_archive, "decompressing");
  std::vector<char> data((char *)p, (char *)p + inflated_size);
  data.push_back('\0');

  mz_free(p);
  mz_zip_reader_end(&zip_archive);

  return data;
}

static inline bool file_exists(const std::string &name) {
  std::ifstream f(name.c_str());
  return f.good();
}

static inline std::vector<char> read_file(const std::string &filepath, const bool zipped) {
  if (!file_exists(filepath)) throw cpmml::ParsingException("Input file " + filepath + " does not exist");

  if (zipped) return read_zip(filepath);

  return read_xml(filepath);
}

template <class T>
inline std::string format_num(const T &value) {
  std::stringstream sstr_value;
  std::string str_value;
  std::stringstream res;
  sstr_value.precision(std::numeric_limits<T>::max_digits10);
  sstr_value << value;
  str_value = sstr_value.str();
  if (str_value.find("e") != std::string::npos) return str_value;
  std::vector<std::string> parts = split(str_value, ".");

  std::reverse(parts[0].begin(), parts[0].end());
  for (auto i = 0u; i < parts[0].size(); i++) {
    if (i != 0 && i % 3 == 0) res << ",";

    res << parts[0][i];
  }

  std::string res_str = res.str();
  std::reverse(res_str.begin(), res_str.end());
  return res_str + (parts.size() > 1 ? "." + (parts[1].size() > 3 ? parts[1].substr(0, 3) : parts[1]) : "");
}

inline std::string format_int(const int &value) {
  std::stringstream sstr_value;
  std::string str_value;
  std::stringstream res;
  sstr_value.precision(std::numeric_limits<int>::max_digits10);
  sstr_value << value;
  str_value = sstr_value.str();
  if (str_value.find("e") != std::string::npos) return str_value;

  std::reverse(str_value.begin(), str_value.end());
  for (auto i = 0u; i < str_value.size(); i++) {
    if (i != 0 && i % 3 == 0) res << ",";

    res << str_value[i];
  }

  std::string res_str = res.str();
  std::reverse(res_str.begin(), res_str.end());
  return res_str;
}

template <typename T, typename... Args>
inline std::unique_ptr<T> make_unique(Args &&... args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
//}@
#endif
