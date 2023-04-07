#pragma once


template <typename T>
class IterableObject
{
//protected:
    

public:
    int size;
    virtual bool contains(T elem) = 0;
};