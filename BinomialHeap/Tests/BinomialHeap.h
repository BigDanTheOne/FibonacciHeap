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
class BinomialHeap {
public:
    struct Node;
    struct NodeLink {
        Node * node;
        int degree;
        NodeLink * parent;
        NodeLink * right_brother;
        NodeLink * left_brother;
        NodeLink * left_child;
    };
    struct Node {
        Key data;
        NodeLink * link;
        bool * deleted;
    };
    class Pointer {
    public:
        Pointer() {}
        ~Pointer() {}
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
        friend BinomialHeap;
    };
    NodeLink * head = nullptr;

    BinomialHeap() {
    }
    BinomialHeap(Key key) {
        Node * node = new Node;
        NodeLink * lnode = new NodeLink;
        min = key;
        node->data = key;
        node->link = lnode;
        lnode->parent = nullptr;
        lnode->node = node;
        lnode->degree = 0;
        lnode->right_brother = nullptr;
        lnode->left_brother = nullptr;
        lnode->left_child = nullptr;
        this->head = lnode;
    }
    ~BinomialHeap() {
        if (head != nullptr) {
            DestructHeap(head);
        }
    }
    bool is_empty() const {
        if (this->head == nullptr)
            return true;
        else
            return false;
    }
    Key get_min() {
        if (head == nullptr) {
            throw std::runtime_error("dsfds");
        }
        return min;
    }
    void merge(BinomialHeap & otherHeap) {
        if (head == nullptr) {
            head = otherHeap.head;
            min = otherHeap.min;
            otherHeap.head = nullptr;
            return;
        }
        if (otherHeap.head == nullptr) {
            return;
        }
        if (this->min > otherHeap.min) {
            this->min = otherHeap.min;
        }
        NodeLink * H;
        NodeLink * H2ptr = otherHeap.head;
        NodeLink * H1ptr = this->head;
        if (H1ptr->degree < H2ptr->degree) {
            H = H1ptr;
            H1ptr = H1ptr->right_brother;
        }
        else {
            H = H2ptr;
            H2ptr = H2ptr->right_brother;
        }
        NodeLink * Hptr = H;
        while (H1ptr != nullptr && H2ptr != nullptr) {
            if (H1ptr->degree < H2ptr->degree) {
                H1ptr->left_brother = Hptr;
                Hptr->right_brother = H1ptr;
                H1ptr = H1ptr->right_brother;
            }
            else {
                H2ptr->left_brother = Hptr;
                Hptr->right_brother = H2ptr;
                H2ptr = H2ptr->right_brother;
            }
            Hptr = Hptr->right_brother;
        }
        if (H1ptr == nullptr) {
            while (H2ptr != nullptr) {
                H2ptr->left_brother = Hptr;
                Hptr->right_brother = H2ptr;
                Hptr = Hptr->right_brother;
                H2ptr = H2ptr->right_brother;
            }
        }
        else {
            while (H1ptr != nullptr) {
                H1ptr->left_brother = Hptr;
                Hptr->right_brother = H1ptr;
                Hptr = Hptr->right_brother;
                H1ptr = H1ptr->right_brother;
            }
        }
        Hptr = H;
        while (Hptr->right_brother != nullptr) {
            if (Hptr->right_brother->right_brother != nullptr &&
                Hptr->degree == Hptr->right_brother->degree &&
                Hptr->right_brother->degree == Hptr->right_brother->right_brother->degree) {
                Hptr = Hptr->right_brother;
            }
            if (Hptr->degree == Hptr->right_brother->degree) {
                if (Hptr->node->data < Hptr->right_brother->node->data) {
                    if (Hptr->left_child != nullptr) {
                        Hptr->left_child->left_brother = Hptr->right_brother;
                    }
                    Hptr->right_brother->parent = Hptr;
                    auto tmp = Hptr->right_brother->right_brother;
                    if (tmp != nullptr) {
                        tmp->left_brother = Hptr;
                    }
                    Hptr->right_brother->right_brother = Hptr->left_child;
                    Hptr->left_child = Hptr->right_brother;
                    Hptr->right_brother = tmp;
                    Hptr->left_child->left_brother = nullptr;
                    Hptr->degree += 1;
                    continue;
                }
                else {
                    Hptr->right_brother->degree += 1;
                    if (Hptr->left_brother != nullptr) {
                        Hptr->left_brother->right_brother = Hptr->right_brother;
                    }
                    Hptr->parent = Hptr->right_brother;
                    if (Hptr->right_brother->left_child != nullptr) {
                        Hptr->right_brother->left_child->left_brother = Hptr;
                    }
                    Hptr->right_brother = Hptr->right_brother->left_child;
                    Hptr->parent->left_child = Hptr;
                    Hptr->parent->left_brother = Hptr->left_brother;
                    Hptr->left_brother = nullptr;
                    while (Hptr->parent != nullptr) {
                        Hptr = Hptr->parent;
                    }
                    continue;
                }
            }
            Hptr = Hptr->right_brother;
        }
        while (H->parent != nullptr) {
            H = H->parent;
        }
        this->head = H;
        otherHeap.head = nullptr;
    }
    Pointer insert(Key key) {
        BinomialHeap<Key> *tmp = new BinomialHeap<Key>(key);
        Pointer ptr;
        ptr.SetPtr((tmp->head->node));
        tmp->head->node->deleted = ptr.GetDeletedPtr();
        this->merge(*tmp);
        delete tmp;
        return ptr;
    }
    Key extract_min() {
        if (head == nullptr) {
            throw std::runtime_error("dsfds");
        }
        NodeLink * curr = this->head;
        //_______single_node_______
        if (curr->right_brother == nullptr && curr->left_child == nullptr) {
            *this->head->node->deleted = true;
            this->head = nullptr;
            return min;
        }
        //______plural_nodes________
        Key minimum = min;
        while (curr->node->data != this->min) {
            curr = curr->right_brother;
        }
        *(curr->node->deleted) = true;
        NodeLink * curr_left_child = curr->left_child;
        curr->left_child = nullptr;
        auto tmp = curr->right_brother;
        if (curr->left_brother != nullptr) {
            curr->left_brother->right_brother = curr->right_brother;
        }
        else {
            this->head = curr->right_brother;
        }
        if (curr->right_brother != nullptr) {
            curr->right_brother->left_brother = curr->left_brother;
        }
        //delete curr;
        curr = curr_left_child;
        while (curr != nullptr) {
            curr->parent = nullptr;
            curr = curr->right_brother;
        }
        BinomialHeap<Key> new_heap;
        while (curr_left_child != nullptr) {
            auto tmp = curr_left_child->right_brother;
            curr_left_child->right_brother = curr_left_child->left_brother;
            curr_left_child->left_brother = tmp;
            if (tmp != nullptr) {
                curr_left_child = tmp;
            }
            else {
                break;
            }

        }
        new_heap.head = curr_left_child;
        new_heap.min = min;
        if (new_heap.head != nullptr) {
            this->merge(new_heap);
        }

        //___________update_min___________
        curr = this->head;
        Key min = curr->node->data;
        while (curr != nullptr) {
            if (min > curr->node->data) {
                min = curr->node->data;
            }
            curr = curr->right_brother;
        }
        this->min = min;
        return minimum;
    }
    void change(Pointer ptr, Key key) {
        if (*ptr.deleted == true) {
            throw std::runtime_error("Node is already deleted");
        }
        NodeLink * curr = ptr.GetPtr()->link;
        if (key < curr->node->data) {
            if (key < this->min) {
                this->min = key;
            }
            curr->node->data = key;
            while (curr->parent != nullptr && curr->node->data <= curr->parent->node->data) {
                VerticalSwap(curr, curr->parent);
                curr = curr->parent;
            }
        }
        else {
            curr->node->data = key;
            auto curr_child = curr->left_child;
            if (curr_child != nullptr) {
                auto curr_child = curr->left_child;
                Key min = curr_child->node->data;
                while (curr_child->right_brother != nullptr) {
                    curr = curr->right_brother;
                    min = Min(min, curr_child->node->data);
                }
                while (curr_child->node->data != min) {
                    curr_child = curr_child->right_brother;
                }
                if (curr_child->node->data < curr->node->data)
                    VerticalSwap(curr, curr_child);
            }
        }
        return;
    }
    void delet(Pointer ptr) {
        change(ptr, this->min);
        *(ptr.deleted) = true;
        extract_min();
    }
private:
    void DestructHeap(NodeLink * curr) {
        if (curr->right_brother != nullptr) {
            DestructHeap(curr->right_brother);
        }
        if (curr->left_child != nullptr) {
            DestructHeap(curr->left_child);
        }
        delete curr->node;
        delete curr;
    }
    Key Min(Key i, Key j) {
        if (i < j) {
            return i;
        }
        else {
            return j;
        }
    }
    void VerticalSwap(NodeLink * child, NodeLink * parent) {
        std::swap(child->node->link, parent->node->link);
        std::swap(child->node, parent->node);
    }
    Key min;
};




#endif //UNTITLED6_BINARYHEAP_H
