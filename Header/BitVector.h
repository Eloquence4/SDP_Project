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
    BitVector();
    ~BitVector();
    BitVector(const BitVector& source);
    BitVector(BitVector&& source);
    BitVector& operator=(const BitVector& rhs);
    BitVector& operator=(BitVector&& rhs);

public:
    void change(size_t index, bool value); // Value = 0 or 1

    bool empty() const;
    size_t len() const;
    size_t bitSetSize() const;

    void resize(size_t new_size);
    void reserve(size_t how_much);

    void clear();

    bool GetBit(size_t index) const;

    void readSet(std::fstream& binaryFile, size_t startIndex, size_t endIndex);

    void remove(size_t index);
    unsigned long long& getBitSet(size_t index = 0);
    const unsigned long long& getBitSet(size_t index) const;

    bool findPattern(const char* pattern, size_t patternLen);

private:

    void copy(unsigned long long* _data, size_t _size);
    void assign(unsigned long long*& _data, size_t& _size);

    unsigned long long* data;
    size_t size;
};

#endif // BitVectorDef
