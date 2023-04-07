// #include "IterableObject.h"
#include "List.h"
#include "String.h"
#include "Iterator.h"
#include <iostream>
using namespace std;

int main()
{
    List<int> mylst;
    mylst.push_back(5);
    mylst.push_back(7);
    mylst.push_back(9);
    mylst.push_back(8);
    mylst.push_back(6);
    mylst.push_back(4);
    mylst.push_back(0);
    cout << mylst;
    mylst.pop_back();
    cout << mylst;


    String s1("bchvjjjs");
    String s2("adcgtyechb");
    String s3 = s2 + s1;
    cout << (s2 + s1);
    cout << s3;
    cout << s2;
    cout << (s2 > s1) << endl;
    s1 += s2;
    cout << s1;

    cout << "Contains: " << s2.contains("tyechb") << endl;
    cout << "Contains: " << s2.contains("tyec") << endl;
    cout << "Contains: " << s2.contains("adcg") << endl;
    cout << "Contains: " << s2.contains("tyr") << endl;

    Iterator<char> i = s1.srbegin();

    ++i;
    cout << *i << endl;
    i--;
    cout << *i << endl;

    Iterator<int> iter = mylst.srbegin();
    iter += 3;
    cout << *iter << endl;

    Iterator<int> iter2 = mylst.srbegin();

    // cout << iter - iter2 << endl;
    iter2 = iter;
    cout << *iter << endl;

    Iterator<int> iter3 = mylst.srbegin();

    cout << iter2 - iter3 << endl;

    mylst.erase(8);

    cout << mylst;

    cout << "Contains: " << mylst.contains(5) << " " << mylst.contains(0);

    return 0;
}