#ifndef UNTITLED6_BINARYHEAP_H
 #define UNTITLED6_BINARYHEAP_H
 
 #include<cmath>
 #include<gtest/gtest.h>
 #include <iostream>


template <typename Key>
struct Node {
	Key data;
	int index;
};


template <typename Key>
class Vector {
public:
	
	Key * arr;
	size_t array_size;
	Vector() {
		arr = new Key[1];
		array_size = 1;
	}

	~Vector(){
		delete arr;
	}
	void ScaleUpArray() {
		Key *new_arr = new Key[array_size * 2];
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
	void ScaleDownArray() {
		Key *new_arr = new Key[(int)(array_size * 0.7)];
		for (int i = 0; i < (int)(array_size * 0.7); ++i) {
			new_arr[i] = arr[i];
		}
		array_size = array_size * 0.7;
		arr = new_arr;

	}
};

double f(int k, size_t & insertCount, size_t & extractCount) {
	int n = insertCount + extractCount;
	return insertCount * std::log(n) / std::log(k) + extractCount * k * std::log(n) / std::log(k);
}

int FindK(size_t insertCount, size_t extractCount) {
	double min = f(2, insertCount, extractCount);
	int k = 2;
	for (int i = 2; i < insertCount / 2; ++i) {
		if (f(2, insertCount, extractCount) < min) {
			min = f(2, insertCount, extractCount);
			k = i;
		}
	}
	return k;
}

template <typename Key >
class BinaryHeap {
public:
	class Pointer {
	public:
		Pointer() {}
		~Pointer() {}
	private:
		Node<Key> * ptr;
		bool * deleted = new bool(false);
		void SetPtr(Node<Key> * curr) {
			ptr = curr;
		}
		Node<Key> * GetPtr() {
			return ptr;
		}
		bool * GetDeletedPtr() {
			return deleted;
		}
		friend BinaryHeap;
	};
	BinaryHeap() {
		size = 0;
	}
	~BinaryHeap() {};
	void optimize(int insertCount, int extractCount) {
		if (insertCount < 0 || extractCount < 0) {
			throw std::logic_error("Incorrect input");
		}
		if (size > 0) {
			throw std::logic_error("Heap is not emty");
		}
		this->k = FindK(insertCount, extractCount);
	}
	bool is_empty() const {
		return (size == 0);
	}
	Pointer insert(Key key) {
		if (size == 0) {
			Node<Key> ** new_buffer = new Node<Key> *[1];
			buffer.arr = new_buffer;
		}
		Node<Key> * new_node = new Node<Key>;
		new_node->data = key;
		new_node->index = size;
		//std::cout << typeid(buffer.arr[size]).name() << std::endl;
		buffer.arr[size] = new_node;
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
		if (size == 0) {
			throw std::logic_error("BinaryHeap is empty");
		}
		int index = ptr.ptr->index;
		std::cout << typeid(buffer.arr[index]).name() << std::endl;
		
		if (buffer.arr[index]->data < key) {
			buffer.arr[index]->data = key;
			SiftDown(index);

			
		}
		else {
			buffer.arr[index]->data = key;
			SiftUp(index);
		}
	}
	void delet(Pointer ptr) {
		if (size == 0) {
			throw std::logic_error("BinaryHeap is empty");
		}
		change(ptr, get_min());
		extract_min();
		*ptr.deleted = true;
	}
	template<class Iterator>
	void IPriorityQueue(Iterator begin, Iterator end) {
		if (size != 0) {
			throw std::logic_error("BinaryHeap is empty!!!!!!");
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
		Node<Key>** new_arr = new Node<Key>*[curr_size];
		while (itr != end) {
			Node<Key> * new_node = new Node<Key>;
			new_node->data = *itr;
			new_node->index = index;
			new_arr[index] = new_node;
			itr++;
			index++;
		}
		std::cout << typeid(buffer.arr).name() << typeid(new_arr).name() << std::endl;
		buffer.arr = new_arr;
		buffer.array_size = curr_size;
		size = curr_size;

		for (int i = curr_size - 1; i >= 0; --i) {
			SiftDown(i);
		}
		buffer.ScaleUpArray();
		return;
	}

	Key get_min() const {
		if (size == 0) {
			throw std::logic_error("BinaryHeap is empty");
		}
		return buffer.arr[0]->data;
	}
	Key extract_min() {
		if (size == 0) {
			throw std::logic_error("BinaryHeap is empty");
		}
		Key minimum = get_min();
		if (size == 1) {
			//buffer.arr = nullptr;
			buffer.array_size = 0;
			size--;
			delete buffer.arr[0];
			return minimum;
		}
		std::swap(buffer.arr[0]->index, buffer.arr[size - 1]->index);
		std::swap(buffer.arr[0], buffer.arr[size - 1]);
		size--;
		SiftDown(0);
		delete buffer.arr[size];
		if (size == buffer.array_size * 0.5) {
			buffer.ScaleDownArray();
		}
		return  minimum;
	}
private:
	size_t k = 2;
	size_t size;
	Vector<Node<Key>* > buffer;

	int SiftUp(int index) {
		int k = this->k;
		while (index > 0 && buffer.arr[(index - 1) / k]->data >= buffer.arr[index]->data) {
			std::swap(buffer.arr[(index - 1) / k]->index, buffer.arr[index]->index);
			std::swap(buffer.arr[(index - 1) / k], buffer.arr[index]);
			index = (index - 1) / k;
		}
		return index;
	}
	int SiftDown(int index) {
		int k = this->k;
		while (k * index + 1 < size) {
			int left = k * index + 1;
			int right = k * index + k;
			int potential = left;
			for (int i = left; i <= right; ++i) {
				if (i < size && buffer.arr[i]->data < buffer.arr[potential]->data) {
					potential = i;
				}
			}
			if (buffer.arr[potential]->data > buffer.arr[index]->data) {
				break;
			}
			std::swap(buffer.arr[potential]->index, buffer.arr[index]->index);
			std::swap(buffer.arr[potential], buffer.arr[index]);
			index = potential;
		}
		return index;
	}
};

#endif //UNTITLED6_BINARYHEAP_H
