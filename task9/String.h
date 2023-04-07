#pragma once
#include "IterableObject.h"
#include "Iterator.h"
#include <string.h>
#include <iostream>
#include <stdio.h>
using namespace std;


class String : virtual public IterableObject<const char *>
{

    char *content;

public:
    String()
    {
        size = 0;
        content = NULL;
    }

    String(const char *text)
    {
        int len = 0;
        for (; text[len] != '\0'; ++len)
            ;

        content = (char *)malloc(len * sizeof(char));
        strcpy(content, text);
        size = len;
        // cout << len<< " ";
    };

    String(const String &B)
    {
        this->size = B.size;
        this->content = (char *)malloc((this->size + 1) * sizeof(char));
        strcpy(this->content, B.content);

    }; ///

    String operator+(const String &B)
    {
        String res = *this;
        res.size += B.size;
        res.content = (char *)realloc(res.content, ((res.size) + 1) * sizeof(char));
        strcpy((res.content) + ((res.size) - B.size), B.content);
        // cout << this->content << " ";
        return res;
    }; ///

    void operator=(const String &B)
    {
        this->size = B.size;
        this->content = (char *)realloc(this->content, (B.size + 1) * sizeof(char));
        strcpy(this->content, B.content);
    };

    bool operator>(const String &B)
    {
        for (int i = 0; (i < this->size) && (i < B.size); ++i)
        {
            if ((this->content)[i] > B.content[i])
                return true;
            if ((this->content)[i] < B.content[i])
                return false;
        }
        if (this->size < B.size)
            return true;
        else
            return false;
    }; ///

    bool operator<(const String &B)
    {
        return !(*this > B);
    }; ///

    bool operator==(const String &B)
    {
        if (B.size != this->size)
            return false;
        for (int i = 0; i < this->size; ++i)
            if ((this->content)[i] != B.content[i])
                return false;
        return true;
    }

    bool operator!=(const String &B)
    {
        return !(*this == B);
    }

    bool operator>=(const String &B)
    {
        return ((*this > B) || (*this == B));
    };

    bool operator<=(const String &B)
    {
        return ((*this < B) || (*this == B));
    }

    void operator+=(const String &B)
    {
        this->size += B.size;
        this->content = (char *)realloc(this->content, ((this->size) + 1) * sizeof(char));
        strcpy((this->content) + ((this->size) - B.size), B.content);
    }

    int Getsize()
    {
        return size;
    };

    friend ostream &operator<<(ostream &output, const String &L)
    {
        for (int i = 0; i < L.size; ++i)
            output << L.content[i];
        output << endl;
        return output;
    };

    bool contains(const char *elem)
    {
        String part(elem);
        int m = size;
        int n = part.size;

        if (n == 0)
        {
            return true;
        }

        if (m < n)
        {
            return false;
        }

        int next[n + 1];

        for (int i = 0; i < n + 1; i++)
        {
            next[i] = 0;
        }

        for (int i = 1; i < n; i++)
        {
            int j = next[i + 1];

            while (j > 0 && part.content[j] != part.content[i])
            {
                j = next[j];
            }

            if (j > 0 || part.content[j] == part.content[i])
            {
                next[i + 1] = j + 1;
            }
        }

        for (int i = 0, j = 0; i < m; i++)
        {
            if ((this->content)[i] == part.content[j])
            {
                if (++j == n)
                {
                    return true;
                }
            }
            else if (j > 0)
            {
                j = next[j];
                i--;
            }
        }
    }

    void clear()
    {
        free(content);
        size = 0;
    }

    //template <typename T>
    Iterator<char> srbegin()
    {
        Iterator<char> iter((this->content)[0]);
        
        return iter;
    };

    ~String()
    {
        free(content);
    };
};