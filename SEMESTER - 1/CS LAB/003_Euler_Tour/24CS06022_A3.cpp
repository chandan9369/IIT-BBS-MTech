#include <bits/stdc++.h>
using namespace std;

#define forn(i, s, n) for (int i = s; i < n; i++)
#define all(x) (x).begin(), (x).end()

#define F first
#define S second
#define pb push_back
#define pi pair<int, int>
#define vi vector<int>
#define vvi vector<vi>

#define endl '\n'
#define int long long

int V, E;
//--------------------- Code Start Here -------------------------

class Graph
{
    vvi graph;
    vi outDeg, inDeg;
    vi trail;
    int source;

public:
    Graph(int V)
    {
        graph.resize(V);
        outDeg.resize(V, 0);
        inDeg.resize(V, 0);
        trail.clear();
        source = 1;
    }
    void addEdge(int u, int v)
    {
        graph[u].pb(v);
        outDeg[u]++;
        inDeg[v]++;
    }
    bool isEulerien()
    {
        int startVertex = 0, endVertex = 0;
        forn(i, 0, V)
        {
            if (outDeg[i] - inDeg[i] == 1)
            {
                startVertex++;
                source = i;
            }
            else if (inDeg[i] - outDeg[i] == 1)
            {
                endVertex++;
            }
            else if (inDeg[i] != outDeg[i])
            {
                return false;
            }
        }

        // now, there should either be 2 odd degree vertex or zero
        return (startVertex == 1 && endVertex == 1) || (startVertex == 0 && endVertex == 0);
    }
    void findEulerTrail()
    {
        stack<int> path;
        path.push(source);

        while (!path.empty())
        {
            // pop the top node
            int u = path.top();

            // explore edges of u
            if (!graph[u].empty())
            {
                // remove the current edge & pick the next node
                int nextNode = graph[u].back();
                path.push(nextNode);
                graph[u].pop_back();
            }
            else
            {
                // there is no edges left
                trail.pb(u);
                // remove the top node
                path.pop();
            }
        }
        reverse(all(trail));
        printTrail();
    }

    void printTrail()
    {
        cout << "Eulerian Trail from graph: \n";
        forn(i, 0, trail.size())
        {
            if (i)
            {
                cout << " -> ";
            }
            cout << trail[i];
        }
        cout << endl;
    }
};

void solve()
{
    cout << "Enter the number of vertex: ";
    cin >> V;

    Graph graph(V + 1);

edgeIP:
    cout << "Enter the number of edges: ";
    cin >> E;
    int mxEdge = V * (V - 1);
    if (E > mxEdge)
    {
        cout << "Number of edges should be less than or equal to " << mxEdge << endl;
        goto edgeIP;
    }

    cout << "Please, Enter vertex starting from 1 !!!" << endl;
    cout << "Enter edges(Format: u -> v): " << endl;

    forn(i, 0, E)
    {
        int u, v;
        cin >> u >> v;
        if (u > V or v > V or u < 1 or v < 1)
        {
            --i;
            cout << "Please enter correct vertices !!!" << endl;

            continue;
        }
        // add edge
        graph.addEdge(u, v);
    }

    // Step - 1: Check whether Euler trail exist or not?
    // for this we need to check 2 condition
    // 1. All Vertices should have even degree or
    // 2. There should be only 2 odd degree vertices

    if (!graph.isEulerien())
    {
        cout << "There is no eulerian trail in graph." << endl;
        return;
    }

    graph.findEulerTrail();
}
//--------------------- Code End Here ---------------------------
signed main()
{
    //---------------------------------------------------------------

    int t = 1;
    // cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}
