#ifndef FILE_H
#define FILE_H

#include "Maillon.h"

template<typename E>
class Queue
{
	private:
		Maillon<E> *first;
		Maillon<E> *last;

	public:
		Queue();
		void enqueue(E e1);
		E dequeue();
		bool isEmpty();
};

#endif //FILE_H
