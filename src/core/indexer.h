
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_INDEXER_H
#define CPMML_INDEXER_H

#include <string>
#include <unordered_map>
#include <utility>

#include "datatype.h"

/**
 * @class Indexer
 * Class used to index all features available in the PMML file.
 *
 * In cPMML features are indexed to gain constant access time. This class keeps
 * the associations name-index, along with the type of the feature.
 */
class Indexer {
 private:
  size_t _size = 0;
  std::unordered_map<std::string, size_t> name_index;
  std::unordered_map<std::string, DataType> name_datatype;
  std::unordered_map<size_t, std::string> index_name;
  std::unordered_map<size_t, DataType> index_datatype;

 public:
  Indexer() = default;

  std::pair<size_t, DataType> get(const std::string &name) const {
    return std::make_pair(name_index.at(name), name_datatype.at(name));
  }

  size_t get_index(const std::string &name) const { return name_index.at(name); }

  DataType get_type(const std::string &name) const { return name_datatype.at(name); }

  std::string get_name(const size_t &index) const { return index_name.at(index); }

  DataType get_type(const size_t &index) const { return index_datatype.at(index); }

  bool contains(const std::string &name) const { return name_index.find(name) != name_index.cend(); }

  std::string random_name() {
    std::string random = std::to_string(rand());
    while (name_index.find(random) != name_index.end()) random = std::to_string(rand());

    return random;
  }

  size_t get_or_set(const std::string &name) {
    if (name_index.find(name) == name_index.end()) {
      name_index[name] = _size;
      index_name[_size] = name;
      return _size++;
    }

    return name_index[name];
  }

  std::pair<size_t, DataType> get_or_set(const std::string &name, const DataType &datatype) {
    if (name_datatype.find(name) == name_datatype.end()) {  // set datatype in case just name is set
      name_datatype[name] = datatype;
      if (name_index.find(name) == name_index.end()) {  // set both in case neither name is set
        name_index[name] = _size;
        index_name[_size] = name;
        index_datatype[_size] = datatype;
        return std::make_pair(_size++, datatype);
      }
      index_datatype[name_index[name]] = datatype;
      return std::make_pair(name_index[name], datatype);
    }

    return get(name);
  }

  size_t size() { return _size; }

  std::unordered_map<std::string, size_t>::const_iterator cbegin() { return name_index.cbegin(); }
  std::unordered_map<std::string, size_t>::const_iterator cend() { return name_index.cend(); }
  std::unordered_map<std::string, size_t>::const_iterator begin() { return name_index.begin(); }
  std::unordered_map<std::string, size_t>::const_iterator end() { return name_index.end(); }
};

#endif
