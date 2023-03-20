#include "graph.h"
#include <iostream>
using namespace std;

int main()
{
    Graph G;
    G.add('i');
    G.add('p');
    cout << G << '\n';

    //G.add('i',"yoi",'p');
    G.add('p',"poi",'i');
    cout << G << '\n';

    G.add('o');
    cout << G << '\n';
    G.add('r');
    cout << G << '\n';
    G.add('o',"poi",'r');
    cout << G << '\n';

    G.remove('p', 'i');
    cout << G << endl;

    G.remove('o');
    cout << G << endl;

    G.add('y');
    G.add('y',"yi",'p');
    G.add('y', "tr", 'i');
    G.add('p', "l",'i');
    G.add('p', "u", 'y');
    G.add('i', "v", 'y');
    G.add('i', "vevabga", 'r');
    G.add('r', "ga", 'y');

    cout << G << endl;

    ///cout << G.reachability('i', 'p') << ' ' << G.reachability('r', 'p') << endl;

    G.OutgoingArcs('y');

    G.add('a');
    G.add('b');
    G.add('a', "rtu", 'b');

    G.StrongConnect();
    cout << endl;

    cout << G.reachability('i', 'y') <<  " " << G.reachability('i', 'r') << G.reachability('r', 'p')<< G.reachability('b', 'a') << endl;

    Graph G2 = G;
    cout << G2 << endl;

    //G2 = G2 - "ui";

    cout << G << endl;

    G.AllWaysFinder('i', 'p');

    G.Cyclic();

    
    // WeightedGraph WG(G);

    // cout << WG;
    // cout << endl;

    // //cout << WG << endl;

    // WG.MSTfinderPrim();
    // cout << endl;

    // G.add('i', "try",'y');
    // WeightedGraph WG3(G);
    // WG3.MSTfinderPrim();
    // cout << endl;

    // Graph Gg;
    // Gg.add('a');
    // Gg.add('b');
    // Gg.add('c');
    // Gg.add('d');

    // Gg.add('a', "f", 'b');
    // Gg.add('a', "f", 'd');
    // Gg.add('b', "f", 'c');
    // Gg.add('d', "f", 'c');

    // WeightedGraph WG2(Gg);
    // cout << WG2 << endl;

    // //WG.MSTfinderPrim();
    // cout << endl;
    // WG2.Color();
    // cout << endl;

    // Graph G1;
    // G1.add('a');
    // G1.add('b');
    // G1.add('c');
    // G1.add('d');
    // G1.add('a', "uity",'b');
    // G1.add('d', "bt",'c');

    // Graph G5;
    // G5.add('a');
    // G5.add('l');
    // G5.add('a', "vsd", 'l');

    // cout << G5 + G1;
}