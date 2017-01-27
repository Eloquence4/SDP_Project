#include "CppUnitTest.h"
#include "../Header/BinaryTree.h"
#include "../Header/BitVector.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace BinaryTreeTests
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(BinaryTreeSearch)
        {
            BinaryTree test(1, 'a');

            test += BinaryTree(2, 'b');
            test += BinaryTree(3, 'c');
            test += BinaryTree(4, 'j');
            test += BinaryTree(5, 'f');

            BitVector vect;
            size_t pos = 0;

            Assert::IsTrue(test.writeBits(vect, pos, 'c'));
            Assert::IsTrue(test.writeBits(vect, pos, 'j'));
            Assert::IsTrue(test.writeBits(vect, pos, 'b'));
            Assert::IsFalse(test.writeBits(vect, pos, 'k'));
            Assert::IsFalse(test.writeBits(vect, pos, 'i'));

            Assert::IsFalse(vect.GetBit(0));
            Assert::IsFalse(vect.GetBit(1));
            Assert::IsTrue(vect.GetBit(2));
            Assert::IsFalse(vect.GetBit(3));
            Assert::IsTrue(vect.GetBit(4));
            Assert::IsFalse(vect.GetBit(5));
            Assert::IsFalse(vect.GetBit(6));
            Assert::IsFalse(vect.GetBit(7));
            Assert::IsTrue(vect.GetBit(8));
            Assert::IsFalse(vect.GetBit(9));
            Assert::IsFalse(vect.GetBit(10));
            Assert::IsFalse(vect.GetBit(11));
            Assert::IsFalse(vect.GetBit(12));
            Assert::IsFalse(vect.GetBit(13));
            Assert::IsFalse(vect.GetBit(14));
            Assert::IsFalse(vect.GetBit(15));
            Assert::IsFalse(vect.GetBit(16));
            Assert::IsFalse(vect.GetBit(17));
            Assert::IsFalse(vect.GetBit(18));
        }

        TEST_METHOD(BinaryTreeOperators)
        {
            BinaryTree a(8, 'k');
            BinaryTree b(2, 'j');
            BinaryTree c = a + b;

            Assert::IsTrue(a > b);
            Assert::IsFalse(a < b);
            Assert::IsTrue(c > a);
            Assert::IsFalse(c < b);
        }

        TEST_METHOD(BinaryTreeHeight)
        {
            BinaryTree test(1, 'a');

            test += BinaryTree(2, 'b');
            test += BinaryTree(3, 'c');
            test += BinaryTree(3, 'f');
            test += BinaryTree(8, 'j');
            test += test;

            Assert::AreEqual(test.height(), 5);

            BinaryTree test2(1, 'a');

            test2 += test;
            test2 += BinaryTree(3, 'c');
            test2 += BinaryTree(3, 'f');
            test2 += BinaryTree(8, 'j');
            test2 += test2;

            Assert::AreEqual(test2.height(), 10);
        }
    };
}