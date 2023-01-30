#ifndef __QUEUE_H_
#define __QUEUE_H_

#include <iostream>

template <typename T>
class Queue {
public:
	// Constructor
	Queue() {
		// Initializes array at size of 1 with no element
		elements = new T[1];
		size = 1;
		count = 0;
	}

	// Destructor
	~Queue() {
		// Deletes every element if T is a pointer type
		if constexpr (std::is_pointer<T>::value) {
			for (int i = 0; i < size; i++) {
				delete elements[i];
			}
		}

		delete[] elements;
	}

	// Copy Constructor
	Queue(const Queue<T>& other) {
		elements = new T[other.size];
		size = other.size;
		count = other.count;

		// Clone elements from the other Queue
		for (int i = 0; i < size; i++) {
			this->elements[i] = other.elements[i];
		}
	}

	// Copy Assignment Operator
	Queue& operator=(const Queue<T>& other) {
		if (this != &other) {
			// Clean the previous array to avoid memory leak
			delete[] elements;

			elements = new T[other.size];
			size = other.size;
			count = other.count;

			// Clone elements from the other Queue
			for (int i = 0; i < size; i++) {
				this->elements[i] = other.elements[i];
			}
		}
		return *this;
	}

	/// <summary>
	/// Adds information to the right position according to its priority
	/// </summary>
	/// <param name="element">Element to add to the array</param>
	void Push(T element) {
		// Check if there's enough space in the array
		if (count == size) {
			// Allocate a new array with double the size
			int newSize = size * 2;
			T* newElements = new T[newSize];

			// Copy elements from the old array to the new array
			for (int i = 0; i < size; i++) {
				newElements[i] = elements[i];
			}

			// Replace the old array with the new array
			delete[] elements;
			elements = newElements;
			size = newSize;
		}

		// Insert the new element into the right position (smaller element gets higher priority)
		int i = count;
		while (i > 0 && element < elements[i - 1]) {
			elements[i] = elements[i - 1];
			i--;
		}
		elements[i] = element;

		// Increment the count
		count++;
	}

	/// <summary>
	/// Removes information of the queue from the front
	/// </summary>
	/// <returns>First element in the queue</returns>
	T Pop() {
		T firstElement = elements[0];
		// Shifts the entries one index down
		for (int i = 1; i < count; i++) {
			elements[i - 1] = elements[i];
		}
		count--;
		return firstElement;
	}

	/// <summary>
	/// Writes in console the contents of the queue
	/// </summary>
	void Print() {
		std::cout << "Begining of queue......." << std::endl;
		for (int i = 0; i < count; i++) {
			std::cout << elements[i] << std::endl;
		}
		std::cout << "End of queue............" << std::endl;
	}

	/// <returns>Number of entries in the queue (not the size of the queue)</returns>
	int GetSize() {
		return count;
	}

	/// <returns>Whether the queue is empty (no entry exists) </returns>
	bool isEmpty() {
		return count == 0;
	}

private:
	T* elements;
	int size;
	int count;
};

#endif //__QUEUE_H_