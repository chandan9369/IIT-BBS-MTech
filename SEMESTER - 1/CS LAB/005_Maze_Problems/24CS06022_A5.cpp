#include <bits/stdc++.h>
using namespace std;

#define forn(i, s, n) for (int i = s; i < n; i++)
#define all(x) (x).begin(), (x).end()

#define F first
#define S second
#define pb push_back
#define pi pair<int, int>
#define vi vector<int>

#define endl '\n'
#define int long long

const int MOD = 1e9 + 7;
const int INF = LLONG_MAX >> 1;

//--------------------- Code Start Here -------------------------
int dx[4] = {1, -1, 0, 0};
int dy[4] = {0, 0, -1, 1};
int n, m;

bool isCross(int r, int c)
{
    return (r < 0 || r >= n || c < 0 || c >= m);
}

// minimum steps required without obstacle break up
int minStepsOA(vector<vi> &grid, int n, int m)
{
    if (grid[0][0] == 1 or grid[n - 1][m - 1] == 1)
    {
        return INF;
    }

    queue<pi> q;

    q.push({0, 0});

    vector<vi> dp(n, vi(m, INF));

    dp[0][0] = 0;

    while (!q.empty())
    {
        auto node = q.front();
        int x = node.F;
        int y = node.S;

        q.pop();

        forn(i, 0, 4)
        {
            int xx = x + dx[i];
            int yy = y + dy[i];
            if (!isCross(xx, yy) && grid[xx][yy] == 0 && dp[xx][yy] == INF)
            {
                dp[xx][yy] = dp[x][y] + 1;
                q.push({xx, yy});
            }
        }
    }
    return dp[n - 1][m - 1];
}

// minimum steps required with obstacle break up
int minStepsWithRemoval(vector<vi> &grid)
{
    priority_queue<vi, vector<vi>, greater<vi>> q;
    vector<vector<pi>> dp(n, vector<pi>(m, {INF, INF}));

    q.push({0, grid[0][0], 0, 0});
    dp[0][0] = {0, grid[0][0]};

    while (!q.empty())
    {
        auto node = q.top();
        int steps = node[0], obsBreak = node[1], x = node[2], y = node[3];
        q.pop();

        if (steps > dp[x][y].F || (steps == dp[x][y].F && obsBreak > dp[x][y].S))
        {
            continue;
        }

        forn(i, 0, 4)
        {
            int xx = x + dx[i];
            int yy = y + dy[i];

            if (!isCross(xx, yy))
            {
                int nObsBreak = obsBreak + grid[xx][yy];
                int nSteps = steps + 1;

                if (nSteps < dp[xx][yy].F || (nSteps == dp[xx][yy].F && nObsBreak < dp[xx][yy].S))
                {
                    dp[xx][yy] = {nSteps, nObsBreak};
                    q.push({nSteps, nObsBreak, xx, yy});
                }
            }
        }
    }
    return dp[n - 1][m - 1].S;
}

// minimum steps required with obstacle break up with atmost K breaks
int minStepsWithK(vector<vi> &grid, int K)
{
    priority_queue<vi, vector<vi>, greater<vi>> q;
    vector<vector<pi>> dp(n, vector<pi>(m, {INF, INF}));

    q.push({0, grid[0][0], 0, 0});
    dp[0][0] = {0, grid[0][0]};

    while (!q.empty())
    {
        auto node = q.top();
        int steps = node[0], obsBreak = node[1], x = node[2], y = node[3];
        q.pop();

        if (obsBreak > K)
        {
            continue;
        }
        if (steps > dp[x][y].F || (steps == dp[x][y].F && obsBreak > dp[x][y].S))
        {
            continue;
        }

        forn(i, 0, 4)
        {
            int xx = x + dx[i];
            int yy = y + dy[i];

            if (!isCross(xx, yy))
            {
                int nObsBreak = obsBreak + grid[xx][yy];
                int nSteps = steps + 1;

                if (nSteps < dp[xx][yy].F || (nSteps == dp[xx][yy].F && nObsBreak < dp[xx][yy].S))
                {
                    dp[xx][yy] = {nSteps, nObsBreak};
                    q.push({nSteps, nObsBreak, xx, yy});
                }
            }
        }
    }
    return dp[n - 1][m - 1].F;
}

void solve()
{
    cout << "============= Enter the number of rows & columns =============\n";
    cout << "rows : ";
    cin >> n;
    cout << "columns : ";
    cin >> m;
    vector<vi> grid(n, vi(m));
    cout << "============= Enter the 0(empty) or 1(obstacle) cell =============\n";
    forn(i, 0, n)
    {
        forn(j, 0, m)
        {
            cin >> grid[i][j];
        }
    }

    cout << "\n--------------------- PART - 1 ---------------------------" << endl;

    cout << "Minimum number of steps needed to reach from the upper left corner (0,0) to the lower right corner (" << n - 1 << "," << m - 1 << ") :\n";

    int val1 = minStepsOA(grid, n, m);
    if (val1 == INF)
        cout << "Can't reach with given state !!!" << endl;
    else
        cout << "Number of Steps  : " << val1 << endl;

    cout << "\n--------------------- PART - 2 ---------------------------" << endl;

    cout << "Minimum number of steps & obstacle break needed to reach from the upper left corner (0,0) to the lower right corner (" << n - 1 << "," << m - 1 << ") :\n";

    int val2 = minStepsWithRemoval(grid);
    if (val2 == INF)
        cout << "Can't reach with given state !!!" << endl;
    else
        cout << "Number of Obstacle Removed : " << val2 << endl;

    cout << "\n--------------------- PART - 3 ---------------------------" << endl;
    int K;
    cout << "\nEnter maxObstacle to break: ";
    cin >> K;

    cout << "Minimum number of steps needed to reach from the upper left corner (0,0) to the lower right corner (" << n - 1 << "," << m - 1 << ") with atmost " << K << " breaks :\n";
    int val3 = minStepsWithK(grid, K);
    if (val3 == INF)
        cout << "Can't reach with given state !!!" << endl;
    else
        cout << "Number of Steps : " << val3 << endl;
}

//--------------------- Code End Here ---------------------------
signed main()
{
    int t = 1;
    while (t--)
    {
        solve();
    }

    return 0;
}
