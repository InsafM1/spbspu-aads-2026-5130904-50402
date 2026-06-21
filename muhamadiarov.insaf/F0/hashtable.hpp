#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include "vector.hpp"

namespace muhamadiarov
{
  template < class Key, class Value >
  class Node
  {
    std::pair< Key, Value > value_;
    size_t psl_;
    bool occupied_;
    Node();
    void swap(Node& other) noexcept;
    void clear() noexcept;
  };

  template < class Key, class Value, class Hash, class Equal >
  class RTIter;

  template < class Key, class Value, class Hash, class Equal >
  class RTCIter;

  template < class Key, class Value, class Hash, class Equal >
  class RobinTable
  {
  public:
    using Iterator = RTIter<Key, Value, Hash, Equal>;
    using ConstIterator = RTCIter<Key, Value, Hash, Equal>;

    RobinTable();
    explicit RobinTable(size_t slots, float max_load = 0.7f);
    RobinTable(std::initializer_list< std::pair< Key, Value > > il);
    RobinTable(const RobinTable& other);
    RobinTable(RobinTable&& other) noexcept;
    RobinTable& operator=(const RobinTable& other);
    RobinTable& operator=(RobinTable&& other) noexcept;

    ~RobinTable();
    
    void add(Key k, Value v);
    void drop(Key k);
    Value &get(Key k);
    const Value &get(Key k) const;
    bool has(Key k) const noexcept;
    void rehash(size_t slots);

    void clear() noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;
    float maxLoadFactor() const noexcept;
    
    Iterator begin() noexcept;
    Iterator end() noexcept;
    ConstIterator begin() const noexcept;
    ConstIterator end() const noexcept;
    ConstIterator cbegin() const noexcept;
    ConstIterator cend() const noexcept;

    void swap(RobinTable& other) noexcept;
  private:
    Vector< Node< Key, Value > > data_;
    Hash hasher_;
    Equal equal_;
    size_t capacity_;
    size_t size_;
    float maxLoad_;

    friend class RTIter<Key, Value, Hash, Equal>;
    friend class RTCIter<Key, Value, Hash, Equal>;
  };
}

#endif
