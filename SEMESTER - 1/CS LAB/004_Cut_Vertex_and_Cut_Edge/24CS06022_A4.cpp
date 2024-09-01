#include <bits/stdc++.h>
using namespace std;

#define forn(i, s, n) for (int i = s; i < n; i++)

#define pb push_back
#define vi vector<int>
#define vb vector<bool>
#define vp vector<pair<int, int>>

#define endl '\n'

//--------------------- Code Start Here -------------------------

// adding edges
void addEdge(vector<vi> &g, int u, int v)
{
    g[u].pb(v);
    g[v].pb(u);
}

void dfs(vector<vi> &graph, vi &disc, vi &low, vb &isAP, vb &visited, vp &bridges, int time, int parent, int source)
{
    // mark source visited
    visited[source] = true;

    // updating disc & low values
    disc[source] = low[source] = ++time;

    int branches = 0;

    for (auto child : graph[source])
    {
        if (!visited[child])
        {
            branches++;
            dfs(graph, disc, low, isAP, visited, bridges, time, source, child);

            // update the low values of source
            low[source] = min(low[source], low[child]);

            // check if child node is AP ?
            if (parent != -1 && low[child] >= disc[source])
            {
                isAP[source] = true;
            }

            // check for cut edge
            if (low[child] > disc[source])
            {
                bridges.pb({source, child});
            }
        }
        else if (parent != child)
        {
            low[source] = min(low[source], disc[child]);
        }
    }

    // check for parent to be a root node
    if (parent == -1 && branches > 1)
    {
        isAP[source] = true;
    }
}
void solve()
{
    int n;
    cout << "Enter the number of vertex: ";
    cin >> n;

    vector<vi> graph(n);

    int e;
edgeIP:
    cout << "Enter the number of edges: ";
    cin >> e;
    int mxEdge = n * (n - 1) / 2;
    if (e > mxEdge)
    {
        cout << "Number of edges should be less than or equal to " << mxEdge << endl;
        goto edgeIP;
    }

    cout << "Enter edges(Format: u -> v): " << endl;

    forn(i, 0, e)
    {
        int u, v;
        cin >> u >> v;
        if (u >= n or v >= n or u < 0 or v < 0)
        {
            --i;
            cout << "Please enter correct vertices !!!" << endl;

            continue;
        }
        // add edge
        addEdge(graph, u, v);
    }

    // exploring graph to find out the cut vertex & cut edges
    vi disc(n, 0), low(n, 0);
    vb visited(n, false), isAP(n, false);
    vp bridges;
    forn(u, 0, n)
    {
        if (!visited[u])
        {
            dfs(graph, disc, low, isAP, visited, bridges, 0, -1, u);
        }
    }

    // printing all the cut vertex
    cout << "All cut-vertex in given graph : " << endl;
    forn(u, 0, n)
    {
        if (isAP[u])
        {
            if (!u)
            {
                cout << " ";
            }
            cout << u;
        }
    }

    cout << endl;
    cout << "Here, is all the Cut-edges: " << endl;
    for (auto edge : bridges)
    {
        cout << edge.first << " --- " << edge.second << endl;
    }
}
//--------------------- Code End Here ---------------------------
int main()
{

    //---------------------------------------------------------------

    //---------------------------------------------------------------

    int t = 1;
    // cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}
