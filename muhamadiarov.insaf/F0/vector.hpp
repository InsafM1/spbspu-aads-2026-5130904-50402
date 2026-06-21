#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <iostream>

namespace muhamadiarov
{
  template < class T >
  class Vector
  {
  public:
    Vector();
    Vector(const Vector& other);
    explicit Vector(size_t capacity);
    Vector(Vector&& other) noexcept;
    
    Vector& operator=(const Vector& other);
    Vector& operator=(Vector&& other) noexcept;
    
    T& operator[](size_t id) noexcept;
    const T& operator[](size_t id) const noexcept;
    
    ~Vector();
    
    void popBack();
    void pushBack(const T& value);
    void popFront();
    void pushFront(const T& value);

    T& at(size_t id);
    const T& at(size_t id) const;

    void reserve(size_t capacity);
    void erase(size_t id) noexcept;
    void clear() noexcept;

    bool isEmpty() const noexcept;
    size_t size() const noexcept;
    size_t capacity() const noexcept;
  private:
    T* data_;
    size_t size_;
    size_t capacity_;
  };
}

namespace muh = muhamadiarov;

template < class T >
muh::Vector< T >::Vector():
  data_(nullptr),
  size_(0),
  capacity_(0)
{}

template < class T >
muh::Vector< T >::Vector(const Vector& other):
  data_(new T[other.capacity_]),
  size_(other.size_),
  capacity_(other.capacity_)
{
  for (size_t i = 0; i < other.size_; ++i)
  {
    data_[i] = other[i];
  }
}

template < class T >
muh::Vector< T >::Vector(size_t capacity):
  data_(new T[capacity]),
  size_(0),
  capacity_(capacity)
{}

template < class T >
muh::Vector< T >::Vector(Vector&& other) noexcept:
  data_(other.data_),
  size_(other.size_),
  capacity_(other.capacity_)
{
  other.data_ = nullptr;
  other.size_ = 0;
  other.capacity_ = 0;
}

template < class T >
muh::Vector< T >& muh::Vector< T >::operator=(const Vector& other)
{
  if (this != &other)
  {
    Vector< T > copy(other);
    clear();
    std::swap(data_, copy.data_);
    std::swap(size_, copy.size_);
    std::swap(capacity_, copy.capacity_);
  }
  return *this;
}

template < class T >
muh::Vector< T >& muh::Vector< T >::operator=(Vector&& other) noexcept
{
  if (this != &other)
  {
    clear();
    delete[] data_;
    data_ = other.data_;
    size_ = other.size_;
    capacity_ = other.capacity_;
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }
  return *this;
}

template < class T >
T& muh::Vector< T >::operator[](size_t id) noexcept
{
  return data_[id];
}

template < class T >
const T& muh::Vector< T >::operator[](size_t id) const noexcept
{
  return data_[id];
}

template < class T >
muh::Vector< T >::~Vector()
{
  clear();
  delete[] data_;
}
#endif
