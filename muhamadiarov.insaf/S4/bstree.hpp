#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <utility>
#include <functional>
#include <cstddef>

namespace muhamadiarov
{
  template < class Key, class Value >
  struct TreeNode
  {
    std::pair< Key, Value > val_;
    TreeNode* left_;
    TreeNode* right_;
    TreeNode* parent_;

    TreeNode();
    TreeNode(const Key& key, const Value& value, TreeNode* parent);
    TreeNode(Key&& key, Value&& value, TreeNode* parent);
  };
  template < class Key, class Value >
  class BSIterator;
  template < class Key, class Value >
  class BSConstIterator;
  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
    using Iter_t = BSIterator< Key, Value >;
    using ConstIter_t = BSConstIterator< Key, Value >;
    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    ~BSTree();
    
    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    void push(const Key& k, const Value& v);
    Value& get(Key k);
    const Value& get(Key k) const;
    void drop(Key k);
    bool has(const Key& k) const noexcept;
    
    ConstIter_t rotateLeft(ConstIter_t it);
    ConstIter_t rotateRight(ConstIter_t it);

    ConstIter_t rotateLargeLeft(ConstIter_t it);
    ConstIter_t rotateLargeRight(ConstIter_t it);

    size_t height(ConstIter_t it) const;
    size_t height() const;
    
    size_t size() const noexcept;
    bool empty() const noexcept;

    void swap(BSTree& other) noexcept;

    void clear();

    Iter_t begin();
    Iter_t end() noexcept;
    ConstIter_t begin() const noexcept;
    ConstIter_t end() const noexcept;
    ConstIter_t cbegin() const;
    ConstIter_t cend() const noexcept;

  private:
    TreeNode< Key, Value >* root;
    size_t size_;
    Compare comp_;
  };
}

namespace muh = muhamadiarov;

template < class Key, class Value >
muh::TreeNode< Key, Value >::TreeNode():
  val_({Key(), Value()}),
  left_(nullptr),
  right_(nullptr),
  parent_(nullptr)
{}

template < class Key, class Value >
muh::TreeNode< Key, Value >::TreeNode(const Key& key, const Value& value, TreeNode* parent):
  val_({key, value}),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

template < class Key, class Value >
muh::TreeNode< Key, Value >::TreeNode(Key&& key, Value&& value, TreeNode* parent):
  val_({std::move(key), std::move(value)}),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}
#endif
