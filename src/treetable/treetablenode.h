
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef TREE_TABLE_NODE_H
#define TREE_TABLE_NODE_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

/**
 * @class TreeTableNode
 *
 * Basic building block of a TreeTable, it can be seen as a column of the table.
 */
template <class K, class V, class H>
class TreeTableNode {
 public:
  K key;
  V value;
  std::unordered_map<K, std::shared_ptr<TreeTableNode>, H> children;

  TreeTableNode() = default;

  explicit TreeTableNode(const V &value) : value(value) {}

  void add(const std::vector<K> &keys, const V &value) {
    if (keys.size() == 0) {
      this->value = value;
      return;
    }
    if (children.find(keys.front()) == children.cend()) children[keys.front()] = std::make_shared<TreeTableNode>();
    children[keys.front()]->add(std::vector<K>(keys.begin() + 1, keys.end()), value);
  }

  inline V get(const std::vector<K> &keys) const {
    if (children.size() == 0) return value;
    return children.at(keys.front())->get(std::vector<K>(keys.begin() + 1, keys.end()));
  }

  inline V get(const std::vector<K> &keys, const size_t &level) const {
    if (children.size() == 0) return value;
    return children.at(keys[level])->get(keys, level + 1);
  }
};

#endif
