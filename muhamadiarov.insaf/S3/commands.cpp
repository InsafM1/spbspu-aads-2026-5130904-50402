#include "commands.hpp"

namespace muhamadiarov
{
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
}
