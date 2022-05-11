#ifndef PILE_H
#define PILE_H

#include "Maillon.h"

template <typename E>
class Pile
{
    private :
      Maillon<E> *first;

    public :
      Pile();
      void push(E e1);
      E pop();
      bool isEmpty();


};

#endif //PILE_H
