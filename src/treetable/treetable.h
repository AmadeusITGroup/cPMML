
/*******************************************************************************
 * Copyright 2019 AMADEUS. All rights reserved.
 * Author: Paolo Iannino
 *******************************************************************************/

#ifndef CPMML_TREETABLE_H
#define CPMML_TREETABLE_H

#include "treetablenode.h"

/**
 * @class TreeTable
 *
 * Tables are an important part of PMML standard. For instance they are the main
 * building block for MapValues transformations.
 *
 * In order to reach max efficiency, this class implements a table with access
 * cost linear in the number of columns.
 *
 * In other words, given a table with m rows and n columns, this class has
 * access cost of O(n).
 */
template <class K, class V, class H>
class TreeTable {
 public:
  TreeTableNode<K, V, H> root;

  TreeTable() = default;

  void add(const std::vector<K> &keys, const V &value) { root.add(keys, value); }

  inline V get(const std::vector<K> &keys) const { return root.get(keys); }
};

#endif
