#ifndef BinaryTreeDec
#define BinaryTreeDec

#include <cstddef>
#include <fstream>
#include "BitVector.h"

// Not the usual Binary Tree, it's one used for the Huffman algorithm

struct Tree_Node
{
    char letter;
    // 3 bytes of padding here that cannot be avoided anyway
    size_t weight; // Required for the Huffman algorithm

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
    // O(1)
    BinaryTree();
    // O(n)
    ~BinaryTree();
    // O(n)
    BinaryTree(const BinaryTree& source);
    // O(1)
    BinaryTree(BinaryTree&& source);
    // O(n)
    BinaryTree& operator=(const BinaryTree& rhs);
    // O(1)
    BinaryTree& operator=(BinaryTree&& rhs);

public:
    // O(n)
    // Constructs a tree with the top being the given node
    BinaryTree(const Tree_Node& src);
    // O(1)
    // Constructs a tree with the top being the given node
    BinaryTree(Tree_Node&& src);

    // O(1)
    // Constructs a tree with the top node's weight and letter defined
    BinaryTree(size_t weight, char letter);

    // O(n)
    // Looks at where the character 'what' is in the tree and writes
    // the bit code for that character in the BitVector vect on position 'pos'
    // Returns true if successful, otherwise it returns false and returns pos to where it was
    bool writeBits(BitVector& vect, size_t& pos, char what) const;
    // O(n)
    // According to the way the tree is defined, it looks at the bits in the bit vector from
    // position 'pos' onwards and returns the character in that position
    char extractBits(const BitVector& vect, size_t& pos) const;

    // Less than O(n), usually
    // Returns the height of a letter
    int height(char ofWhat) const;
    // O(n)
    // Returns the height of the tree
    int height() const;
    // O(1)
    // Returns the weight of the topmost node
    size_t weight() const;

    // O(n)
    // Summates the two binary trees according to the way the Huffman Tree works
    BinaryTree& operator+=(const Tree_Node& rhs);
    // O(2n)
    // Summates the two binary trees according to the way the Huffman Tree works
    BinaryTree  operator+ (const Tree_Node& rhs) const;
    // O(n)
    // Summates the two binary trees according to the way the Huffman Tree works
    BinaryTree& operator+=(const BinaryTree& rhs);
    // O(2n)
    // Summates the two binary trees according to the way the Huffman Tree works
    BinaryTree  operator+ (const BinaryTree& rhs) const;

    // O(1)
    // Summates the two binary trees according to the way the Huffman Tree works
    BinaryTree& operator+=(BinaryTree&& rhs);
    // O(n)
    // Summates the two binary trees according to the way the Huffman Tree works
    BinaryTree  operator+ (BinaryTree&& rhs) const;

    // O(1)
    // The tree with the smaller weight on the top node is smaller
    bool operator< (const BinaryTree& rhs) const;
    // O(1)
    // The tree with the smaller weight on the top node is smaller
    bool operator> (const BinaryTree& rhs) const;
    // O(1)
    // The tree with the smaller weight on the top node is smaller
    bool operator<=(const BinaryTree& rhs) const;
    // O(1)
    // The tree with the smaller weight on the top node is smaller
    bool operator>=(const BinaryTree& rhs) const;

    // O(n)
    // Exports the tree to a binary file
    void BinaryExport(std::fstream& file) const;
    // O(n)
    // Imports data from a binary file
    // Clears all of the current data
    void BinaryImport(std::fstream& file);

    // Less than O(n), usually
    // holder must be empty, or this function will leak
    // Writes down where the character 'target' is in the tree into the 'holder' character array
    // Returns true if successful, otherwise returns false and sets holder to NULL
    bool CreatePattern(char*& holder, char target) const;

private:

    int height(char ofWhat, const Tree_Node* cur) const;
    int height(const Tree_Node* cur) const;

    bool CreatePattern(char* holder, char target, const Tree_Node* cur, size_t len, size_t counter) const;

    char extractBits(const BitVector& vect, size_t& pos, const Tree_Node* cur) const;

    void BinaryExport(std::fstream& file, const Tree_Node* cur) const;
    void BinaryImport(std::fstream& file, Tree_Node*& cur);

    bool writeBits(BitVector& vect, size_t& pos, char what, const Tree_Node* cur) const;
    void copy(const Tree_Node* what, Tree_Node*& into);
    void clear(Tree_Node* cur);

    bool leaf(const Tree_Node* node) const;

    Tree_Node* top;
};

#endif // BinaryTreeDec
