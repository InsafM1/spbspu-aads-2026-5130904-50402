#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <cstddef>
#include <string>
#include <functional>
#include "hashTable.hpp"
#include "hasher.hpp"
#include <list.hpp>

namespace muhamadiarov
{
  struct Graph
  {
    using key_t = std::pair< std::string, std::string >;
    List< std::string > vertices_;
    HashTable< key_t, List< size_t >, SipHash< std::string >, std::equal_to< key_t > > bonds_;
    Graph() noexcept;
    explicit Graph(size_t bucketCapacity, size_t bucketCounts);
    void addVertex(const std::string &vertex);
    void removeVertex(const std::string &vertex);
    void addConnection(const std::string &from, const std::string &to, size_t weight);
    void removeConnection(const std::string &from, const std::string &to, size_t weight);
  };
}
#endif
