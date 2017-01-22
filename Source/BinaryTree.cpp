#include <utility> // For std::move()
#include "../Header/BinaryTree.h"

BinaryTree::BinaryTree()
    : top(nullptr)
{ }

BinaryTree::~BinaryTree()
{
    clear(top);
}

BinaryTree::BinaryTree(const BinaryTree& source)
    : top(nullptr)
{
    copy(source.top, top);
}

BinaryTree::BinaryTree(BinaryTree&& source)
    : top(source.top)
{
    source.top = nullptr;
}

BinaryTree& BinaryTree::operator=(const BinaryTree& rhs)
{
    if(this != &rhs)
    {
        clear(top);
        copy(rhs.top, top);
    }

    return *this;
}

BinaryTree& BinaryTree::operator=(BinaryTree&& rhs)
{
    if(this != &rhs)
    {
        clear(top);
        top = rhs.top;
        rhs.top = nullptr;
    }
    return *this;
}

BinaryTree::BinaryTree(const Tree_Node& src)
    : top(nullptr)
{
    copy(&src, top);
}

BinaryTree::BinaryTree(Tree_Node&& src)
    : top(nullptr)
{
    top = new Tree_Node(src.weight, src.letter, src.left, src.right);

    src.left = nullptr;
    src.right = nullptr;
}

BinaryTree::BinaryTree(size_t weight, char letter)
    : top(nullptr)
{
    top = new Tree_Node(weight, letter);
}

bool BinaryTree::search(BitVector& vect, size_t& pos, char what) const
{
    return search(vect, pos, what, top);
}

BinaryTree& BinaryTree::operator+=(const Tree_Node& rhs)
{
    Tree_Node* tmp = new Tree_Node(rhs);
    Tree_Node* new_top = new Tree_Node;

    new_top->left = top;
    new_top->right = tmp;

    if(top)
        new_top->weight += top->weight;

    new_top->weight += tmp->weight;

    top = new_top;

    return *this;
}

BinaryTree BinaryTree::operator+(const Tree_Node& rhs) const
{
    return BinaryTree(*this) += rhs;
}

BinaryTree& BinaryTree::operator+=(const BinaryTree& rhs)
{
    BinaryTree* right_node = new BinaryTree(rhs);
    Tree_Node* new_top = new Tree_Node;

    new_top->left = top;
    new_top->right = right_node->top;

    if(top)
        new_top->weight += top->weight;
    if(right_node->top)
        new_top->weight += top->weight;

    // Leaks a pointer (aka 4 bytes) into the memory if not done
    right_node->top = nullptr;
    delete right_node;
    //

    top = new_top;

    return *this;
}

BinaryTree BinaryTree::operator+(const BinaryTree& rhs) const
{
    return BinaryTree(*this) += rhs;
}

BinaryTree& BinaryTree::operator+=(BinaryTree&& rhs)
{
    Tree_Node* new_top = new Tree_Node;

    new_top->left = top;
    new_top->right = rhs.top;

    if(top)
        new_top->weight += top->weight;
    if(rhs.top)
        new_top->weight += rhs.top->weight;

    rhs.top = nullptr;
    top = new_top;

    return *this;
}

BinaryTree BinaryTree::operator+(BinaryTree&& rhs) const
{
    return BinaryTree(*this) += std::move(rhs);
}

bool BinaryTree::operator<(const BinaryTree& rhs) const
{
    if(!top && !rhs.top)
        return false;
    if(!top && rhs.top)
        return true;
    if(top && !rhs.top)
        return false;
    if(top->weight < rhs.top->weight)
        return true;

    return false;
}

bool BinaryTree::operator>(const BinaryTree& rhs) const
{
    if(!top && !rhs.top)
        return false;
    if(!top && rhs.top)
        return false;
    if(top && !rhs.top)
        return true;
    if(top->weight > rhs.top->weight)
        return true;

    return false;
}

bool BinaryTree::operator<=(const BinaryTree& rhs) const
{
    return !(*this > rhs);
}

bool BinaryTree::operator>=(const BinaryTree& rhs) const
{
    return !(*this < rhs);
}

bool BinaryTree::search(BitVector& vect, size_t& pos, char what, const Tree_Node* cur) const
{
    if(!cur)
        return false;

    if(leaf(cur))
        if(cur->letter == what)
            return true;
        else
            return false;

    size_t oldPos = pos;
    ++pos;

    if(search(vect, pos, what, cur->left))
    {
        vect.change(oldPos, 0);
        return true;
    }
    else if(search(vect, pos, what, cur->right))
    {
        vect.change(oldPos, 1);
        return true;
    }
    else
    {
        --pos;
        return false;
    }
}

void BinaryTree::copy(const Tree_Node* what, Tree_Node*& into)
{
    if(!what)
        return;

    into = new Tree_Node(what->weight, what->letter);
    copy(what->left, into->left);
    copy(what->right, into->right);
}

void BinaryTree::clear(Tree_Node* cur)
{
    if(!cur)
        return;

    clear(cur->left);
    clear(cur->right);
    delete cur;
}

bool BinaryTree::leaf(const Tree_Node * node) const
{
    return !node->left && !node->right;
}







