#include "Main.h"
using namespace std;

void wrapper() {
#pragma region Rule of Three
	cout << "Test - Rule of Three" << endl;
	Queue<int> testQueue1;
	testQueue1.Push(5);
	testQueue1.Push(123);
	testQueue1.Push(3);
	testQueue1.Print();

	cout << endl;
	cout << "Copy Constructor" << endl;
	Queue<int> testQueue2(testQueue1);
	testQueue2.Print();

	cout << endl;
	cout << "Copy Assignment Operator" << endl;
	Queue<int> testQueue3;
	testQueue3.Push(10);
	testQueue3.Push(7);
	testQueue3.Print();
	cout << endl;
	testQueue1 = testQueue3;
	testQueue1.Print();

	cout << endl;
	cout << "Destructor is a bit hard to tell immediately but it's there (no memory leak)" << endl;

#pragma endregion

#pragma region Int Queue
	cout << endl;
	cout << "Test - Int Queue" << endl;
	Queue<int> intQueue;
	// Initial elements
	intQueue.Push(0);
	cout << "	just pushed +++++" << endl;
	intQueue.Push(1);
	cout << "	just pushed +++++" << endl;
	intQueue.Push(2);
	cout << "	just pushed +++++" << endl;
	intQueue.Print();

	// Pop an element
	intQueue.Pop();
	cout << "		just popped -----" << endl;
	// Add more elements
	intQueue.Push(3);
	cout << "	just pushed +++++" << endl;
	intQueue.Push(4);
	cout << "	just pushed +++++" << endl;
	intQueue.Push(5);
	cout << "	just pushed +++++" << endl;
	intQueue.Print();

	// Pop elements
	intQueue.Pop();
	cout << "		just popped -----" << endl;
	intQueue.Pop();
	cout << "		just popped -----" << endl;
	intQueue.Print();

	// Empty the queue
	int count = intQueue.GetSize();
	for (int i = 0; i < count; i++) {
		intQueue.Pop();
		cout << "		just popped -----" << endl;
	}
	intQueue.Print();
	cout << endl;
	cout << "Calling `isEmpty()` to check..." << endl;
	if (intQueue.isEmpty()) {
		cout << "The int queue is now empty!" << endl;
	}

	// Complex Data usage
	cout << endl;
	cout << "Int Queue With Complex Operations" << endl;
	for (int i = 0; i < 10; i++)
	{
		intQueue.Push(i);
		cout << "	just pushed +++++" << endl;
		if (i % 2 == 0)
		{
			intQueue.Pop();
			cout << "		just popped -----" << endl;
		}
	}
	intQueue.Print();

#pragma endregion

#pragma region Pointer Queue
	cout << endl;
	cout << "Test - Pointer Queue (of ints)" << endl;
	Queue<int*> ptrQueue;
	ptrQueue.Push(new int(222));
	ptrQueue.Push(new int(333));
	ptrQueue.Print();
#pragma endregion

#pragma region Alberto Queue
	cout << endl;
	cout << "Test - Alberto Queue" << endl;
	Queue<AlbertoClass> albertoQueue;
	for (int i = 30; i < 40; i++)
	{
		AlbertoClass alberto(i);
		albertoQueue.Push(alberto);
		cout << "	just pushed +++++" << endl;
	}
	albertoQueue.Print();

#pragma endregion
}

int main()
{
	wrapper();
	// Output memory leaks (if any)
	_CrtDumpMemoryLeaks();

	cout << endl;
	cout << "Press Enter to finish." << endl;
	getchar();
}
