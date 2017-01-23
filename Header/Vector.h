#ifndef VectorDec
#define VectorDec

#include <cstddef>
#include "BinaryTree.h"

enum VectorErrors
{
    INVALID_INDEX
};

class Vector
{
public:
    Vector();
    ~Vector();
    Vector(const Vector& src);
    Vector(Vector&& src);
    Vector& operator=(const Vector& rhs);
    Vector& operator=(Vector&& rhs);

public:

    bool full() const;
    bool empty() const;

    size_t size() const;
    size_t limit() const;
    bool empty() const;

    void resize(size_t new_size);
    void reserve(size_t how_much);
    void shrink_to_fit();

    Tree_Node& operator[](size_t index);
    const Tree_Node& operator[](size_t index) const;

    void push_back(const Tree_Node& new_data);

    Tree_Node pop_back();

    void clear();

private:
    void copy(const Vector& src);
    void assign(Vector& src);

    Tree_Node* data;
    size_t cur_size;
    size_t max_size;
};

#endif
