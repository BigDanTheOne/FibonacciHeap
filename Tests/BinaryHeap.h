//
// Created by akopi on 18-Nov-18.
//

#ifndef UNTITLED6_BINARYHEAP_H
#define UNTITLED6_BINARYHEAP_H

#include<cmath>
#include<gtest/gtest.h>
#include <iostream>
#include<set>
#include<ctime>
#include<memory>
#include<vector>
#include<cmath>
template <typename Key>
class Array {
public:
    std::pair<Key, int> * arr;
    size_t array_size;
    Array() {
        arr = new std::pair<Key, int>[10];
        array_size = 10;
    }
    ~Array() {
        //delete[] arr;
    }
    void ScaleUpArray();
    void ScaleDownArray();
};
double f(int k, size_t & insertCount, size_t & extractCount) {
    int n = insertCount + extractCount;
    return insertCount * std::log(n) / std::log(k) + extractCount * k * std::log(n) / std::log(k);
}
int FindK(size_t insertCount, size_t extractCount) {
    double min = f(2,  insertCount,  extractCount);
    int k = 2;
    for (int i = 2; i < insertCount / 2; ++i) {
        if (f(2, insertCount, extractCount) < min) {
            min = f(2, insertCount, extractCount);
            k = i;
        }
    }
    return k;
}
template <typename Key>
class BinaryHeap {
public:
    class Pointer {
    public:
        Pointer() {}
        ~Pointer() {}
    private:
        std::shared_ptr<std::pair<Key, int>> ptr;
        void SetPtr(std::pair<Key, int> curr) {
            ptr = std::make_shared<std::pair<Key, int>>(curr);
        }
        std::shared_ptr<std::pair<Key, int>> GetPtr() {
            return ptr;
        }
        friend BinaryHeap;
    };
    BinaryHeap();
    ~BinaryHeap();
    void optimize(size_t insertCount, size_t extractCount) {
        if(insertCount < 0 || extractCount < 0 || size > 0){
            throw std::logic_error("BinaryHeap is empty");
        }
        this->k = FindK(insertCount, extractCount);
    }
    bool is_empty() const;
    Pointer insert(Key key) {
        if (size == 0) {
            Array<Key> new_buffer;
            buffer = new_buffer;
        }
        buffer.arr[size] = std::make_pair(key, size);
        size++;
        int index = SiftUp(size - 1);
        if (size == buffer.array_size) {
            buffer.ScaleUpArray();
        }
        BinaryHeap<Key>::Pointer ptr;
        ptr.SetPtr(buffer.arr[index]);
        return ptr;
    }
    void change(Pointer ptr, Key key) {
        int index = ptr.ptr->second;
        if (buffer.arr[index].first < key) {
            buffer.arr[index].first = key;
            SiftDown(index);
        }
        else {
            buffer.arr[index].first = key;
            SiftUp(index);
        }
    }
    void delet(Pointer ptr) {
        if (size == 0) {
            throw std::logic_error("BinaryHeap is empty");
        }
        change(ptr, get_min());
        extract_min();
    }
    template<class Iterator>
    void IPriorityQueue(Iterator begin, Iterator end) {
        if (size != 0) {
            throw std::logic_error("BinaryHeap is empty");
        }
        if (begin == end) {
            return;
        }
        int curr_size = 0;
        auto itr = begin;
        while (itr != end) {
            ++curr_size;
            ++itr;
        }
        itr = begin;
        int index = 0;
        std::pair<Key, int> * new_arr = new std::pair<Key, int>[curr_size];
        while (itr != end) {
            new_arr[index] = std::make_pair(*itr, index);
            //std::cout << index << " =index||" << *itr << " *itr||" << std::endl;
            itr++;
            index++;
        }
        buffer.arr = new_arr;
        buffer.array_size = curr_size;
        size = curr_size;
        for (int i = curr_size - 1; i >= 0; --i) {
            SiftDown(i);
        }
        for (int i = 0; i < 0; ++i) {
            std::cout << buffer.arr[i].first << std::endl;
        }

        return;
    }

    Key get_min() const;
    Key extract_min();
private:
    size_t k = 2;
    size_t size;
    Array<Key> buffer;

    int SiftUp(int index);
    int SiftDown(int index);
};
// __________Heaps methods_____________
template<typename Key>
BinaryHeap<Key>::BinaryHeap() {
    size = 0;
}
template<typename Key>
BinaryHeap<Key>::~BinaryHeap() {
    buffer.~Array();
}
template<typename Key>
bool BinaryHeap<Key>::is_empty() const {
    return (size == 0);
}


template<typename Key>
Key BinaryHeap<Key>::get_min() const {
    if (size == 0) {
        throw std::logic_error("BinaryHeap is empty");
    }
    return buffer.arr[0].first;
}
template<typename Key>
Key BinaryHeap<Key>::extract_min() {
    if (size == 0) {
        throw std::logic_error("BinaryHeap is empty");
    }
    Key minimum = get_min();
    if (size == 1) {
        buffer.arr = nullptr;
        buffer.array_size = 0;
        size--;
        return minimum;
    }
    buffer.arr[0].first = buffer.arr[size - 1].first;
    size--;
    SiftDown(0);
    if (size == buffer.array_size * 0.5) {
        buffer.ScaleDownArray();
    }
    return  minimum;
}
template<typename Key>
void Array<Key>::ScaleUpArray() {
    std::pair<Key, int> *new_arr = new std::pair<Key, int>[array_size * 2];
    for (int i = 0; i < array_size; ++i) {
        new_arr[i] = arr[i];
    }
    if (array_size == 0) {
        array_size = 1;
    }
    else {
        array_size = array_size * 2;
    }
    arr = new_arr;
}
template<typename Key>
void Array<Key>::ScaleDownArray() {
    std::pair<Key, int>* new_arr = new std::pair<Key, int>[(int)(array_size * 0.7)];
    for (int i = 0; i < array_size * 0.7; ++i) {
        new_arr[i] = arr[i];
    }
    array_size = array_size * 0.7;
    arr = new_arr;
}
template<typename Key>
int BinaryHeap<Key>::SiftUp(int index) {
    int k = this->k;
    while (index > 0 && buffer.arr[(index - 1) / k].first >= buffer.arr[index].first) {
        std::swap(buffer.arr[(index - 1) / k].first, buffer.arr[index].first);
        index = (index - 1) / k;
    }
    return index;
}
template<typename Key>
int BinaryHeap<Key>::SiftDown(int index) {
    int k = this->k;
    while (k * index + 1 < size) {
        int left = k * index + 1;
        int right = k * index + k;
        int potential = left;
        for (int i = left; i <= right; ++i) {
            if (i < size && buffer.arr[i].first < buffer.arr[potential].first) {
                potential = i;
            }
        }
        if (buffer.arr[potential] > buffer.arr[index]) {
            break;
        }
        std::swap(buffer.arr[potential].first, buffer.arr[index].first);
        index = potential;
    }
    return index;
}


#endif //UNTITLED6_BINARYHEAP_H
