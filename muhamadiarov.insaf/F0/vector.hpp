#ifndef VECTOR_HPP
#define VECTOR_HPP

namespace muhamadiarov
{
  template < class T >
  class Vector
  {
  public:
    Vector();
    Vector(const Vector& other);
    Vector(size_t capacity);
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

#endif
