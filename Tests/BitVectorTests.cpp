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

        TEST_METHOD(BitSetFindPattern)
        {
            BitVector vect;

            vect.change(0, 1);
            vect.change(1, 1);
            vect.change(2, 0);
            vect.change(3, 0);
            vect.change(4, 1);

            char a[] = "11001";

            Assert::IsTrue(vect.findPattern(a, strlen(a)));

            vect.change(725465321, 1);
            vect.change(725465322, 0);
            vect.change(725465323, 0);
            vect.change(725465324, 0);
            vect.change(725465325, 1);

            a[1] = '0';

            // Takes 52 seconds to do find these 2 patters
            Assert::IsTrue(vect.findPattern(a, strlen(a)));
            vect.change(725465322, 1);

            Assert::IsFalse(vect.findPattern(a, strlen(a)));
        }
    };
}