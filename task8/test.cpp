#include "graph.h" 
#include <iostream>
using namespace std;


int main() {
    Graph G;
    G.add('i');
    G.add('p');
    cout << G << '\n';

    G.add('i',"yoi",'p');
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

    G.StrongConnect();
    cout << endl;

    cout << G.reachability('i', 'y') <<  " " << G.reachability('i', 'r') << endl;

    Graph G2 = G;
    cout << G2 << endl;

    //G2 = G2 - "ui";

    cout << G << endl;

    G.AllWaysFinder('i', 'p');

}