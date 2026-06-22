#ifndef GRAPH_HPP
#define GRAPH_HPP
#include "hashtable.hpp"
#include "vector.hpp"
#include "edge.hpp"
#include <list.hpp>
#include <string>

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

  class Graph
  {
  public:
    using Key_t = std::pair< int, int >;
    using GraphTable_t = RobinTable< Key_t, List< Edge >, PairHash, std::equal_to< Key_t > >;
    using PointVableTable_t = RobinTable< int, int, SipHash< int >, std::equal_to< int > >;

    Graph();
    explicit Graph(size_t countConnections, size_t countTown);
    ~Graph();

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
#endif
