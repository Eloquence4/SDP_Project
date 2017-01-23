#include "../Header/Vector.h"

Vector::Vector()
    : data(nullptr)
    , cur_size(0)
    , max_size(0)
{ }

Vector::~Vector()
{
    delete[] data;
}

Vector::Vector(const Vector& src)
    : data(nullptr)
    , cur_size(0)
    , max_size(0)
{
    copy(src);
}

Vector::Vector(Vector&& src)
    : data(src.data)
    , cur_size(src.cur_size)
    , max_size(src.max_size)
{
    src.data = nullptr;
    src.cur_size = 0;
    src.max_size = 0;
}

Vector& Vector::operator=(const Vector& rhs)
{
    if(this != &rhs)
        copy(rhs);

    return *this;
}

Vector& Vector::operator=(Vector&& rhs)
{
    if(this != &rhs)
        assign(rhs);

    return *this;
}

bool Vector::full() const
{
    return cur_size == max_size;;
}

size_t Vector::size() const
{
    return cur_size;
}

size_t Vector::limit() const
{
    return max_size;
}

bool Vector::empty() const
{
    return cur_size == 0;
}

void Vector::resize(size_t new_size)
{
    if(new_size == 0)
    {
        delete[] data;
        cur_size = 0;
        max_size = 0;
        return;
    }

    if(new_size == cur_size)
        return;

    Tree_Node* new_data = new Tree_Node[new_size];
    for(int i = 0; i < new_size && i < cur_size; ++i)
        new_data[i] = data[i];

    delete[] data;
    data = new_data;

    if(cur_size > new_size)
        cur_size = new_size;

    max_size = new_size;
}

void Vector::reserve(size_t how_much)
{
    resize(max_size + how_much);
}

void Vector::shrink_to_fit()
{
    resize(cur_size);
}

Tree_Node& Vector::operator[](size_t index)
{
    if(index >= max_size)
        throw INVALID_INDEX;

    if(index >= cur_size)
        cur_size = index+1;

    return data[index];
}

const Tree_Node& Vector::operator[](size_t index) const
{
    if(index >= max_size)
        throw INVALID_INDEX;

    return data[index];
}

void Vector::push_back(const Tree_Node& new_data)
{
    if(max_size == 0)
        resize(16);
    else if(full())
        resize(max_size * 2);

    data[cur_size] = new_data;
    ++cur_size;
}

Tree_Node Vector::pop_back()
{
    if(empty())
        return;

    --cur_size;

    return data[cur_size+1];
}

void Vector::clear()
{
    delete[] data;

    data = nullptr;
    cur_size = 0;
    max_size = 0;
}

void Vector::copy(const Vector& src)
{
    Tree_Node* new_data = new Tree_Node[src.max_size];

    for(int i = 0; i < src.cur_size; ++i)
        new_data[i] = src.data[i];

    delete[] data;
    data = new_data;
    cur_size = src.cur_size;
    max_size = src.max_size;
}

void Vector::assign(Vector& src)
{
    delete[] data;
    data = src.data;
    cur_size = src.cur_size;
    max_size = src.max_size;

    src.data = nullptr;
    src.cur_size = 0;
    src.max_size = 0;
}











