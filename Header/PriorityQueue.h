#ifndef PriorityQueueDec
#define PriorityQueueDec

#include <cstddef>
#include <utility> // For std::move()
#include "BinaryTree.h"

enum PRIORITY_QUEUE_ERROS
{
    Queue_Is_Empty
};

class PriorityQueue
{
public:
    PriorityQueue();
    ~PriorityQueue();
    PriorityQueue(const PriorityQueue& src);
    PriorityQueue(PriorityQueue&& src);
    PriorityQueue& operator=(const PriorityQueue& rhs);
    PriorityQueue& operator=(PriorityQueue&& rhs);

public:

    void push(const BinaryTree& what);
    void push(BinaryTree&& what);

    BinaryTree pop();
    const BinaryTree& front() const;
    BinaryTree front();

    bool empty() const;
    bool full() const;
    size_t size() const;

    void resize(size_t new_size);
    void reserve(size_t how_much);

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
