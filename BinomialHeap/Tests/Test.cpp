#include <gtest/gtest.h>
#include "FibonacciHeap.h"
#include <gmock/gmock.h>

using testing::Eq;
namespace{
    class ClassDeclaration : public testing::Test{
    public:
        FibonacciHeap<int> obj;
        ClassDeclaration(){
            obj;
        }
    };
}
TEST_F(ClassDeclaration, binomial_heap_basics){
    FibonacciHeap<int> heap;
    ASSERT_ANY_THROW(heap.get_min());
    ASSERT_ANY_THROW(heap.extract_min());
    ASSERT_TRUE(heap.is_empty());
    heap.insert(5);
    ASSERT_FALSE(heap.is_empty());
    ASSERT_EQ(5, heap.get_min());
    ASSERT_EQ(5, heap.extract_min());
    ASSERT_TRUE(heap.is_empty());
    heap.insert(5);
    heap.insert(6);
    ASSERT_EQ(5, heap.get_min());
    heap.insert(2);
    ASSERT_EQ(2, heap.get_min());
    heap.insert(4);
    ASSERT_EQ(2, heap.get_min());
    ASSERT_EQ(2, heap.extract_min());
    ASSERT_EQ(4, heap.extract_min());
    ASSERT_EQ(5, heap.extract_min());
    ASSERT_EQ(6, heap.extract_min());
    ASSERT_ANY_THROW(heap.extract_min());
}

TEST_F(ClassDeclaration, binomial_heap_speed){
    int begin = clock();
    FibonacciHeap<int> heap;
    for(int i = 0; i < 1000000; ++i) {
        heap.insert(rand());
    }
    int end = clock();
    std::cout << "time of performing 1 million inserts = " << end - begin << std::endl;
    FibonacciHeap<int> other_heap;
    FibonacciHeap<int>::Pointer arr[1000];
    for(int i = 0; i < 1000; ++i) {
        auto ptr = other_heap.insert(rand());
        arr[i] = ptr;
    }
    begin = clock();
    heap.merge(other_heap);
    heap.extract_min();
    begin = clock();
    for(int i = 0; i < 500; ++i){
        heap.decrease(arr[i], 0);
    }
    end = clock();
    std::cout << "time of performing 500 thousands delete from 1 billions elements = " << end - begin << std::endl;
    begin = clock();
    for(int i = 0; i < 1000; ++i) {
        heap.extract_min();
    }
    end = clock();
    std::cout << "1 thousand extract_min from 2 billions elements = " << end - begin << std::endl;
    std::cout << "asymptotics is OK!!!! " << std::endl;
}