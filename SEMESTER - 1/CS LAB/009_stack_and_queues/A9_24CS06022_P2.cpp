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

int trap1(vi &hist)
{
    int n = hist.size();
    stack<int> st;
    vi lmax(n), rmax(n);
    // lmax construction
    forn(i, 0, n)
    {
        while (!st.empty() && st.top() <= hist[i])
        {
            st.pop();
        }

        lmax[i] = (st.empty() ? hist[i] : st.top());

        // rmax current element
        if (st.empty())
            st.push(hist[i]);
    }

    // clear stack
    while (!st.empty())
        st.pop();
    // rmax construction
    for (int i = n - 1; i >= 0; i--)
    {
        while (!st.empty() && st.top() <= hist[i])
        {
            st.pop();
        }

        rmax[i] = (st.empty() ? hist[i] : st.top());

        // irmaxrt current element
        if (st.empty())
            st.push(hist[i]);
    }

    int amount = 0;
    forn(i, 0, n)
    {
        amount += min(rmax[i], lmax[i]) - hist[i];
    }
    return amount;
}
int trap2(vi &hist)
{
    int l = 0;
    int r = hist.size() - 1;
    int lmax = hist[l];
    int rmax = hist[r];
    int water = 0;

    while (l < r)
    {
        // operate over left value as it is smaller than right one
        if (lmax < rmax)
        {
            l++;
            lmax = max(lmax, hist[l]);
            water += lmax - hist[l];
        }
        else
        // operate over right value as it is smaller than left one
        {
            r--;
            rmax = max(rmax, hist[r]);
            water += rmax - hist[r];
        }
    }

    return water;
}
void solve()
{
    int n;
    cout << "Enter the number of bars: ";
    cin >> n;
    vi arr(n);
    cout << "Enter heights of " << n << " bars: " << endl;

    forn(i, 0, n)
    {
        cin >> arr[i];
    }
    cout << "Amount of rain water trapped in a given histogram: " << endl;

    cout << "Using stack based method: " << trap1(arr) << endl;
    cout << "Using two-pointer based method: " << trap2(arr) << endl;
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
