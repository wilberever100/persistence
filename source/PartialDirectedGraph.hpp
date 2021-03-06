// Copyright 2020 Roger Peralta Aranibar
#ifndef SOURCE_PARTIALDIRECTEDGRAPH_HPP_
#define SOURCE_PARTIALDIRECTEDGRAPH_HPP_

#include <utility>

#include "DirectedGraph.hpp"

namespace ADE {
namespace Persistence {

template <typename Type>
class PartialNode : public Node<Type> {
 public:
  typedef Type data_type;
  PartialNode(data_type data, std::size_t const& out_ptrs_size,
              std::size_t const& in_ptrs_size)
      : Node<Type>(data, out_ptrs_size) {}

  data_type get_data() { return *PointerMachine::Node<Type>::data_; }

  data_type get_data(unsigned int version) { return *Node<Type>::data_; }

  bool set_data(data_type const data) {
    *Node<Type>::data_ = data;
    return true;
  }

  bool set_ptr(PartialNode* ptr, unsigned int id) {
    Node<Type>::forward_ = ptr;
    return true;
  }

  PartialNode& operator[](std::size_t id) const {
    return *dynamic_cast<PartialNode*>(&(Node<Type>::operator[](id)));
  }

  PartialNode& operator[](
      std::pair<std::size_t, unsigned int> id_version) const {
    return *dynamic_cast<PartialNode*>(
        &(Node<Type>::operator[](id_version.first)));
  }
};

template <typename Type, typename Node = PartialNode<Type>>
class PartialDirectedGraph : public DirectedGraph<Type, Node> {
 public:
  typedef Type data_type;

  PartialDirectedGraph(data_type const data, std::size_t const& out_ptrs_size,
                       std::size_t const& in_ptrs_size)
      : DirectedGraph<Type, Node>(data, out_ptrs_size),
        in_ptrs_size_(in_ptrs_size),
        current_version(0) {}

  Node* get_root_ptr() {
    return DirectedGraph<Type, Node>::get_root_ptr();
  }

  Node* get_root_ptr(unsigned int version) {
    return DirectedGraph<Type, Node>::get_root_ptr();
  }

  Node* insert_vertex(data_type const data, Node* u, std::size_t position) {
    ++current_version;
    return insert_vertex(data, u, position, current_version);
  }

  void add_edge(Node* u, Node* v, std::size_t position) {
    ++current_version;
    return add_edge(u, v, position, current_version);
  }

 private:
  Node* insert_vertex(data_type const data, Node* u, std::size_t position,
                      unsigned int version) {
    return DirectedGraph<Type, Node>::insert_vertex(data, u, position);
  }

  void add_edge(Node* u, Node* v, std::size_t position, unsigned int version) {
    return DirectedGraph<Type, Node>::add_edge(u, v, position);
  }

  std::size_t in_ptrs_size_;
  unsigned int current_version;
};

}  // namespace Persistence
}  // namespace ADE

#endif  // SOURCE_PARTIALDIRECTEDGRAPH_HPP_
