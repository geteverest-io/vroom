/*

This file is part of VROOM.

Copyright (c) 2015-2025, Julien Coupey.
All rights reserved (see LICENSE).

*/

#include "structures/generic/edge.h"

namespace vroom::utils {

template <class T>
Edge<T>::Edge(Index first_vertex, Index second_vertex, T weight)
  : _first_vertex(std::min(first_vertex, second_vertex)),
    _second_vertex(std::max(first_vertex, second_vertex)),
    _weight(weight) {
}

template <class T> bool Edge<T>::operator<(const Edge& rhs) const {
  return (this->_first_vertex < rhs._first_vertex) ||
         ((this->_first_vertex == rhs._first_vertex) &&
          (this->_second_vertex < rhs._second_vertex));
}

template <class T> bool Edge<T>::operator==(const Edge& rhs) const {
  return (this->_first_vertex == rhs._first_vertex) &&
         (this->_second_vertex == rhs._second_vertex);
}

template class Edge<UserCost>;

} // namespace vroom::utils
