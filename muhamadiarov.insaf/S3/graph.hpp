#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <cstddef>
#include <string>
#include <functional>
#include <stdexcept>
#include <utility>
#include "hashTable.hpp"
#include "hasher.hpp"
#include <list.hpp>

namespace muhamadiarov
{
  struct PairHash
  {
    size_t operator()(const std::pair<std::string, std::string>& p) const
    {
      SipHash<std::string> hasher;
      return hasher(p.first) ^ (hasher(p.second) << 1);
    }
  };
  struct Graph
  {
    using key_t = std::pair< std::string, std::string >;
    List< std::string > vertices_;
    HashTable< key_t, List< size_t >, PairHash, std::equal_to< key_t > > bonds_;
    Graph();
    explicit Graph(size_t bucketCapacity, size_t bucketCounts);
    bool findV(const std::string& vertex) const;
    bool findC(const std::string& from, const std::string& to, size_t weight);
    void addVertex(const std::string& vertex);
    void removeVertex(const std::string& vertex);
    void addConnection(const std::string& from, const std::string& to, size_t weight);
    void removeConnection(const std::string& from, const std::string& to, size_t weight);
  };
}

namespace muh = muhamadiarov;

muh::Graph::Graph():
  vertices_(),
  bonds_(5, 5)
{}

muh::Graph::Graph(size_t bucketCapacity, size_t bucketCounts):
  vertices_(),
  bonds_(bucketCapacity, bucketCounts)
{}

bool muh::Graph::findV(const std::string& vertex) const
{
  LCIter< std::string  > iter = vertices_.cbegin();
  while (iter != vertices_.cend())
  {
    if (*iter == vertex)
    {
      return true;
    }
    ++iter;
  }
  return false;
}

bool muh::Graph::findC(const std::string& from, const std::string& to, size_t weight)
{
  key_t pair{from, to};
  if (!bonds_.has(pair))
  {
    return false;
  }
  List< size_t >& list = bonds_.get(pair);
  LCIter< size_t > iter = list.cbegin();
  while (iter != list.cend())
  {
    if (*iter == weight)
    {
      return true;
    }
    ++iter;
  }
  return false;
}

void muh::Graph::addVertex(const std::string& vertex)
{
  if (findV(vertex))
  {
    return;
  }
  vertices_.pushBack(vertex);
}

void muh::Graph::removeVertex(const std::string& vertex)
{
  LIter< std::string > iter = vertices_.begin();
  bool found = false;
  while (iter != vertices_.end())
  {
    if (*iter == vertex)
    {
      iter = vertices_.erase(iter);
      found = true;
    }
    else
    {
      ++iter;
    }
  }
  if (!found)
  {
    throw std::out_of_range("Vertex not found");
  }
  HTIter< key_t, List<size_t>, PairHash, std::equal_to<key_t> > it = bonds_.begin();
  while (it != bonds_.end())
  {
    const key_t& key = it->key_;
    if (key.first == vertex || key.second == vertex)
    {
      bonds_.drop(key);
    }
    ++it;
  }
}

void muh::Graph::addConnection(const std::string& from, const std::string& to, size_t weight)
{
  if (findC(from, to, weight))
  {
    return;
  }
  key_t pair{from, to};
  List< size_t >& list = bonds_.get(pair);
  list.pushFront(weight);
}

void muh::Graph::removeConnection(const std::string& from, const std::string& to, size_t weight)
{
  key_t pair{from, to};
  if (!bonds_.has(pair))
  {
    return;
  }
  List< size_t >& list = bonds_.get(pair);
  LIter< size_t > iter = list.begin();
  while (iter != list.end())
  {
    if (*iter == weight)
    {
      iter = list.erase(iter);
    }
    else
    {
      ++iter;
    }
  }
}
#endif
