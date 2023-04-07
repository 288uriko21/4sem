#pragma once
#include "IterableObject.h"
using namespace std;

template <typename T>
class Iterator
{
    T *elem;

public:
    Iterator(T &el)
    {
        elem = &el;
    }

    void operator=(Iterator <T> get)
    {
        this->elem = get.elem;
    }

    T &operator*()
    {
        return *elem;
    }

    T &operator[](int n)
    {
        return *(elem + n);
    }

    void operator++()
    {
        ++elem;
    }

    void operator++(int)
    {
        ++elem;
    }

    void operator--()
    {
        ++elem;
    }

    void operator--(int)
    {
        ++elem;
    }

    void operator+=(int a)
    {
        elem += a;
    }

    int operator-(Iterator <T> a)
    {
        return int((this->elem) - (a.elem));
    }

    void operator-=(int a)
    {
        elem -= a;
    }

};
