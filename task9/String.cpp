#include "String.h"

#include <iostream>
using namespace std;

ostream &operator<<(ostream &output, const String &L)
{
    for (int i = 0; i < L.size; ++i)
        output << L.content[i];
    output << endl;
    return output;
}

String::String(const char *text)
{
    int len = 0;
    for (; text[len] != '\0'; ++len)
        ;

    content = (char *)malloc((len + 1) * (sizeof(char)));
    size = len;
}

int String::Getsize()
{
    return size;
}


bool String::contains(const char *text)
{
    int m = 0;
    for (; text[m] != '\0'; ++m)
        ;
    int n = size;

    // если шаблон это пустая строка
    if (n == 0)
    {
        cout << "The pattern occurs with shift 0";
        return;
    }

    // если длина текста меньше длины шаблона
    if (m < n)
    {
        cout << "Pattern not found";
        return;
    }

    // next[i] сохраняет индекс следующего лучшего частичного совпадения
    int next[n + 1];

    for (int i = 0; i < n + 1; i++)
    {
        next[i] = 0;
    }

    for (int i = 1; i < n; i++)
    {
        int j = next[i + 1];

        while (j > 0 && content[j] != content[i])
        {
            j = next[j];
        }

        if (j > 0 || content[j] == content[i])
        {
            next[i + 1] = j + 1;
        }
    }

    for (int i = 0, j = 0; i < m; i++)
    {
        if (text[i] == content[j])
        {
            if (++j == n)
            {
                cout << "The pattern occurs with shift " << i - j + 1 << endl;
            }
        }
        else if (j > 0)
        {
            j = next[j];
            i--; // так как `i` будет увеличен на следующей итерации
        }
    }
}

String::~String()
{
    free(content);
};