#include "CppUnitTest.h"
#include "../Header/BitVector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BitVectorTests
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(BitSetChange)
        {
            BitVector vect;

            vect.change(5, 1);
            vect.change(853, 1);
            vect.change(725465321, 1);

            Assert::IsTrue(vect.GetBit(5));
            Assert::IsTrue(vect.GetBit(853));
            Assert::IsTrue(vect.GetBit(725465321));

            vect.change(5, 0);
            vect.change(853, 0);
            vect.change(725465321, 0);

            Assert::IsFalse(vect.GetBit(5));
            Assert::IsFalse(vect.GetBit(853));
            Assert::IsFalse(vect.GetBit(725465321));

            vect.resize(2);

            Assert::IsFalse(vect.GetBit(5));
        }

    };
}