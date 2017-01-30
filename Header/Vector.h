#ifndef VectorDec
#define VectorDec

#include <cstddef>
#include "BinaryTree.h"

enum VECTOR_ERRORS
{
    INVALID_VECTOR_INDEX,
    NOTHING_TO_POP
};

class Vector
{
public:
    // O(1)
    Vector();
    // O(1)
    ~Vector();
    // O(n)
    Vector(const Vector& src);
    // O(1)
    Vector(Vector&& src);
    // O(n)
    Vector& operator=(const Vector& rhs);
    // O(1)
    Vector& operator=(Vector&& rhs);

public:
    // O(1)
    // Checks if the vector is full
    bool full() const;
    // O(1)
    // Checks if the vector is empty
    bool empty() const;

    // O(1)
    // Returns the number of elements in the vector
    size_t size() const;
    // O(1)
    // Returns the maximum number of elements before it has to resize;
    size_t limit() const;

    // O(n)
    // Resizes the vector
    void resize(size_t new_size);
    // O(n)
    // Resizes the vector to the current size + how_much
    void reserve(size_t how_much);
    // O(n)
    // Shrinks the vector so it consumes the least amount of space
    void shrink_to_fit();

    // O(1)
    // Returns a reference to the element on position index
    Tree_Node& operator[](size_t index);
    // O(1)
    // Returns a reference to the element on position index
    const Tree_Node& operator[](size_t index) const;

    // O(1) or O(n) if it has to resize
    // Adds an element to the end of the vector
    void push_back(const Tree_Node& new_data);

    // O(1)
    // Returns a copy of the last element in the vector
    Tree_Node pop_back();

    // O(1)
    // Deletes the data in the vector
    void clear();

private:
    void copy(const Vector& src);
    void assign(Vector& src);

    Tree_Node* data;
    size_t cur_size;
    size_t max_size;
};

#endif // VectorDec
