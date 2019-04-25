//
// Created by akopi on 18-Nov-18.
//

#ifndef UNTITLED6_BINARYHEAP_H
#define UNTITLED6_BINARYHEAP_H

#include <iostream>
#include<cmath>
#include<set>
#include<memory>
#include<vector>

template <typename Key>
class FibonacciHeap {
public:
    struct Node {
        Key data;
        bool * deleted;
        int degree = 0;
        Node * parent = nullptr;
        Node * right_brother = nullptr;
        Node * left_brother = nullptr;
        Node * left_child = nullptr;
        bool mark = false;
    };
    class Pointer {
    public:
        Pointer() {}
        ~Pointer(){}
    private:
        Node * ptr;
        bool * deleted = new bool(false);
        void SetPtr(Node * curr) {
            ptr = curr;
        }
        Node * GetPtr() {
            return ptr;
        }
        bool * GetDeletedPtr() {
            return deleted;
        }
        friend FibonacciHeap;
    };
    Node * head = nullptr;
    Node * tail = nullptr;

    FibonacciHeap() {
    }
    FibonacciHeap(Key key) {
        Node * lnode = new Node;
        min = lnode;
        lnode->data = key;
        this->head = lnode;
        this->tail = lnode;
        lnode->degree = 0;
        nnodes = 1;

    }

    bool is_empty() const {
        if (head == nullptr)
            return true;
        else
            return false;
    }
    Key get_min() {
        if (head == nullptr) {
            throw std::runtime_error("Heap is empty!!!!!");
        }
        return min->data;
    }
    void merge(FibonacciHeap & otherHeap) {
        if (head == nullptr) {
            degree = otherHeap.degree;
            head = otherHeap.head;
            tail = otherHeap.tail;
            min = otherHeap.min;
            nnodes = otherHeap.nnodes;
            otherHeap.head = nullptr;
            otherHeap.tail = nullptr;
            otherHeap.min = nullptr;
            return;
        }
        if (otherHeap.head == nullptr) {
            return;
        }
        if (min->data > otherHeap.min->data) {
            min = otherHeap.min;
        }
        if (degree < otherHeap.degree) {
            degree = otherHeap.degree;
        }
        nnodes += otherHeap.nnodes;
        auto curr = tail;
        curr->right_brother = otherHeap.head;
        (otherHeap.head)->left_brother = curr;
        tail = otherHeap.tail;
        otherHeap.head = nullptr;
        otherHeap.tail = nullptr;
        otherHeap.min = nullptr;
        otherHeap.nnodes = 0;
    }
    Pointer insert(Key key) {
        FibonacciHeap<Key> *tmp = new FibonacciHeap<Key>(key);
        Pointer ptr;
        ptr.SetPtr(tmp->head);
        tmp->head->deleted = ptr.GetDeletedPtr();
        tmp->degree = 0;
        merge(*tmp);
        delete tmp;
        return ptr;
    }
    Key extract_min() {
        if (head == nullptr) {
            throw std::runtime_error("Heap is empty!!!!");
        }
        Node * curr = min;
        Node * end = tail;
        //_______single_node_______
        if (head->right_brother == nullptr && head->left_child == nullptr) {
            *head->deleted = true;
            head = nullptr;
            tail = nullptr;
            degree = -1;
            nnodes = 0;
            return min->data;
        }
        //______plural_nodes________
        Key minimum = min->data;
        *curr->deleted = true;
        if (*end->deleted == true) {
            end = end->left_brother;
        }
        Node * curr_left_child = curr->left_child;
        curr->left_child = nullptr;
        auto tmp = curr->right_brother;
        if (curr->left_brother != nullptr) {
            curr->left_brother->right_brother = curr->right_brother;
        }
        else {
            head = curr->right_brother;
        }
        if (curr->right_brother != nullptr) {
            curr->right_brother->left_brother = curr->left_brother;
        }
        else {
            tail = curr->left_brother;
        }
        if (curr_left_child != nullptr) {
            curr = curr_left_child;
            if (curr->right_brother != nullptr) {
                curr->right_brother->parent = nullptr;
            }
            while (curr->right_brother != nullptr) {
                curr->right_brother->parent = nullptr;
                curr = curr->right_brother;
            }
            //__________single_tree__________
            if (end == nullptr) {
                head = curr_left_child;
            }
                //_________plural_tree_____________
            else {
                //_____merge_children_with_general_list_of_nodes_____
                if (curr_left_child != nullptr) {
                    end->right_brother = curr_left_child;
                    curr_left_child->left_brother = end->right_brother;
                    tail = curr;
                }
            }
        }


        delete min;
        consolidate();
        //___________update_min_and__degree___________
        curr = head;
        min = curr;
        int max_degree = 0;
        while (curr != nullptr) {
            if (min->data > curr->data) {
                min = curr;
            }
            if (max_degree < curr->degree) {
                max_degree = curr->degree;
            }
            curr = curr->right_brother;
        }
        degree = max_degree;
        nnodes -= 1;
        return minimum;
    }
    void decrease(Pointer ptr, Key key) {
        if (*ptr.deleted == true) {
            throw std::runtime_error("Node is already deleted");
        }
        Node * curr = ptr.GetPtr();
        if (key > curr->data) {
            throw std::runtime_error("Incorrect key");
        }
        if (min->data > key) {
            min = curr;
        }
        curr->data = key;
        if (curr->parent == nullptr || curr->parent->data < key) {
            return;
        }
        Cut(curr);
    }

private:
    void Cut(Node * curr) {
        if (curr->parent == nullptr) {
            curr->mark = false;
            return;
        }
        if (curr->right_brother != nullptr) {
            curr->right_brother->left_brother = curr->left_brother;
        }
        if (curr->left_brother != nullptr) {
            curr->left_brother->right_brother = curr->right_brother;
        }
        curr->mark = false;
        curr->parent->degree -= 1;
        if (curr->parent->left_child == curr) {
            curr->parent->left_child = curr->right_brother;
        }
        curr->left_brother = nullptr;
        curr->right_brother = head;
        head->left_brother = curr;
        head = curr;
        if (curr->parent->mark == true) {
            Cut(curr->parent);
        }
        else {
            curr->parent->mark = true;
        }
        curr->parent = nullptr;
    }
    void MergeNodes(Node** & array, Node * curr) {
        if (array[curr->degree] == nullptr) {
            array[curr->degree] = curr;
        }
        else {
            if (curr->data <= array[curr->degree]->data) {
                //__________added_tree_goes_up_________
                if (curr->left_child != nullptr) {
                    curr->left_child->left_brother = array[curr->degree];
                }
                array[curr->degree]->parent = curr;
                array[curr->degree]->right_brother = curr->left_child;
                curr->left_child = array[curr->degree];
                curr->left_child->left_brother = nullptr;
                array[curr->degree] = curr;
                if (degree < curr->degree + 1) {
                    degree = curr->degree + 1;
                }
                array[curr->degree] = nullptr;
                curr->degree = curr->degree + 1;
                MergeNodes(array, curr);
            }
            else {
                //added tree goes down
                curr->parent = array[curr->degree];
                if (array[curr->degree]->left_child != nullptr) {
                    array[curr->degree]->left_child->left_brother = curr;
                }
                curr->right_brother = array[curr->degree]->left_child;
                array[curr->degree]->left_child = curr;;
                curr->left_brother = nullptr;
                array[curr->degree] = nullptr;
                curr = curr->parent;
                curr->degree += 1;
                if (degree < curr->degree) {
                    degree = curr->degree;
                }
                MergeNodes(array, curr);
            }
        }
    }

    void consolidate() {
        int array_size = (int)std::log2(nnodes) + 10;
        //std::vector<Node*> array(array_size);
        Node** array = new Node*[array_size];
        for (int i = 0; i <array_size; ++i) {
            array[i] = nullptr;
        }
        auto curr = head;
        while (curr != nullptr) {
            auto tmp = curr->right_brother;
            if (curr->right_brother != nullptr) {
                curr->right_brother->left_brother = nullptr;
            }
            curr->right_brother = nullptr;
            MergeNodes(array, curr);
            curr = tmp;
        }
        int begin;
        for (int i = 0; i < array_size; ++i) {
            if (array[i] != nullptr) {
                begin = i;
                break;
            }
        }
        head = array[begin];
        curr = head;
        for (int i = begin + 1; i < array_size; ++i) {
            if (array[i] != nullptr) {
                curr->right_brother = array[i];
                array[i]->left_brother = curr;
                curr = array[i];
            }
        }
        tail = curr;
        delete array;
        return;
    }
    Node * min;
    int degree = -1;
    int nnodes = 0;
};




#endif //UNTITLED6_BINARYHEAP_H
