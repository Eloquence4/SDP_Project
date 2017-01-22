#include "../Header/PriorityQueue.h"

PriorityQueue::PriorityQueue()
    : data(nullptr)
    , start(0)
    , end(0)
    , max_size(0)
    , LastCommandWasPush(0)
{ }

PriorityQueue::~PriorityQueue()
{
    delete[] data;
}

PriorityQueue::PriorityQueue(const PriorityQueue& src)
    : data(nullptr)
    , start(src.start)
    , end(src.end)
    , max_size(src.max_size)
    , LastCommandWasPush(src.LastCommandWasPush)
{
    copy(src);
}

PriorityQueue::PriorityQueue(PriorityQueue&& src)
    : data(src.data)
    , start(src.start)
    , end(src.end)
    , max_size(src.max_size)
    , LastCommandWasPush(src.LastCommandWasPush)
{
    src.data = nullptr;
    src.start = 0;
    src.end = 0;
    src.max_size = 0;
    src.LastCommandWasPush = 0;
}

PriorityQueue& PriorityQueue::operator=(const PriorityQueue& rhs)
{
    if(this != &rhs)
        copy(rhs);
    return *this;
}

PriorityQueue& PriorityQueue::operator=(PriorityQueue&& rhs)
{
    assign(rhs);

    return *this;
}

void PriorityQueue::push(const BinaryTree& what)
{
    if(max_size == 0)
        resize(15);
    else if(full())
        resize(max_size * 2);

    size_t pos = start;

    while(data[pos] < what)
        pos++;

    if(pos != end)
        for(int j = end; j != pos;)
        {
            data[j] = std::move(data[j-1]);
            j = (j - 1) % max_size;
        }

    data[pos] = what;

    end = (end+1) % max_size;

    LastCommandWasPush = 1;
}

void PriorityQueue::push(BinaryTree&& what)
{
    if(max_size == 0)
        resize(15);
    else if(full())
        resize(max_size * 2);

    size_t pos = start;

    if(!empty())
        for(size_t i = start; i != end && data[pos] < what; i = (i+1) % max_size)
            pos++;

    if(pos != end)
        for(int j = end; j != pos;)
        {
            data[j] = std::move(data[j - 1]);
            j = (j - 1) % max_size;
        }

    data[pos] = std::move(what);

    end = (end + 1) % max_size;

    LastCommandWasPush = 1;
}

BinaryTree PriorityQueue::pop()
{
    if(empty())
        throw Queue_Is_Empty;

    size_t old_start = start;
    start = (start + 1) % max_size;

    LastCommandWasPush = 0;

    return data[old_start];
}

const BinaryTree& PriorityQueue::front() const
{
    if(empty())
        throw Queue_Is_Empty;

    return data[start];
}

BinaryTree PriorityQueue::front()
{
    if(empty())
        throw Queue_Is_Empty;

    return data[start];
}

bool PriorityQueue::empty() const
{
    return (max_size == 0) || (start == end && !LastCommandWasPush);
}

bool PriorityQueue::full() const
{
    return (max_size == 0) || (start == end && LastCommandWasPush);
}

size_t PriorityQueue::size() const
{
    if(full())
        return max_size;
    else if(empty())
        return 0;
    else if(start < end)
        return end - start;
    else
        return end - start + max_size;
}

void PriorityQueue::resize(size_t new_size)
{
    if(new_size == 0)
    {
        clear();
        return;
    }

    if(max_size == 0)
    {
        data = new BinaryTree[new_size];
        max_size = new_size;
        return;
    }

    if(max_size == new_size)
        return;
    
    BinaryTree* newData = new BinaryTree[new_size];

    size_t i = 0;
    size_t j = (start + 1) % max_size;

    if(!empty())
    {
        newData[0] = std::move(data[start]);
        i = 1;

        for(; j != end && i < new_size; i++)
        {
            newData[i] = std::move(data[j]);
            j = (j + 1) % max_size;
        }
    }

    delete[] data;
    start = 0;
    end = i % new_size;
    max_size = new_size;
}

void PriorityQueue::reserve(size_t how_much)
{
    resize(max_size + how_much);
}

void PriorityQueue::clear()
{
    delete[] data;
    start = 0;
    end = 0;
    max_size = 0;
    LastCommandWasPush = 0;
    data = nullptr;
}

void PriorityQueue::copy(const PriorityQueue& src)
{
    BinaryTree* new_data = new BinaryTree[src.max_size];

    for(size_t i = 0; i < max_size; i++)
        new_data[i] = src.data[i];

    delete[] data;

    data = new_data;
    start = src.start;
    end = src.end;
    max_size = src.max_size;
    LastCommandWasPush = src.LastCommandWasPush;
}

void PriorityQueue::assign(PriorityQueue& src)
{
    delete[] data;

    data = src.data;
    start = src.start;
    end = src.end;
    max_size = src.max_size;
    LastCommandWasPush = src.LastCommandWasPush;

    src.data = nullptr;
    src.clear();
}

