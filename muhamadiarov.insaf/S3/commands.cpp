#include "commands.hpp"

namespace muhamadiarov
{
  using HTIter_t = HTIter< std::string, Graph, SipHash<std::string>, std::equal_to<std::string> >;  

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
      out << "<INVALID COMMAND>";
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
}
