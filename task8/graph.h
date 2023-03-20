#pragma once

#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>

using namespace std;

class Graph
{
protected:
    char *vertex;
    char ***adjacencyTable;
    int Vertexnumb;

public:
    Graph(); //

    Graph(const Graph &G); //

    void add(char vertex); //

    void add(char vertex1, const char *arc, char vertex2); //

    void remove(char vertex); //

    void remove(char vertex1, char vertex2); //

    ~Graph(); //

    friend ostream &operator<<(ostream &output, const Graph &G); //

    friend Graph operator+(const Graph &G, const Graph &D); //                                       ?????????????

    Graph &operator-(char vertex); ///

    Graph &operator-(char *arc); ///

    void operator delete[](void *a); // синоним удаления вершины                 ?????????

    bool reachability(char vertex1, char vertex2); ///

    void StrongConnect();

    void OutgoingArcs(char vertex) const; ///

    void FindVertexIndex(char a, char b, int &resa, int &resb) const; ///

    void AllWaysFinder(char vertex1, char vertex2);

    void Cyclic();

private:
    vector<vector<int>> g, gr;
    vector<vector<int>> graph;
    vector<char> used;
    vector<int> order, component;
    vector<int> color;
    vector<char> way;
    vector<char> cl;
    vector<int> p;
    int cycle_st, cycle_end;

    void dfs2(int v);

    void dfs1(int v);

    void dfs(int v, char vertex2, int beg);

    bool Cdfs(int v);
};

struct Arc
{
    char *info;
    int weight;

    Arc(char *info, int weight);

    Arc(const Arc &G);

    Arc & operator = (const Arc & B);

    ~Arc();
};

class WeightedGraph : public Graph
{
protected:
    Arc **adjacencyTablePro;

public:
    WeightedGraph(const Graph &G);

    void MSTfinderPrim();

    void Color();

    ~WeightedGraph();

    friend ostream &operator<<(ostream &output, const WeightedGraph &G);
};
