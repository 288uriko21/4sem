#include "graph.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>
using namespace std;

// class graph {
// 	char* vertex;
// 	char ** adjacencyTable;

Graph::Graph()
{
	this->vertex = NULL;
	this->adjacencyTable = NULL;
	this->Vertexnumb = 0;

	vector<vector<int>> g_init(100), gr_init(100);
	vector<int> color_init(100);
	vector<vector<int>> graph_init(100, vector<int>(100));

	g = g_init;
	gr = gr_init;
	graph = graph_init;
	color = color_init;
}

Graph::~Graph() //!
{
	for (int i = 0; i < this->Vertexnumb; ++i)
		for (int j = 0; j < this->Vertexnumb; ++j)
			free((this->adjacencyTable)[i][j]);

	free(this->adjacencyTable);
}

ostream &operator<<(ostream &output, const Graph &A)
{
	output << "Vertex List: ";
	for (int i = 0; i < A.Vertexnumb; ++i)
		output << A.vertex[i] << " ";
	output << "\n";
	output << "Arcs List: ";

	for (int i = 0; i < A.Vertexnumb; ++i)
		for (int j = 0; j < A.Vertexnumb; ++j)
			if (A.adjacencyTable[i][j] != NULL)
				output << "(" << A.vertex[i] << ", " << A.adjacencyTable[i][j] << ", " << A.vertex[j] << ") ";

	output << "\n";

	return output;
}

void Graph::add(char vertex)
{
	this->Vertexnumb++;
	if (this->Vertexnumb == 1)
	{
		(this->vertex) = (char *)malloc(sizeof(char));
		(this->adjacencyTable) = (char ***)malloc(sizeof(char **));
		(this->adjacencyTable)[0] = (char **)malloc(sizeof(char *));
		(this->adjacencyTable)[0][0] = NULL;
	}
	else
	{
		(this->vertex) = (char *)realloc(this->vertex, (this->Vertexnumb) * sizeof(char));

		(this->adjacencyTable) = (char ***)realloc(this->adjacencyTable, (this->Vertexnumb) * sizeof(char **));
		(this->adjacencyTable)[(this->Vertexnumb) - 1] = (char **)malloc((this->Vertexnumb) * sizeof(char *));

		for (int i = 0; i < (this->Vertexnumb); ++i)
			(this->adjacencyTable)[(this->Vertexnumb) - 1][i] = NULL;

		for (int i = 0; i < (this->Vertexnumb) - 1; ++i)
		{
			(this->adjacencyTable)[i] = (char **)realloc((this->adjacencyTable)[i], (this->Vertexnumb) * sizeof(char *));
			(this->adjacencyTable)[i][(this->Vertexnumb) - 1] = NULL;
		}
	}
	this->vertex[Vertexnumb - 1] = vertex;
}

void Graph::add(char vertex1, const char *arc, char vertex2)
{
	int ind1 = -1, ind2 = -1, namelen = 0;

	this->FindVertexIndex(vertex1, vertex2, ind1, ind2);

	if (ind2 == -1 || ind1 == -1)
	{
		cout << "not all vertices exist\n";
		return;
	}

	for (; arc[namelen] != '\0'; ++namelen)
		;

	(this->adjacencyTable)[ind1][ind2] = (char *)malloc((namelen + 1) * sizeof(char));
	strcpy((this->adjacencyTable)[ind1][ind2], arc);
	(this->adjacencyTable)[ind1][ind2][namelen] = '\0';
}

void Graph::remove(char vertex1, char vertex2)
{
	int ind1 = -1, ind2 = -1;
	this->FindVertexIndex(vertex1, vertex2, ind1, ind2);

	if (ind2 == -1 || ind1 == -1)
	{
		cout << "not all vertices exist\n";
		return;
	}

	free((this->adjacencyTable)[ind1][ind2]);
	(this->adjacencyTable)[ind1][ind2] = NULL;
}

void Graph::FindVertexIndex(char a, char b, int &resa, int &resb) const
{
	int i = 0;

	while (i < (this->Vertexnumb))
	{
		if ((this->vertex)[i] == a)
			resa = i;

		if ((this->vertex)[i] == b)
			resb = i;
		++i;
	}
}

void Graph::remove(char vertex)
{
	int ind = -1, namelen = 0;
	this->FindVertexIndex(vertex, vertex, ind, ind);
	if (ind == -1)
	{
		cout << "vertix not exist\n";
		return;
	}

	// cout << "del: " << ind << endl;

	char *NewVertex = (char *)malloc(((this->Vertexnumb) - 1) * sizeof(char));
	for (int i = 0, dupi = 0; i < (this->Vertexnumb); ++i, ++dupi)
		if (vertex != (this->vertex)[i])
			NewVertex[dupi] = (this->vertex)[i];
		else
			--dupi;

	// cout << "1" << endl;
	char ***NewadjacencyTable = (char ***)malloc(((this->Vertexnumb) - 1) * sizeof(char **));

	for (int i = 0; i < ((this->Vertexnumb) - 1); ++i)
		NewadjacencyTable[i] = (char **)malloc(((this->Vertexnumb) - 1) * sizeof(char *));
	// cout << "2" << endl;
	//  cout << (this->adjacencyTable)[0][0][0] << endl;

	for (int i = 0, dupi = 0; i < (this->Vertexnumb); ++i, ++dupi)
	{
		if (i != ind)
		{
			for (int j = 0, dupj = 0; j < (this->Vertexnumb); ++j, ++dupj)
			{
				if (j != ind)
				{
					if ((this->adjacencyTable)[i][j] != NULL)
					{
						// cout << "2.1" << endl;
						for (; (this->adjacencyTable)[i][j][namelen] != '\0'; ++namelen)
							;
						// cout << "2.2" << endl;
						// cout << namelen << endl;

						NewadjacencyTable[dupi][dupj] = (char *)malloc((namelen + 1) * sizeof(char));
						strcpy(NewadjacencyTable[dupi][dupj], (this->adjacencyTable)[i][j]);
						NewadjacencyTable[dupi][dupj][namelen] = '\0';
						namelen = 0;
					}
					else
						NewadjacencyTable[dupi][dupj] = NULL;
					// cout << i << " " << j << endl;
				}
				else
					--dupj;
			}
		}
		else
			--dupi;
	}

	// cout << "3" << endl;

	for (int i = 0; i < this->Vertexnumb; ++i)
		for (int j = 0; j < this->Vertexnumb; ++j)
		{
			if ((adjacencyTable)[i][j] != NULL)
				free((this->adjacencyTable)[i][j]);
		}

	// cout << "4" << endl;

	free((this->vertex));
	free(this->adjacencyTable);

	// cout << "5" << endl;

	this->adjacencyTable = NewadjacencyTable;
	this->vertex = NewVertex;
	this->Vertexnumb = Vertexnumb - 1;
}

void Graph::OutgoingArcs(char vertex) const
{
	int ind = -1;
	this->FindVertexIndex(vertex, vertex, ind, ind);
	if (ind == -1)
	{
		cout << "vertix not exist\n";
		return;
	}
	cout << "Outgoing Arcs:" << endl;
	for (int i = 0; i < this->Vertexnumb; ++i)
	{
		if ((this->adjacencyTable)[ind][i] != NULL)
		{
			cout << (this->adjacencyTable)[ind][i] << endl;
		}
	}
	cout << endl;
}

bool Graph::reachability(char vertex1, char vertex2)
{
	int V = this->Vertexnumb;
	int ind1 = -1, ind2 = -1;
	this->FindVertexIndex(vertex1, vertex2, ind1, ind2);
	if (ind2 == -1 || ind1 == -1)
	{
		cout << "not all vertices exist\n";
		return false;
	}

	for (int i = 0; i < V; ++i)
		for (int j = 0; j < V; ++j)
			if (this->adjacencyTable[i][j] != NULL)
				graph[i][j] = 1;
			else
				graph[i][j] = 0;

	for (int i = 0; i < V; i++)
		for (int s = 0; s < V; s++)
			for (int t = 0; t < V; t++)
				if (graph[s][i] && graph[i][t])
					graph[s][t] = 1;

	if (graph[ind1][ind2] == 1)
		return true;
	else
		return false;
}

void Graph::dfs1(int v)
{
	used[v] = true;
	for (size_t i = 0; i < g[v].size(); ++i)
		if (!used[g[v][i]])
			dfs1(g[v][i]);
	order.push_back(v);
}

void Graph::dfs2(int v)
{
	used[v] = true;
	component.push_back(v);
	for (size_t i = 0; i < gr[v].size(); ++i)
		if (!used[gr[v][i]])
			dfs2(gr[v][i]);
}

void Graph::StrongConnect()
{

	cout << "Components of strong connectivity: " << endl;
	int n = this->Vertexnumb;

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			if (this->adjacencyTable[i][j] != NULL)
				graph[i][j] = 1;
			else
				graph[i][j] = 0;

	for (int a = 0; a < n; ++a)
		for (int b = 0; b < n; ++b)
		{
			if (graph[a][b] != 0)
			{
				g[a].push_back(b);
				gr[b].push_back(a);
			}
		}

	used.assign(n, false);
	for (int i = 0; i < n; ++i)
		if (!used[i])
			dfs1(i);
	used.assign(n, false);
	for (int i = 0; i < n; ++i)
	{
		int v = order[n - 1 - i];
		if (!used[v])
		{
			dfs2(v);
			for (size_t i = 0; i < component.size(); ++i)
				printf("%c ", this->vertex[component[i]]);
			cout << endl;
			component.clear();
		}
	}

	for (int a = 0; a < n; ++a)
	{
		g[a].clear();
		gr[a].clear();
	}
	order.clear();
	used.clear();
}

Graph::Graph(const Graph &G)
{
	int namelen = 0;
	this->Vertexnumb = G.Vertexnumb;

	(this->vertex) = (char *)malloc((this->Vertexnumb) * sizeof(char));

	(this->adjacencyTable) = (char ***)malloc((this->Vertexnumb) * sizeof(char **));

	for (int i = 0; i < (this->Vertexnumb); ++i)
	{
		(this->adjacencyTable)[i] = (char **)malloc((this->Vertexnumb) * sizeof(char *));
		this->vertex[i] = G.vertex[i];
	}

	for (int i = 0; i < (this->Vertexnumb); ++i)
		for (int j = 0; j < (this->Vertexnumb); ++j)
			if ((G.adjacencyTable)[i][j] != NULL)
			{

				for (; (G.adjacencyTable)[i][j][namelen] != '\0'; ++namelen)
					;

				this->adjacencyTable[i][j] = (char *)malloc((namelen + 1) * sizeof(char));
				strcpy(this->adjacencyTable[i][j], (G.adjacencyTable)[i][j]);
				this->adjacencyTable[i][j][namelen] = '\0';
				namelen = 0;
			}
			else
				this->adjacencyTable[i][j] = NULL;
}

Graph &operator+(const Graph &G, const Graph &D)
{
}

Graph &Graph::operator-(char vertex)
{
	this->remove(vertex);

	return *this;
}

Graph &Graph::operator-(char *arc)
{
	for (int i = 0; i < (this->Vertexnumb); ++i)
		for (int j = 0; j < (this->Vertexnumb); ++j)
			if ((this->adjacencyTable)[i][j] != NULL)
			{
				if (strcmp((adjacencyTable)[i][j], arc) == 0)
				{
					free((adjacencyTable)[i][j]);
					(adjacencyTable)[i][j] = NULL;
				}
			}

	return *this;
}

void Graph::dfs(int v, char vertex2, int beg)
{
	vector<int> colored;
	color[v] = 1;
	used.push_back(this->vertex[v]);
	// cout << this->vertex[v] << " " << endl;
	for (int i = 0; i < this->Vertexnumb; ++i)
	{
		// cout << *i << " ";
		if (graph[v][i] == 1)
			if (color[i] == 0)
			{
				colored.push_back(i);
				dfs(i, vertex2, beg);
			}
	}
	if (v == beg)
		color[v] = 2;
	else
		color[v] = 0;

	for (vector<int>::iterator i = colored.begin(); i != colored.end(); ++i)
		color[*i] = 0;
	if (vertex2 == this->vertex[v])
	{
		for (vector<char>::iterator i = used.begin(); i != used.end(); ++i)
			cout << *i << " ";
		cout << endl;
	}
	colored.clear();
	used.pop_back();
}

// void dfs (int v) {
// 	time_in[v] = dfs_timer++;
// 	color[v] = 1;
// 	for (vector<int>::iterator i=g[v].begin(); i!=g[v].end(); ++i)
// 		if (color[*i] == 0)
// 			dfs (*i);
// 	color[v] = 2;
// 	time_out[v] = dfs_timer++;
// }

void Graph::AllWaysFinder(char vertex1, char vertex2)
{
	int ind = -1;
	FindVertexIndex(vertex1, vertex1, ind, ind);
	if (ind == -1)
	{
		cout << "vertix not exist\n";
		return;
	}

	cout << "All ways: " << endl;
	int n = this->Vertexnumb;

	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			if (this->adjacencyTable[i][j] != NULL)
			{
				graph[i][j] = 1;
				// cout << "!";
			}
			else
				graph[i][j] = 0;
	dfs(ind, vertex2, ind);
	used.clear();
	color.clear();
	cout << endl;
}