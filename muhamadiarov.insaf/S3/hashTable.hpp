#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP
#include <stdexcept>
#include <utility>

namespace muhamadiarov
{
  template < class Key, class Value >
  struct Slot
  {
    Key key_;
    Value value_;
    bool empty_ = true;
    Slot() = default;
    Slot(const Key& key, const Value& value);
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    class HTIterator;
    class HTCIterator;

    HashTable(size_t bucketCapacity, size_t bucketCount);
    HashTable(const HashTable& other);
    HashTable(HashTable&& other) noexcept;
    HashTable& operator=(const HashTable& other);
    HashTable& operator=(HashTable&& other) noexcept;
    ~HashTable() noexcept;


    void add(const Key& k, const Value& v);
    Value drop(const Key& k);
    bool has(const Key& k) const noexcept;
    void rehesh(size_t slots);
    bool empty() const noexcept;
    size_t size() const noexcept;
    size_t bucketCapacity() const noexcept;
    size_t bucketCount() const noexcept;
    size_t getBucketStart(const Key& k) const;
    bool findSLot(const Key& k, size_t& id) const;
    bool findFreeSlot(const Key& k, size_t& id) const; 

    HTIterator begin();
    HTIterator end();
    HTCIterator cbegin() const;
    HTCIterator cend() const;
  private:
    size_t bucketCapacity_;
    size_t bucketCount_;
    size_t otherflowBucketIndex_;
    size_t capacity_;
    size_t size_;
    Slot< Key, Value >* slots_;
    Hash hash_;
    Equal equal_;
  };
    
  template < class Key, class Value, class Hash, class Equal >
  class HashTable< Key, Value, Hash, Equal >::HTIterator
  {
    friend class HashTable< Key, Value, Hash, Equal >;
  public:
    HTIterator(HashTable< Key, Value, Hash, Equal >* ht, size_t ind, Slot< Key, Value >* current = nullptr);

    Slot< Key, Value >& operator*() noexcept;
    bool operator==(const HTIterator& other) const;
    bool operator!=(const HTIterator& other) const;
    
    HTIterator& operator++();
    HTIterator& operator++(int);
  private:
    HashTable< Key, Value, Hash, Equal >* ht_;
    size_t bucketIndex_;
    Slot< Key, Value >* current_;
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashTable< Key, Value, Hash, Equal >::HTCIterator
  {
    friend class HashTable< Key, Value, Hash, Equal >;
  public:
    HTCIterator(HashTable< Key, Value, Hash, Equal >* ht, size_t ind, Slot< Key, Value >* current = nullptr);

    Slot< Key, Value >& operator*() const noexcept;
    bool operator==(const HTCIterator& other) const;
    bool operator!=(const HTCIterator& other) const;
    
    HTCIterator& operator++();
    HTCIterator& operator++(int);
  private:
    HashTable< Key, Value, Hash, Equal >* ht_;
    size_t bucketIndex_;
    Slot< Key, Value >* current_;
  };
}

namespace muh = muhamadiarov;

template < class Key, class Value >
muh::Slot< Key, Value >::Slot(const Key& key, const Value& value):
  key_(key), 
  value_(value), 
  empty_(false) 
{}

template < class Key, class Value, class Hash, class Equal >
muh::HashTable< Key, Value, Hash, Equal >::HashTable(size_t bucketCapacity, size_t bucketCount):
  bucketCapacity_(bucketCapacity),
  bucketCount_(bucketCount),
  otherflowBucketIndex_(bucketCapacity * bucketCount),
  capacity_((bucketCount + 1) * bucketCapacity),
  size_(0)
{
  if (bucketCapacity == 0 || bucketCount == 0)
  {
    throw std::invalid_argument("Count of buckets and their capacity must be > 0");
  }
  slots_ = new Slot< Key, Value >[capacity_];
}

template < class Key, class Value, class Hash, class Equal >
muh::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& other):
  bucketCapacity_(other.bucketCapacity_),
  bucketCount_(other.bucketCount_),
  otherflowBucketIndex_(other.otherflowBucketIndex_),
  capacity_(other.capacity_),
  size_(other.size_),
  hash_(other.hash_),
  equal_(other.equal_)
{
  slots_ = new Slot< Key, Value >[capacity_];
  try
  {
    for (size_t i = 0; i < capacity_; ++i)
    {
      if (!other.slots_[i].empty_)
      {
        slots_[i] = other.slots_[i];
      }
    }
  }
  catch(...)
  {
    delete [] slots_;
    throw;
  }
}

template < class Key, class Value, class Hash, class Equal >
muh::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& other) noexcept:
  bucketCapacity_(other.bucketCapacity_),
  bucketCount_(other.bucketCount_),
  otherflowBucketIndex_(other.otherflowBucketIndex_),
  capacity_(other.capacity_),
  size_(other.size_),
  hash_(std::move(other.hash_)),
  equal_(std::move(other.equal_)),
  slots_(other.slots_)
{
  other.slots_ = nullptr;
  other.bucketCapacity_ = 0;
  other.bucketCount_ = 0;
  other.otherflowBucketIndex_ = 0;
  other.capacity_ = 0;
  other.size_ = 0;
}

template < class Key, class Value, class Hash, class Equal >
muh::HashTable< Key, Value, Hash, Equal >& muh::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& other)
{
  if (this != &other) {
    delete[] slots_;
      
    bucketCapacity_ = other.bucketCapacity_;
    bucketCount_ = other.bucketCount_;
    otherflowBucketIndex_ = other.otherflowBucketIndex_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    hash_ = other.hash_;
    equal_ = other.equal_;

    slots_ = new Slot<Key, Value>[capacity_];
    for (size_t i = 0; i < capacity_; ++i) {
      slots_[i] = other.slots_[i];
    }
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
muh::HashTable< Key, Value, Hash, Equal >& muh::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& other) noexcept
{
  if (this != &other) {
    delete[] slots_;
      
    bucketCapacity_ = other.bucketCapacity_;
    bucketCount_ = other.bucketCount_;
    otherflowBucketIndex_ = other.otherflowBucketIndex_;
    capacity_ = other.capacity_;
    size_ = other.size_;
    slots_ = other.slots_;
    hash_ = std::move(other.hash_);
    equal_ = std::move(other.equal_);
    
    other.slots_ = nullptr;
    other.bucketCapacity_ = 0;
    other.bucketCount_ = 0;
    other.otherflowBucketIndex_ = 0;
    other.capacity_ = 0;
    other.size_ = 0;
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
muh::HashTable< Key, Value, Hash, Equal >::~HashTable() noexcept
{
  delete [] slots_;
}

template < class Key, class Value, class Hash, class Equal >
size_t muh::HashTable< Key, Value, Hash, Equal >::getBucketStart(const Key& k) const
{
  return hash_(k) % capacity_ * bucketCapacity_;
}

template < class Key, class Value, class Hash, class Equal >
bool muh::HashTable< Key, Value, Hash, Equal >::findSLot(const Key& k, size_t& id) const
{
  size_t startBucket = getBucketStart(k);
  size_t endBucket = startBucket + bucketCapacity_;
  for (size_t i = startBucket; i < endBucket; ++i)
  {
    if (!slots_[i].empty && equal_(slots_[i].key_, k))
    {
      id = i;
      return true;
    }
  }

  size_t startOtherflow = otherflowBucketIndex_;
  size_t endOtherflow = startOtherflow + bucketCapacity_;
  for (size_t i = startOtherflow; i < endOtherflow; ++i)
  {
    if (!slots_[i].empty_ && equal(slots_[i].key_, k))
    {
      id = i;
      return true;
    }
  }

  return false;
}

template < class Key, class Value, class Hash, class Equal >
bool muh::HashTable< Key, Value, Hash, Equal >::findFreeSlot(const Key& k, size_t& id) const
{
  size_t startBucket = getBucketStart(k);
  size_t endBucket = startBucket + bucketCapacity_;
  for (size_t i = startBucket; i < endBucket; ++i)
  {
    if (slots_[i].empty_)
    {
      id = i;
      return true;
    }
  }

  size_t startOtherflow = otherflowBucketIndex_;
  size_t endOtherflow = startOtherflow + bucketCapacity_;
  for (size_t i = startBucket; i < endOtherflow; ++i)
  {
    if (slots_[i].empty_)
    {
      id = i;
      return true;
    }
  }

  return false;
}

template <class Key, class Value, class Hash, class Equal>
void muh::HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& v)
{
  size_t startBucket;
  bool result = findSLot(k, startBucket);
  if (result)
  {
    return;
  }
  result = findFreeSlot(k, startBucket);
  if (!result)
  {
    throw std::runtime_error("Hash-Table is otherflow: Not find free slot for data");
  }
  slots_[startBucket] = Slot< Key, Value >(k, v);
  ++size_;
}
  
#endif
