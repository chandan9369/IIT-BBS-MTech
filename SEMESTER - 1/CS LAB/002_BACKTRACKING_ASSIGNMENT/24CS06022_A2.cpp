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

//--------------------- Code Start Here -------------------------

// Global Properties
int n;
int space = -1;
int maxDepth = 15;

// Helper functions used
int mergeArr(vi &arr, int lo, int mid, int hi)
{
    vi temp;
    int i = lo;
    int j = mid + 1;
    int cnt = 0;

    while (i <= mid && j <= hi)
    {
        if (arr[i] <= arr[j])
        {
            temp.pb(arr[i]);
            i++;
        }
        else
        {
            cnt += mid - i + 1;
            temp.pb(arr[j]);
            j++;
        }
    }

    // push all remaining elements to temp
    while (i <= mid)
    {
        temp.pb(arr[i++]);
    }
    while (j <= hi)
    {
        temp.pb(arr[j++]);
    }

    // transfering all elements from temporary to arr
    for (int k = lo; k <= hi; k++)
    {
        arr[k] = temp[k - lo];
    }

    return cnt;
}

int inversions(vi &arr, int lo, int hi)
{
    if (lo < hi)
    {
        int mid = (lo + hi) >> 1;
        int val = inversions(arr, lo, mid);
        val += inversions(arr, mid + 1, hi);
        val += mergeArr(arr, lo, mid, hi);

        return val;
    }
    return 0;
}

bool isSolvable(vector<vi> &grid)
{
    vi arr;
    for (auto &row : grid)
    {
        for (auto &e : row)
        {
            if (e != 0)
                arr.pb(e);
        }
    }

    int inv_count = inversions(arr, 0, arr.size() - 1);

    // erase arr
    arr.clear();
    cout << "Inversions: " << inv_count << endl;
    // Check if system is solvable or not
    if (n % 2 == 0)
        return (inv_count + space) & 1;
    else
        return inv_count % 2 == 0;
}

void showGrid(string grid)
{
    forn(i, 0, n)
    {
        forn(j, 0, n)
        {
            if (grid[i * n + j] == '0')
                cout << "  ";
            else
                cout << grid[i * n + j] << " ";
        }
        cout << endl;
    }
}
bool solvePuzzle(string curr, string goal)
{
    stack<pair<string, vector<string>>> stateStack;
    set<string> visited;

    // push the initial configuration & initial path
    stateStack.push({curr, {curr}});
    visited.insert(curr);

    while (!stateStack.empty())
    {
        // pop the current state from the top of the stack
        auto [currState, currPath] = stateStack.top();
        stateStack.pop(); // pop after extraction

        // check if we reached the goal state
        if (currState == goal)
        {
            cout << "\nNumber of steps needed to reach goal state: " << currPath.size() - 1 << endl;

            forn(i, 0, currPath.size())
            {
                cout << "Step " << i << ":\n";
                showGrid(currPath[i]);
                cout << endl;
            }
            return true;
        }

        // check if we cross the max depth
        if (currPath.size() - 1 >= maxDepth)
        {
            continue; // continue to the next state, don't push further
        }

        // find the index of the empty slot
        int idx = currState.find('0');
        if (idx == string::npos)
        {
            cout << "\nInvalid State !!" << endl;
            return false;
        }

        // explore all possible moves
        vector<int> validMoves;
        if (idx >= n)
            validMoves.pb(-n); // up
        if (idx < n * (n - 1))
            validMoves.pb(n); // down
        if (idx % n != 0)
            validMoves.pb(-1); // left
        if ((idx + 1) % n != 0)
            validMoves.pb(1); // right

        for (auto move : validMoves)
        {
            string nState = currState;
            swap(nState[idx], nState[idx + move]);

            if (visited.find(nState) == visited.end())
            {
                vector<string> newPath = currPath;
                newPath.pb(nState);
                visited.insert(nState);
                stateStack.push({nState, newPath});
            }
        }
    }

    cout << "\n!!! Puzzle can't be solved with given limits !!!" << endl;
    return false;
}

void solve()
{
    cout << "\nEnter the dimension of Grid: ";
    cin >> n;

    if (n > 3)
    {
        cout << "\nSystem not compatible to solve/check grid with width/height > 3 " << endl;
        return;
    }

    vector<vi> grid(n, vi(n));

    cout << "\n!!! Please, enter '0' for empty cell !!!\n";
    cout << "\nEnter initial state: " << endl;
    forn(i, 0, n)
    {
        forn(j, 0, n)
        {
            cin >> grid[i][j];
            if (grid[i][j] == 0)
                space = i;
        }
    }

    if (!isSolvable(grid))
    {
        cout << "\n!!! Given puzzle is not solvable !!!\n";
        return;
    }

    cout << "\n!!! Given puzzle is solvable !!!\n";

    cout << "Now, Enter goal state: \n";
    vector<vi> goal(n, vi(n));

    forn(i, 0, n)
    {
        forn(j, 0, n)
        {
            cin >> goal[i][j];
        }
    }

    string start = "";
    string end = "";
    forn(i, 0, n)
    {
        forn(j, 0, n)
        {
            start += to_string(grid[i][j]);
            end += to_string(goal[i][j]);
        }
    }

    while (true)
    {
        cout << "Enter the max depth: ";
        cin >> maxDepth;
        bool val = solvePuzzle(start, end);
        if (!val)
        {
            cout << "\nNo solution found within the current depth limit. Consider increasing the limit.\n";
            cout << "Please increase the limit.\n";
        }
        else
        {
            break;
        }
    }
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