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

int largestArea(vi &hist, int n)
{
    stack<int> st;
    vi pse(n + 1), nse(n + 1);
    // pse construction
    forn(i, 1, n + 1)
    {
        while (!st.empty() && hist[st.top()] >= hist[i])
        {
            st.pop();
        }

        pse[i] = (st.empty() ? 0 : st.top());

        // insert current element
        st.push(i);
    }

    // clear stack
    while (!st.empty())
        st.pop();
    // nse construction
    for (int i = n; i >= 1; i--)
    {
        while (!st.empty() && hist[st.top()] >= hist[i])
        {
            st.pop();
        }

        nse[i] = (st.empty() ? n + 1 : st.top());

        // insert current element
        st.push(i);
    }

    int maxArea = 0;
    forn(i, 1, n + 1)
    {
        maxArea = max(maxArea, (nse[i] - pse[i] - 1) * hist[i]);
    }
    return maxArea;
}
void solve()
{
    int n;
    cout << "Enter the number of bars: ";
    cin >> n;
    vi arr(n + 1);
    cout << "Enter heights of " << n << " bars: " << endl;

    forn(i, 1, n + 1)
    {
        cin >> arr[i];
    }
    cout << "Largest area of rectangle in a given histogram: " << largestArea(arr, n) << endl;
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
