#ifndef PriorityQueueDec
#define PriorityQueueDec

#include <cstddef>
#include <utility> // For std::move()
#include "BinaryTree.h"

enum PRIORITY_QUEUE_ERROS
{
    QUEUE_IS_EMPTY
};

class PriorityQueue
{
public:
    // O(1)
    PriorityQueue();
    // O(1)
    ~PriorityQueue();
    // O(n)
    PriorityQueue(const PriorityQueue& src);
    // O(1)
    PriorityQueue(PriorityQueue&& src);
    // O(n)
    PriorityQueue& operator=(const PriorityQueue& rhs);
    // O(1)
    PriorityQueue& operator=(PriorityQueue&& rhs);

public:
    // O(n) or O(2n)
    // Adds the element to the queue
    // Becomes O(2n) if it has to resize to a bigger queue
    void push(const BinaryTree& what);
    // O(1) or O(n)
    // Adds the element to the queue
    // Becomes O(n) if it has to resize to a bigger queue
    void push(BinaryTree&& what);

    // O(1)
    // Removes the first element in the queue
    // Can throw QUEUE_IS_EMPTY
    BinaryTree pop();
    // O(1)
    // Returns a reference to the first element
    // Can throw QUEUE_IS_EMPTY
    const BinaryTree& front() const;
    // O(n)
    // Returns a copy of the first element
    // Can throw QUEUE_IS_EMPTY
    BinaryTree front();

    // O(1)
    // Check if the queue is empty
    bool empty() const;
    // O(1)
    // Check if the queue is full
    bool full() const;
    // O(1)
    // Returns the size of the queue
    size_t size() const;

    // O(n)
    // Resizes the queue
    void resize(size_t new_size);
    // O(n)
    // Resizes the queue to the current size + how_much
    void reserve(size_t how_much);

    // O(n^2)
    // Deletes the elements of the queue
    void clear();

private:

    void copy(const PriorityQueue& src);
    void assign(PriorityQueue& src);

    BinaryTree* data;
    size_t start;
    size_t end;
    size_t max_size           : 31;
    size_t LastCommandWasPush : 1;

};

#endif
