#ifndef BinaryTreeDec
#define BinaryTreeDec

#include <cstddef>
#include "BitVector.h"

struct Tree_Node
{
    size_t weight; // Required for the Huffman algorithm
    char letter;

    Tree_Node* left;
    Tree_Node* right;

    Tree_Node(size_t _weight = 0, char _letter = '\0', Tree_Node* _left = nullptr, Tree_Node* _right = nullptr)
        : weight(_weight)
        , letter(_letter)
        , left(_left)
        , right(_right)
    { }
};

class BinaryTree
{
public:
    BinaryTree();
    ~BinaryTree();
    BinaryTree(const BinaryTree& source);
    BinaryTree(BinaryTree&& source);
    BinaryTree& operator=(const BinaryTree& rhs);
    BinaryTree& operator=(BinaryTree&& rhs);

public:
    BinaryTree(const Tree_Node& src);
    BinaryTree(Tree_Node&& src);

    BinaryTree(size_t weight, char letter);

    bool search(BitVector& vect, size_t& pos, char what) const;

    BinaryTree& operator+=(const Tree_Node& rhs);
    BinaryTree  operator+ (const Tree_Node& rhs) const;
    BinaryTree& operator+=(const BinaryTree& rhs);
    BinaryTree  operator+ (const BinaryTree& rhs) const;

    BinaryTree& operator+=(BinaryTree&& rhs);
    BinaryTree  operator+ (BinaryTree&& rhs) const;

    bool operator< (const BinaryTree& rhs) const;
    bool operator> (const BinaryTree& rhs) const;
    bool operator<=(const BinaryTree& rhs) const;
    bool operator>=(const BinaryTree& rhs) const;

private:

    bool search(BitVector& vect, size_t& pos, char what, const Tree_Node* cur) const;
    void copy(const Tree_Node* what, Tree_Node*& into);
    void clear(Tree_Node* cur);

    bool leaf(const Tree_Node* node) const;

    Tree_Node* top;
};

#endif // BinaryTreeDec
