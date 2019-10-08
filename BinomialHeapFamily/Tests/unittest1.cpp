#include <ctime>
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>
#include<vector>

#include "stdafx.h"
#include "CppUnitTest.h"
#include "../UnitTest1/binomialHeapFamily.h"
#include "../UnitTest1/IndexPQ.h"
#include "../UnitTest1/PriorityQueue.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(insertAndMinimum)
		{
			//srand(time(NULL));
			srand(3000);
			PriorityQueue <int> queue;
			binomialHeapFamily <int> binheap;
			int random = rand() % 300000;
			binheap.insert(random);
			queue.push(random);

			for (int i = 1; i < 200000; i++) {
				int random = rand() % 300000;
				queue.push(random);
				binheap.insert(random, 1);
				int min = queue.top();
				int min2 = binheap.minimum(1);
				Assert::AreEqual(min, min2);
			}
		}


		TEST_METHOD(extractMin) {
			srand(3000);
			PriorityQueue <int> queue;
			binomialHeapFamily <int> binheap;
			int random = rand() % 300000;
			binheap.insert(random);
			queue.push(random);

			for (int i = 1; i < 200000; i++) {
				int aleatorio = rand() % 300000;
				if (!binheap.isElem(random)) {
					queue.push(random);
					binheap.insert(random, 1);
					Assert::AreEqual(queue.top(), binheap.minimum(1));
					queue.pop();
					binheap.extractMin(1);
				}
			}
		}

		TEST_METHOD(decreaseKey) {
			srand(3000);
			IndexPQ <int, less<int>> queue(200000);
			binomialHeapFamily <int> binheap;
			int random = rand() % 300000;
			binheap.insert(random);
			queue.push(random, random);

			for (int i = 0; i < 200000; ++i) {
				int random = rand() % 300000;
				if (!binheap.isElem(random)) {
					queue.push(random, random);
					binheap.insert(random, 1);
				}
			}
			for (int j = 0; j < 200000; j++) {
				int bigRandom = rand() % 300000;
				int smallRandom = rand() % 30000;

				if (binheap.isElem(bigRandom) && !binheap.isElem(smallRandom) &&
					smallRandom < bigRandom) {
					queue.update(bigRandom, smallRandom);
					binheap.decreaseKey(bigRandom, smallRandom);
					int min = queue.top().prioridad;
					int min2 = binheap.minimum(1);
					Assert::AreEqual(min, min2);
				}
			}
		}

		TEST_METHOD(deleteKey) {
			srand(3000);
			binomialHeapFamily <int> binheap;
			vector <int> v;
			int random = rand() % 300000;
			binheap.insert(random);
			v.push_back(random);


			for (int i = 0; i < 200000; i++) {
				int aleatorio = rand() % 300000;
				if (!binheap.isElem(random))
					v.push_back(random);
				binheap.insert(random, 1);
			}
			while (!v.empty()) {
				binheap.deleteKey(v.back());
				v.pop_back();
			}

			Assert::AreEqual(true, binheap.emptyKey(1));
		}
	};

	TEST_CLASS(UnitTest2)
	{
	public:
		TEST_METHOD(minNotFound)
		{
			binomialHeapFamily<int> binheap;
			try {
				binheap.minimum(1);
				Assert::Fail();
			}
			catch (domain_error d) {
			}
		}

		TEST_METHOD(keyNotFound)
		{
			binomialHeapFamily<int> binheap;
			try {
				binheap.decreaseKey(3, 1);
				Assert::Fail();
			}
			catch (domain_error d) {
			}
		}

		TEST_METHOD(keyAlreadyExists)
		{
			binomialHeapFamily<int> binheap;
			binheap.insert(1);
			binheap.insert(2);
			try {
				binheap.decreaseKey(2, 1);
				Assert::Fail();
			}
			catch (domain_error d) {
			}
		}

		TEST_METHOD(nonValidNewKey)
		{
			binomialHeapFamily<int> binheap;
			binheap.insert(1);
			binheap.insert(2);
			try {
				binheap.decreaseKey(2, 5);
				Assert::Fail();
			}
			catch (domain_error d) {
			}
		}

		TEST_METHOD(nonExistingHeap)
		{
			binomialHeapFamily<int> binheap;
			try {
				binheap.insert(3, 5);
				Assert::Fail();
			}
			catch (domain_error d) {
			}
		}
	};
}