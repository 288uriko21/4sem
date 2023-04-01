#pragma once


template <typename T>
class IterableObject
{
protected:
    int size;

public:
    virtual bool contains(T elem) = 0;
};