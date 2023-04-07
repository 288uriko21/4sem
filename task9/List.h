#pragma once
#include "IterableObject.h"
#include "Iterator.h"

#include <iostream>
using namespace std;

#define allocSize 10

template <typename T>
class List : virtual public IterableObject<T>
{

    T *content;
    int sizeAll;

public:
    List()
    {
        content = (T *)malloc(allocSize * (sizeof(T)));
        this->size = 0;
        sizeAll = 1;
    };

    bool contains(T elem)
    {
        for (int i = 0; i < this->size; ++i)
            if (elem == content[i])
                return true;
        return false;
    };

    void push_back(T elem)
    {
        ++(this->size);
        if (this->size > sizeAll * allocSize)
        {
            ++sizeAll;
            content = (T *)realloc(content, sizeAll * allocSize);
        }
        content[this->size - 1] = elem;
    };

    void pop_back()
    {
        --(this->size);
    };

    int Getsize()
    {
        return this->size;
    };

    void erase(T elem)
    {
        int i;

        for (i = 0; i < this->size; ++i)
            if (elem == content[i])
                break;

        for (int j = i + 1; j < this->size; ++j)
            content[j - 1] = content[j];

        --(this->size);
    };

    friend ostream &operator<<(ostream &output, const List<T> &L)
    {
        for (int i = 0; i < L.size; ++i)
            output << L.content[i] << ' ';
        output << endl;
        return output;
    };

    Iterator<T> srbegin()
    {
        Iterator<T> iter((this->content)[0]);

        return iter;
    };

    ~List()
    {
        free(content);
    };
};