#include <utility> // Has std::move
#include "../Header/BitVector.h"

BitVector::BitVector()
    : data(nullptr)
    , size(0)
{ }

BitVector::~BitVector()
{
    delete[] data;
}

BitVector::BitVector(const BitVector& source)
    : data(nullptr)
    , size(0)
{
    copy(source.data, source.size);
}

BitVector::BitVector(BitVector&& source)
    : data(nullptr)
    , size(0)
{
    assign(source.data, source.size);
}

BitVector& BitVector::operator=(const BitVector& rhs)
{
    if(this != &rhs)
        copy(rhs.data, rhs.size);
    return *this;
}

BitVector& BitVector::operator=(BitVector&& rhs)
{
    if(this != &rhs)
        assign(rhs.data, rhs.size);
    return *this;
}

void BitVector::change(size_t index, bool value)
{
    int pos = index % dataSize;
    index = index / dataSize;

    if(index >= size)
        resize(index+1);

    if(value)
        data[index] |= (unsigned long long)1 << (dataSize - pos - 1);
    else
        data[index] &= ~((unsigned long long)1 << (dataSize - pos - 1));
}

bool BitVector::empty() const
{
    return size == 0;
}

size_t BitVector::len() const
{
    return size * dataSize;
}

void BitVector::resize(size_t new_size)
{
    if(new_size == size)
        return;
    if(new_size == 0)
        clear();

    unsigned long long* tmp = new unsigned long long[new_size];

    if(new_size > size)
    {
        for(size_t i = 0; i < size; i++)
            tmp[i] = data[i];
        for(size_t i = size; i < new_size; i++)
            tmp[i] = 0;
    }
    else
        for(size_t i = 0; i < new_size; i++)
            tmp[i] = data[i];

    size = new_size;
    delete[] data;
    data = tmp;
}

void BitVector::reserve(size_t how_much)
{
    resize(size + how_much);
}

void BitVector::clear()
{
    delete[] data;
    size = 0;
}

bool BitVector::GetBit(size_t index)
{
    int pos = index % dataSize;
    index = index / dataSize;

    if(index >= size)
        throw INVALID_INDEX;

    return (data[index] >> (dataSize - pos - 1)) & 1;
}

void BitVector::remove(size_t index)
{
    if(index >= size)
        return;

    for(size_t i = index; i < size-1; ++i)
        data[i] = data[i+1];
    --size;
}

unsigned long long& BitVector::getBitSet(size_t index)
{
    if(index >= size)
        throw INVALID_INDEX;

    return data[index];
}

const unsigned long long& BitVector::getBitSet(size_t index) const
{
    if(index >= size)
        throw INVALID_INDEX;

    return data[index];
}

void BitVector::copy(unsigned long long* _data, size_t _size)
{
    unsigned long long* tmp = new unsigned long long[_size];

    for(size_t i = 0; i < _size; i++)
        tmp[i] = _data[i];

    if(size != 0)
        clear();

    data = tmp;
    size = _size;
}

void BitVector::assign(unsigned long long*& _data, size_t& _size)
{
    if(size != 0)
        clear();

    data = _data;
    size = _size;

    _data = nullptr;
    _size = 0;
}











