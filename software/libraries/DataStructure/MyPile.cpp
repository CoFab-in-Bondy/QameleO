#include "MyPile.h"

#include "Maillon.h"
#include "Maillon.cpp"
#include <stdlib.h>

template <typename E>
Pile<E>::Pile()
{
  this->first=NULL;
}

template <typename E>
void Pile<E>::push(E e1)
{
  if(this->first==NULL){
    this->first = new Maillon<E>(e1);
  }else {
    this->first =  new Maillon<E>(e1, this->first);
  }
}

template <typename E>
E Pile<E>::pop()
{
  E toPop = this->first->getValue();

  this->first = this->first->getNext();

  return toPop;
  //Il faut tester la condition, mais je ne sais pas s'il vaut mieux le mettre ici ou bien dans le code qui appelle cette méthode
  // if (this->isEmpty()){
  //
  // }else {
  //   //la pile est vide, il n'y a pas d'élément qui puisse être enlevé
  //   return NULL;
  // }
}

template <typename E>
bool Pile<E>::isEmpty()
{
  if (this->first==NULL){
    return true;
  } else {
    return false;
  }
}
