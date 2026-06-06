#ifndef HASHER_HPP
#define HASHER_HPP

#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>

namespace muhamadiarov
{
  template<class T>
  class SipHash
  {
  public:
      SipHash();
      size_t operator()(const T& v) const;
      
  private:
      uint64_t key_[2] = {0x0706050403020100ULL, 0x0f0e0d0c0b0a0908ULL};
      mutable boost::hash2::siphash_24 h_;
  };
}

namespace muh = muhamadiarov;

template<class T>
muh::SipHash<T>::SipHash():
  h_(key_)
{}

template<class T>
size_t muh::SipHash<T>::operator()(const T& v) const
{
    boost::hash2::siphash_24 h(h_);
    boost::hash2::hash_append(h, boost::hash2::default_flavor{}, v);
    return boost::hash2::get_integral_result<size_t>(h);
}
#endif
