#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "hashtable.hpp"
#include "vector.hpp"
#include "edge.hpp"
#include <list.hpp>
#include <lciter.hpp>
#include <liter.hpp>
#include <string>

namespace muhamadiarov
{ 
  struct PairHash
  {
    size_t operator()(const std::pair< int, int>& p) const
    {
      SipHash< int > hasher;
      return hasher(p.first) ^ (hasher(p.second) << 1);
    }
  };

  class Graph
  {
  public:
    using Key_t = std::pair< int, int >;
    using GraphTable_t = RobinTable< Key_t, List< Edge >, PairHash, std::equal_to< Key_t > >;
    using PointVableTable_t = RobinTable< int, int, SipHash< int >, std::equal_to< int > >;

    Graph();
    explicit Graph(size_t countConnections, size_t countTown);
    ~Graph() = default;

    bool findVertex(int vertex) const;
    bool findConnection(int from, const Edge& edge) const;
    bool findConnection(int from, int to, RoadType type, size_t distance) const;
    
    void addVertex(int vertex);
    void removeVertex(int vertex) noexcept;

    void addConnection(int from, int to, RoadType type, size_t distance);
    void addConnection(int from, Edge& edge);
    void removeConnection(int from, int to, RoadType type, size_t distance) noexcept;
    void removeConnection(int from, const Edge& edge) noexcept;

    void removeAllConnection(int from, int to) noexcept;
    void clear() noexcept;

    bool getEdges(int from, int to, List< Edge >& edges) const;
    bool hasConnection(int from, int to) const;
    List< std::pair< int, Edge > > getOutBounds(int from) const;
    List< std::pair< int, Edge > > getIncoming(int to) const;
  private:
    List< int > vertices_;
    GraphTable_t connections_;
    PointVableTable_t pointValues_;
  };
}

namespace muh = muhamadiarov;

muh::Graph::Graph():
  connections_(16),
  pointValues_(16)
{}

muh::Graph::Graph(size_t countConnections, size_t countTown):
  connections_(countConnections),
  pointValues_(countTown)
{}

bool muh::Graph::findVertex(int vertex) const
{
  LCIter< int > iter = vertices_.cbegin();
  for (size_t i = 0; i < vertices_.size(); ++i)
  {
    if (*iter == vertex)
    {
      return true;
    }
    ++iter;
  }
  return false;
}

bool muh::Graph::findConnection(int from, const Edge& edge) const
{
  Key_t pair{from, edge.to_};
  const List< Edge >& list = connections_.get(pair);
  LCIter< Edge > iter = list.cbegin();
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (*iter == edge)
    {
      return true;
    }
    ++iter;
  }
  return false;
}

bool muh::Graph::findConnection(int from, int to, RoadType type, size_t distance) const
{
  const Edge edge{to, type, distance};
  return findConnection(from, edge);
}

void muh::Graph::addVertex(int vertex)
{
  if (findVertex(vertex))
  {
    return;
  }
  vertices_.pushBack(vertex);
}

void muh::Graph::removeVertex(int vertex) noexcept
{
  if (!findVertex(vertex))
  {
    return;
  }
  LIter< int > iter = vertices_.begin();
  for (size_t i = 0; i < vertices_.size(); ++i)
  {
    if (*iter == vertex)
    {
      iter = vertices_.erase(iter);
    }
    else
    {
      ++iter;
    }
  }

  List< Key_t > toRemove;
  RTIter< Key_t, List< Edge >, PairHash, std::equal_to< Key_t > > it = connections_.begin();
  while (it != connections_.end())
  {
    const Key_t& key = it->first;
    if (key.first == vertex || key.second == vertex)
    {
      toRemove.pushBack(key);
    }
  }

  LIter< Key_t > rmIter = toRemove.begin();
  for (size_t i = 0; i < toRemove.size(); ++i)
  {
    connections_.drop(*rmIter);
    ++rmIter;
  }
}

void muh::Graph::addConnection(int from, int to, RoadType type, size_t distance)
{
  Edge edge{to, type, distance};
  addConnection(from, edge);
}
void muh::Graph::addConnection(int from, Edge& edge)
{
  addVertex(from);
  addVertex(edge.to_);

  Key_t pair{from, edge.to_};
  if (connections_.has(pair))
  {
    List< Edge >& list = connections_.get(pair);
    list.pushFront(edge);
  }
  else
  {
    List< Edge > newList;
    newList.pushFront(edge);
    connections_.add(pair, newList);
  }
}

void muh::Graph::removeConnection(int from, int to, RoadType type, size_t distance) noexcept
{
  const Edge edge{to, type, distance};
  removeConnection(from, edge);
}

void muh::Graph::removeConnection(int from, const Edge& edge) noexcept
{
  if (!findConnection(from, edge))
  {
    return;
  }

  Key_t pair{from, edge.to_};
  List< Edge >& list = connections_.get(pair);
  LIter< Edge > iter = list.begin();
  for (size_t i = 0; i < list.size(); ++i)
  {
    if (*iter == edge)
    {
      iter = list.erase(iter);
      break;
    }
    else
    {
      ++iter;
    }
  }
  if (list.size() == 0)
  {
    connections_.drop(pair);
  }
}

void muh::Graph::removeAllConnection(int from, int to) noexcept
{
  Key_t pair{from, to};
  connections_.drop(pair);
}

void muh::Graph::clear() noexcept
{
  vertices_.clear();
  connections_.clear();
  pointValues_.clear();
}

bool muh::Graph::getEdges(int from, int to, List< Edge >& edges) const
{
  Key_t pair{from, to};
  if (connections_.has(pair))
  {
    edges = connections_.get(pair);
    return true;
  }
  return false;
}

bool muh::Graph::hasConnection(int from, int to) const
{
  Key_t pair{from, to};
  return connections_.has(pair);
}

muh::List< std::pair< int, muh::Edge > > muh::Graph::getOutBounds(int from) const
{
  List< std::pair< int, Edge > > result;
  for (auto it = connections_.cbegin(); it != connections_.cend(); ++it)
  {
    if (it->first.first == from)
    {
      const List< Edge >& edges = it->second;
      LCIter< Edge > edgeIter = edges.cbegin();
      for (size_t i = 0; i < edges.size(); ++i)
      {
          result.pushBack({it->first.second, *edgeIter});
          ++edgeIter;
      }
    }
  }
  return result;
}

muh::List< std::pair< int, muh::Edge > > muh::Graph::getIncoming(int to) const
{
  List< std::pair< int, Edge > > result;
  for (auto it = connections_.cbegin(); it != connections_.cend(); ++it)
  {
    if (it->first.second == to)
    {
      const List< Edge >& edges = it->second;
      LCIter< Edge > edgeIter = edges.cbegin();
      for (size_t i = 0; i < edges.size(); ++i)
      {
        result.pushBack({it->first.first, *edgeIter});
        ++edgeIter;
      }
    }
  }
  return result;
}
#endif
