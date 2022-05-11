#include "Maillon.h"

#include <stdlib.h>

template <typename E>
Maillon<E>::Maillon(E v)
{
  this->value=v;
  this->next=NULL;
}

template <typename E>
Maillon<E>::Maillon(E v, Maillon<E>* n)
{
  this->value=v;
  this->next=n;
}

template <typename E>
bool Maillon<E>::isEmpty()
{
  if (this->value!=0){
    return false;
  }else {
    return true;
  }
}

template <typename E>
E Maillon<E>::getValue()
{
  return this->value;
}

template <typename E>
Maillon<E>* Maillon<E>::getNext()
{
  return this->next;
}

template <typename E>
void Maillon<E>::setNext(Maillon<E> *n)
{
  this->next=n;
}
