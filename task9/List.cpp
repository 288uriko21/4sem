#include "List.h"
#include <iostream>
using namespace std;

#define allocSize 10

// template <typename T>
// void List<T>::push_back(const T elem)
// {
//     ++size;
//     if (size > sizeAll * allocSize)
//     {
//         ++sizeAll;
//         content = (T *)realloc(content, sizeAll * allocSize);
//     }
//     content[size - 1] = elem;
// }

// template <typename T>
// List<T>::List()
// {
//     content = malloc(allocSize * (sizeof(T)));
//     size = 0;
//     sizeAll = 1;
// }

// template <typename T>
// ostream &operator<<(ostream &output, const List<T> &L)
// {
//     for (int i = 0; i < L.size; ++i)
//         output << L.content[i] << ' ';
//     output << endl;
//     return output;
// }