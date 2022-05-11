#ifndef MAILLON_H
#define MAILLON_H

template <typename E>
class Maillon
{
  private :
    E value;
    Maillon<E> *next;

  public :
    Maillon(E v);
    Maillon(E v, Maillon<E>* n);
    bool isEmpty();
    E getValue();
    Maillon<E>* getNext();
    void setNext(Maillon<E> *n);

};

#endif //MAILLON_H
