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
}
