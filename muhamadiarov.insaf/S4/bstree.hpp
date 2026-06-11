#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <utility>
#include <functional>
#include <cstddef>
#include <stdexcept>

namespace muhamadiarov
{
  template< class Key, class Value >
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
  template< class Key, class Value >
  class BSIterator;
  template< class Key, class Value >
  class BSConstIterator;
  template< class Key, class Value, class Compare = std::less< Key > >
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
    TreeNode< Key, Value >* root_;
    size_t size_;
    Compare comp_;
    void copyTree(TreeNode< Key, Value >* source, TreeNode< Key, Value >* parent);
  };

  template< class Key, class Value >
  class BSIterator
  {
  public:
    BSIterator();
    explicit BSIterator(TreeNode< Key, Value >* node);
    BSIterator(const BSIterator&) = default;
    BSIterator& operator=(const BSIterator&) = default;

    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();

    BSIterator& operator++();
    BSIterator& operator++(int);
    
    BSIterator& operator--();
    BSIterator& operator--(int);

    bool operator==(const BSIterator& other) const;
    bool operator!=(const BSIterator& other) const;
  private:
    TreeNode< Key, Value >* curr_;
    friend class BSTree< Key, Value, std::less< Key > >;

    static TreeNode< Key, Value >* getNext(TreeNode< Key, Value >* node);
    static TreeNode< Key, Value >* getPrevious(TreeNode< Key, Value >* node);
  };

  template< class Key, class Value >
  class BSConstIterator
  {
  public:
    BSConstIterator();
    explicit BSConstIterator(TreeNode< Key, Value >* node);
    BSConstIterator(const BSConstIterator&) = default;
    BSConstIterator& operator=(const BSConstIterator&) = default;

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;

    BSConstIterator& operator++();
    BSConstIterator& operator++(int);
    
    BSConstIterator& operator--();
    BSConstIterator& operator--(int);

    bool operator==(const BSConstIterator& other) const;
    bool operator!=(const BSConstIterator& other) const;
  private:
    const TreeNode< Key, Value >* curr_;
    friend class BSTree< Key, Value, std::less< Key > >;

    static const TreeNode< Key, Value >* getNext(const TreeNode< Key, Value >* node);
    static const TreeNode< Key, Value >* getPrevious(const TreeNode< Key, Value >* node);
  };
}

namespace muh = muhamadiarov;

template< class Key, class Value >
muh::TreeNode< Key, Value >::TreeNode():
  val_({Key(), Value()}),
  left_(nullptr),
  right_(nullptr),
  parent_(nullptr)
{}

template< class Key, class Value >
muh::TreeNode< Key, Value >::TreeNode(const Key& key, const Value& value, TreeNode* parent):
  val_({key, value}),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

template< class Key, class Value >
muh::TreeNode< Key, Value >::TreeNode(Key&& key, Value&& value, TreeNode* parent):
  val_({std::move(key), std::move(value)}),
  left_(nullptr),
  right_(nullptr),
  parent_(parent)
{}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >::BSTree():
  root_(new TreeNode< Key, Value >()),
  size_(0),
  comp_()
{}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  root_(new TreeNode< Key, Value >()),
  size_(0),
  comp_(other.comp_)
{
  if (!other.empty())
  {
    copyTree(other.root_->left_, nullptr);
  }
}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  root_(other.root_),
  size_(other.size_),
  comp_(std::move(other.comp_))
{
  other.root_ = nullptr;
  other.size_ = 0;
}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
  delete root_;
}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >& muh::BSTree< Key, Value, Compare >::operator=(
  const BSTree& other
)
{
  if (this != &other)
  {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
muh::BSTree< Key, Value, Compare >& muh::BSTree< Key, Value, Compare >::operator=(
  BSTree&& other
) noexcept
{
  if (this != &other)
  {
    clear();
    delete root_;
    root_ = other.root_;
    size_ = other.size_;
    comp_ = std::move(other.comp_);
    other.root_ = nullptr;
    other.size_ = 0;
  }
  return *this;
}

template< class Key, class Value, class Compare >
void muh::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
}

template<class Key, class Value, class Compare>
void muh::BSTree<Key, Value, Compare>::copyTree(
  TreeNode< Key, Value >* source,
  TreeNode< Key, Value >* parent
)
{
  if (!source)
  {
    return;
  }

  TreeNode<Key, Value>* newNode = new TreeNode<Key, Value>(
    source->val_.first,
    source->val_.second,
    parent
  );
  
  if (!parent)
  {
    root_->left_ = newNode;
  } 
  else if (comp_(source->val_.first, parent->val_.first))
  {
    parent->left_ = newNode;
  }
  else
  {
    parent->right_ = newNode;
  }
  
  copyTree(source->left_, newNode);
  copyTree(source->right_, newNode);
  ++size_;
}

template< class Key, class Value >
muh::BSIterator< Key, Value >::BSIterator():
  curr_(nullptr)
{}

template< class Key, class Value >
muh::BSIterator< Key, Value >::BSIterator(TreeNode< Key, Value >* node):
  curr_(node)
{}

template< class Key, class Value >
std::pair< Key, Value >& muh::BSIterator< Key, Value >::operator*()
{
  if (!curr_)
  {
    throw std::runtime_error("Dereferencing nullptr");
  }
  return curr_->val_;
}

template< class Key, class Value >
std::pair< Key, Value >* muh::BSIterator< Key, Value >::operator->()
{
  if (!curr_)
  {
    throw std::runtime_error("Dereferencing nullptr");
  }
  return &(curr_->val_);
}

template< class Key, class Value >
muh::BSIterator< Key, Value >& muh::BSIterator< Key, Value >::operator++()
{
  curr_ = getNext(curr_);
  return *this;
}

template< class Key, class Value >
muh::BSIterator< Key, Value >& muh::BSIterator< Key, Value >::operator++(int)
{
  BSIterator temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
muh::BSIterator< Key, Value >& muh::BSIterator< Key, Value >::operator--()
{
  curr_ = getPrevious(curr_);
  return *this;
}

template< class Key, class Value >
muh::BSIterator< Key, Value >& muh::BSIterator< Key, Value >::operator--(int)
{
  BSIterator temp(*this);
  --(*this);
  return temp;
}

template< class Key, class Value >
bool muh::BSIterator< Key, Value >::operator==(const BSIterator< Key, Value >& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool muh::BSIterator< Key, Value >::operator!=(const BSIterator< Key, Value >& other) const
{
  return !(*this == other);
}

template < class Key, class Value >
muh::TreeNode< Key, Value >* muh::BSIterator< Key, Value >::getNext(
  TreeNode< Key, Value >* node
)
{
  if (!node)
  {
    return nullptr;
  }

  if (node->right_)
  {
    node = node->right_;
    while (node->left_)
    {
      node = node->left_;
    }
    return node;
  }

  const TreeNode< Key, Value >* parent = node->parent_;
  while (parent && node == parent->right_)
  {
    node = parent;
    parent = parent->parent_;
  }
  return parent;
}

template< class Key, class Value >
muh::TreeNode< Key, Value >* muh::BSIterator< Key, Value >::getPrevious(
  TreeNode< Key, Value >* node
)
{
  if (!node)
  {
    return nullptr;
  }

  if (node->left)
  {
    node = node->left;
    while (node->right_)
    {
      node = node->right_;
    }
    return node;
  }

  const TreeNode< Key, Value >* parent = node->parent_;
  while (parent && node == parent->left_)
  {
    node = parent;
    parent = parent->parent_;
  }
  return parent;
}
#endif
