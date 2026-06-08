#include "commands.hpp"

namespace muhamadiarov
{
  using HTIter_t = HTIter< std::string, Graph, SipHash<std::string>, std::equal_to<std::string> >;  
  using HTIter_k_t = HTIter< Graph::key_t, List< size_t >, PairHash, std::equal_to< Graph::key_t > >;

  void sortStringList(List< std::string >& list)
  {
    if (list.size() <= 1)
    {
      return;
    }
    
    bool swapped = false;
    do
    {
      swapped = false;
      LIter< std::string > it = list.begin();
      LIter< std::string > next = it;
      ++next;
      while (next != list.end())
      {
        if (*next < *it)
        {
          std::swap(*it, *next);
          swapped = true;
        }
        ++it;
        ++next;
      }
    } while (swapped);
  }
  
  void sortWeights(List< size_t >& list)
  {
    if (list.size() <= 1)
    {
      return;
    }

    bool swapped = false;
    do
    {
      swapped = false;
      LIter< size_t > it = list.begin();
      LIter< size_t > next = it;
      ++next;
      while (next != list.end())
      {
        if (*next < *it)
        {
          std::swap(*it, *next);
          swapped = true;
        }
        ++it;
        ++next;
      }
    } while (swapped);
  }

  void sortOutboundList(List< std::pair< std::string, List< size_t > > >& list)
  {
    if (list.size() <= 1)
    {
      return;
    }

    bool swapped = false;
    do
    {
      swapped = false;
      LIter< std::pair< std::string, List< size_t > > > it = list.begin();
      LIter< std::pair< std::string, List< size_t > > > next = it;
      ++next;
      while (next != list.end())
      {
        if (next->first < it->first)
        {
          std::swap(*it, *next);
          swapped = true;
        }
        ++it;
        ++next;
      }
    } while (swapped);
  }

  void cmdGraphs(std::istream&, std::ostream& out, GraphTable& graphs)
  {
    List< std::string > names;
    HTIter_t it = graphs.begin();
    while (it != graphs.end())
    {
      names.pushBack(it->key_);
      ++it;
    }

    sortStringList(names);

    LCIter< std::string > printIt = names.cbegin();
    while (printIt != names.cend())
    {
      out << *printIt << '\n';
      ++printIt;
    }
  }

  void cmdVertexes(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;
    in >> graphName;
    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    const Graph& graph = graphs.cget(graphName);
    List< std::string > vertices;
    LCIter< std::string > it = graph.vertices_.cbegin();
    while (it != graph.vertices_.cend())
    {
      vertices.pushBack(*it);
    }

    sortStringList(vertices);

    LCIter< std::string > printIt = vertices.cbegin();
    while (printIt != vertices.cend())
    {
      out << *printIt << '\n';
      ++printIt;
    }
  }

  void cmdOutbound(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;
    std::string vertex;
    in >> graphName >> vertex;
    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
  
    Graph& graph = graphs.get(graphName);
    
    if (!graph.findV(vertex))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    
    List< std::pair< std::string, List< size_t > > > outbound;
    HTIter_k_t it = graph.bonds_.begin();
    while (it != graph.bonds_.end())
    {
      const Graph::key_t& key = it->key_;
      if (key.second == vertex)
      {
        std::pair< std::string, List< size_t > > item;
        item.first = key.first;
        List< size_t > weights;
        LCIter<size_t> wIt = it->value_.cbegin();
        while (wIt != it->value_.cend())
        {
          weights.pushBack(*wIt);
          ++wIt;
        }

        sortWeights(weights);
        item.second = weights;
        outbound.pushBack(item);
      }
      ++it;
    }

    sortOutboundList(outbound);

    LIter<std::pair<std::string, List<size_t> > > printIt = outbound.begin();
    while (printIt != outbound.end())
    {
      out << printIt->first;
      LCIter<size_t> wIt = printIt->second.cbegin();
      while (wIt != printIt->second.cend())
      {
        out << " " << *wIt;
        ++wIt;
      }
      out << "\n";
      ++printIt;
    }
  }

  void cmdInbound(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;
    std::string vertex;
    in >> graphName >> vertex;

    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }
    
    Graph& graph = graphs.get(graphName);
    
    if (!graph.findV(vertex))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    List<std::pair<std::string, List<size_t> > > inbound;
    HTIter_k_t it = graph.bonds_.begin();
    while (it != graph.bonds_.end())
    {
      const Graph::key_t& key = it->key_;
      if (key.second == vertex)
      {
        std::pair<std::string, List<size_t> > item;
        item.first = key.first;

        List<size_t> weights;
        LCIter<size_t> wIt = it->value_.cbegin();
        while (wIt != it->value_.cend())
        {
          weights.pushBack(*wIt);
          ++wIt;
        }

        sortWeights(weights);
        item.second = weights;
        inbound.pushBack(item);
      }
      ++it;
    }

    sortOutboundList(inbound);

    LIter<std::pair<std::string, List<size_t> > > printIt = inbound.begin();
    while (printIt != inbound.end())
    {
      out << printIt->first;
      LCIter<size_t> wIt = printIt->second.cbegin();
      while (wIt != printIt->second.cend())
      {
        out << " " << *wIt;
        ++wIt;
      }
      out << "\n";
      ++printIt;
    }
  }

  void cmdBind(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;
    std::string vertexA;
    std::string vertexB;
    size_t weight;
    in >> graphName >> vertexA >> vertexB >> weight;

    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph& graph = graphs.get(graphName);

    if (!graph.findV(vertexA))
    {
      graph.addVertex(vertexA);
    }
    if (!graph.findV(vertexB))
    {
      graph.addVertex(vertexB);
    }

    graph.addConnection(vertexA, vertexB, weight);
  }

  void cmdCut(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;
    std::string vertexA;
    std::string vertexB;
    size_t weight;
    in >> graphName >> vertexA >> vertexB >> weight;

    if (!graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph& graph = graphs.get(graphName);

    if (!graph.findV(vertexA) || !graph.findV(vertexB))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    try
    {
      graph.removeConnection(vertexA, vertexB, weight);
    }
    catch (...)
    {
      out << "<INVALID COMMAND>\n";
    }
  }

  void cmdCreate(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;
    size_t vertexCount;
    in >> graphName >> vertexCount;

    if (graphs.has(graphName))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph newGraph(5, 5);
    for (size_t i = 0; i < vertexCount; ++i)
    {
      std::string vertex;
      in >> vertex;
      newGraph.addVertex(vertex);
    }

    graphs.add(graphName, newGraph);
  }
  
  void cmdMerge(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string newName;
    std::string oldName1;
    std::string oldName2;
    in >> newName >> oldName1 >> oldName2;

    if (graphs.has(newName) || !graphs.has(oldName1) || !graphs.has(oldName2))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Graph& g1 = graphs.get(oldName1);
    Graph& g2 = graphs.get(oldName2);
    Graph merged(5, 5);

    LCIter<std::string> vIt = g1.vertices_.cbegin();
    while (vIt != g1.vertices_.cend())
    {
      merged.addVertex(*vIt);
      ++vIt;
    }

    vIt = g2.vertices_.cbegin();
    while (vIt != g2.vertices_.cend())
    {
      merged.addVertex(*vIt);
      ++vIt;
    }

    HTIter_k_t eIt= g1.bonds_.begin();
    while (eIt != g1.bonds_.end())
    {
      const Graph::key_t& key = eIt->key_;
      LCIter<size_t> wIt = eIt->value_.cbegin();
      while (wIt != eIt->value_.cend())
      {
        merged.addConnection(key.first, key.second, *wIt);
        ++wIt;
      }
      ++eIt;
    }

    eIt = g2.bonds_.begin();
    while (eIt != g2.bonds_.end())
    {
      const Graph::key_t& key = eIt->key_;
      LCIter<size_t> wIt = eIt->value_.cbegin();
      while (wIt != eIt->value_.cend())
      {
        merged.addConnection(key.first, key.second, *wIt);
        ++wIt;
      }
      ++eIt;
    }

    graphs.add(newName, merged);
  }
}
