#ifndef BitVectorDef
#define BitVectorDef

#include <fstream>

enum BIT_VECTOR_ERRORS
{
    INVALID_BIT_INDEX
};

class BitVector
{
public:
    static const short dataSize = 64;

public:
    // O(1)
    BitVector();
    // O(1)
    ~BitVector();
    // O(n)
    BitVector(const BitVector& source);
    // O(1)
    BitVector(BitVector&& source);
    // O(n)
    BitVector& operator=(const BitVector& rhs);
    // O(1)
    BitVector& operator=(BitVector&& rhs);

public:
    // O(1)
    // Changes the value of the bit on position index to a 0 or 1
    void change(size_t index, bool value);

    // O(1)
    // Returns true if the vector has no data in it
    bool empty() const;
    // O(1)
    // Returns the length of the bit vector
    size_t len() const;
    // O(1)
    // Returns the number of elements in the array
    // In other works the length / 64
    size_t bitSetSize() const;

    // O(n)
    // Resizes the vector to a new size, new_size must be greater than 0
    void resize(size_t new_size);
    // O(n)
    // Resizes to the current size + how_much
    void reserve(size_t how_much);

    // O(1)
    // Deletes all the data that the vector holds
    void clear();

    // O(1)
    // Returns the value of the bit on position index
    // Can throw INVALID_BIT_INDEX
    bool GetBit(size_t index) const;

    // O(endIndex - startIndex + 1)
    // Reads a bit set from a binary file, starts reading from StartIndex and finishes at EndIndex
    void readSet(std::fstream& binaryFile, size_t startIndex, size_t endIndex);

    // O(n - index)
    // Removes the bit set on position 'index' and copies all of the previous ones back once
    void remove(size_t index);
    // O(1)
    // Returns the bit set on the index
    // Can throw INVALID_BIT_INDEX
    unsigned long long& getBitSet(size_t index = 0);
    // O(1)
    // Returns the bit set on the index
    // Can throw INVALID_BIT_INDEX
    unsigned long long getBitSet(size_t index) const;

    // O(n)
    // Finds a pattern of characters in the bit vector
    // Pattern must be a string of the type "100101" or something simillar
    bool findPattern(const char* pattern, size_t patternLen);

private:

    void copy(unsigned long long* _data, size_t _size);
    void assign(unsigned long long*& _data, size_t& _size);

    unsigned long long* data;
    size_t size;
};

#endif // BitVectorDef
