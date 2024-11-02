#include <bits/stdc++.h>
using namespace std;

#define forn(i, s, n) for (int i = s; i < n; i++)

#define vi vector<int>

#define endl '\n'
#define int long long

const int MOD = 1e9 + 7;
const int INF = LLONG_MAX >> 1;

//--------------------- BFS Code Start Here -------------------------
bool bfs(vector<vi> &g, int source, int dest, vi &parent)
{
    int n = g.size();
    vi visited(n, 0);
    queue<int> q;
    q.push(source);
    visited[source] = 1;
    parent[source] = -1;

    while (!q.empty())
    {
        int node = q.front();
        q.pop();

        // if destination node
        if (node == dest)
        {
            return true;
        }

        forn(v, 0, n)
        {
            if (visited[v] == 0 && g[node][v] > 0)
            {
                visited[v] = 1;
                parent[v] = node;
                q.push(v);
            }
        }
    }
    return false;
}
// ---------------- BFS Code End --------------------
// ---------------- MAX Flow Code Start Here --------------------
int maxNetFlow(vector<vi> &net, int s, int t)
{
    // Algo :
    // 1. find s-t path
    // 2.1 if found any path then find bottleneck capacity(min cap)
    // 2.2 augment the path means update the path
    // 2.2.1 decrease the flow for forward edge
    // 2.2.2 increase the flow for backward edge

    int n = net.size();
    // Create copy of graph to work on
    vector<vi> residual(n, vi(n, 0));

    // copying
    forn(i, 0, n)
    {
        forn(j, 0, n)
        {
            residual[i][j] = net[i][j];
        }
    }

    vi parent(n);
    int v, u;
    // find s-t path
    int maxFlow = 0;
    while (bfs(residual, s, t, parent))
    {
        // now, finding the bottleneck capacity of the s-t path
        int bottleneck = 1e8;
        v = t;         // sink node
        while (v != s) // untill source node
        {
            u = parent[v];
            bottleneck = min(bottleneck, residual[u][v]);
            v = u;
        }

        // now, augmenting the s-t path
        v = t;
        while (v != s)
        {
            u = parent[v];
            // decrease forward edge
            residual[u][v] -= bottleneck;
            // increase backward edge
            residual[v][u] += bottleneck;
            v = u;
        }

        // updating the total flow
        maxFlow += bottleneck;
    }
    return maxFlow;
}
// ---------------- MAX Flow Code End --------------------
void solve()
{
    int m;
    cout << "Enter the number of types of toys: ";
    cin >> m;
    int n;
    cout << "Enter the number of friends: ";
    cin >> n;

    n++; // adding sumanna as a source node
    vector<vi> data(n, vi(m, 0));
    cout << "Enter the toys of every person: \n";
    forn(i, 0, n)
    {
        int a;
        forn(j, 0, m)
        {
            cin >> a;
            data[i][j] = a;
        }
    }
    cout << "\nAssume Source node: 0(Sumana) & Dummy sink node: " << n + m << endl;

    // creating network flow for given problem
    vector<vi> graph(m + n + 1, vi(m + n + 1, 0));

    // setting for source node
    forn(i, 1, m + 1)
    {
        graph[0][i] = data[0][i - 1];
    }

    // setting for friends
    forn(i, 1, n)
    {
        forn(j, 1, m + 1)
        {
            if (data[i][j - 1] > 1)
                graph[m + i][j] = data[i][j - 1] - 1;
        }
    }

    // setting up the toy nodes
    forn(i, 1, m + 1)
    {
        graph[i][m + n] = 1;
    }

    // setting up the exchange option for sumanna
    forn(i, 1, n)
    {
        forn(j, 1, m + 1)
        {
            if (data[i][j - 1] == 0)
            {
                graph[j][i + m] = 1;
            }
        }
    }

    cout << "Maximum number of Disney toys Sumana can collect: " << maxNetFlow(graph, 0, n + m) << endl;
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
