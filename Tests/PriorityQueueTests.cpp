#include "CppUnitTest.h"
#include "../Header/PriorityQueue.h"
#include "../Header/BinaryTree.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PriorityQueueTests
{
    TEST_CLASS(UnitTest1)
    {
    public:

        TEST_METHOD(PriorityQueuePushPopClear)
        {
            PriorityQueue q;

            q.push(BinaryTree(24, 'a'));
            q.push(BinaryTree(241, 'b'));
            q.push(BinaryTree(12, 'c'));
            q.push(BinaryTree(240, 'd'));
            q.push(BinaryTree(97432, 'f'));

            BinaryTree a = q.pop();
            BinaryTree b = q.pop();

            Assert::IsTrue(a < b);
            a = q.pop();
            Assert::IsTrue(b < a);
            b = q.pop();
            Assert::IsTrue(a < b);
            a = q.pop();
            Assert::IsTrue(b < a);

            Assert::IsTrue(q.empty());

            q.push(BinaryTree(24, 'a'));
            q.push(BinaryTree(241, 'b'));
            q.push(BinaryTree(12, 'c'));
            q.push(BinaryTree(240, 'd'));
            q.push(BinaryTree(97432, 'f'));

            q.clear();

            Assert::IsTrue(q.empty());
            Assert::IsTrue(q.full());
        }

    };
}