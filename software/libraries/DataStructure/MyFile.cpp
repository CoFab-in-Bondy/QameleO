#include "MyFile.h"

#include "Maillon.h"
//#include "Maillon.cpp"
#include <stdlib.h>

template<typename E>
Queue<E>::Queue()
{
	this->first=NULL;
	this->last=NULL;
}

template<typename E>
void Queue<E>::enqueue(E e1)
{
	//printf("J'entre avec difficulté %d\n", e1);
	if (this->first==NULL){
		this->first = new Maillon<E>(e1);
	}else if (this->last==NULL){
		this->last = new Maillon<E>(e1);
		this->first->setNext(this->last);
	}else {
		Maillon<E> *tmp = new Maillon<E>(e1);
		this->last->setNext(tmp);
		this->last=tmp;
	}
}

template<typename E>
E Queue<E>::dequeue()
{
	E toDequeue = this->first->getValue();

  	this->first = this->first->getNext();
  	if(this->first==NULL)
  		this->last=NULL;

  	return toDequeue;
}

template<typename E>
bool Queue<E>::isEmpty()
{
	if (this->first==NULL){
		return true;
	}else {
		return false;
	}

}
