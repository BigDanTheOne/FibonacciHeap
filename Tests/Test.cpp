#include <gtest/gtest.h>
#include "BinaryHeap.h"
#include <gmock/gmock.h>

using testing::Eq;
namespace{
    class ClassDeclaration : public testing::Test{
    public:
        BinaryHeap<int> obj;
        ClassDeclaration(){
            obj;
        }
    };
}
TEST_F(ClassDeclaration, BasicTest){
    BinaryHeap<int> heap;
    ASSERT_EQ(true, heap.is_empty());
    ASSERT_ANY_THROW(heap.get_min());
    ASSERT_ANY_THROW(heap.extract_min());
    heap.insert(5);
    ASSERT_EQ(false, heap.is_empty());
    heap.insert(17);
    ASSERT_EQ(5, heap.get_min());
    ASSERT_EQ(5, heap.extract_min());
    ASSERT_EQ(17, heap.extract_min());
    ASSERT_EQ(true, heap.is_empty());
    heap.insert(5);
    heap.insert(5);
    heap.insert(10);
    heap.insert(17);
    ASSERT_EQ(5, heap.get_min());
    ASSERT_EQ(5, heap.extract_min());
    ASSERT_EQ(5, heap.extract_min());
    ASSERT_EQ(10, heap.extract_min());
    ASSERT_EQ(17, heap.extract_min());
    ASSERT_EQ(true, heap.is_empty());
}

TEST_F(ClassDeclaration, IPriorityQueueTest) {
    int* arr = new int[4]{1, 1, 2, 3};
    BinaryHeap<int> heap;;
    heap.IPriorityQueue(arr, arr + 4);
    ASSERT_EQ(1, heap.get_min());
    ASSERT_EQ(1, heap.extract_min());
    ASSERT_EQ(1, heap.extract_min());
    ASSERT_EQ(2, heap.extract_min());
    ASSERT_EQ(3, heap.extract_min());
    ASSERT_ANY_THROW(heap.get_min());
    ASSERT_ANY_THROW(heap.extract_min());
}
TEST_F(ClassDeclaration, heap_pointers){
    int* arr = new int[3]{1, 2, 3};
    BinaryHeap<int> heap;
    heap.IPriorityQueue(arr, arr + 3);
    BinaryHeap<int>::Pointer ptr0 = heap.insert(0);
    ASSERT_EQ(0, heap.get_min());
    heap.delet(ptr0);
    ASSERT_EQ(1, heap.get_min());
    BinaryHeap<int>::Pointer ptr1 = heap.insert(-10);
    ASSERT_EQ(-10, heap.get_min());
    heap.change(ptr1, 10);
    ASSERT_EQ(1, heap.get_min());
}
TEST_F(ClassDeclaration, optimization){
    BinaryHeap<int> heap;
    int* arr = new int[3]{1, 2, 3};
    heap.IPriorityQueue(arr, arr + 3);
    ASSERT_ANY_THROW(heap.optimize(5, 5));
    heap.extract_min();
    heap.extract_min();
    heap.extract_min();
    ASSERT_ANY_THROW(heap.optimize(-5, 5));
    ASSERT_NO_THROW(heap.optimize(5, 5));


}
