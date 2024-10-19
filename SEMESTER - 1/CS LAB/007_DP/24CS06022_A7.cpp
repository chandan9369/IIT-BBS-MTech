#include <bits/stdc++.h>
using namespace std;

#define forn(i, s, n) for (int i = s; i < n; i++)
#define all(x) (x).begin(), (x).end()

#define F first
#define S second
#define pb push_back
#define vi vector<int>
#define vvi vector<vi>

#define endl '\n'
#define int long long

const int MOD = 1e9 + 7;
const int INF = LLONG_MAX >> 1;

//--------------------- Code Start Here -------------------------
int convertToB(string A, string B)
{
    int m = A.length(), n = B.length();
    vvi edit(m + 1, vi(n + 1, INT_MAX));

    // base cases
    edit[0][0] = 0;

    // inserting B character in empty A
    forn(k, 1, n + 1)
    {
        edit[0][k] = 2 + k;
    }

    // deleting character of A to convert to empty B
    forn(k, 1, m + 1)
    {
        edit[k][0] = 2 + k;
    }

    forn(i, 1, m + 1)
    {
        forn(j, 1, n + 1)
        {
            if (A[i - 1] == B[j - 1])
            {
                edit[i][j] = edit[i - 1][j - 1];
            }
            else
            {
                // replace penalty
                edit[i][j] = edit[i - 1][j - 1] + 2;
            }
            // Insert Cases
            forn(k, 1, j + 1)
            {
                // inserting sequence of character of B into A
                // insert length of (j-k+1) to minimize penalty
                edit[i][j] = min(edit[i][j], edit[i][k - 1] + 2 + (j - k + 1));
            }

            // Delete Cases
            forn(k, 1, i + 1)
            {
                // deleting sequence of character of A to make B
                // delete length of (i-k+1) to minimize penalty
                edit[i][j] = min(edit[i][j], edit[k - 1][j] + 2 + (i - k + 1));
            }
        }
    }
    return edit[m][n];
}
void solve()
{
    string A, B;
    cout << "Enter string A: ";
    cin >> A;

    cout << "Enter string B: ";
    cin >> B;

    cout << "Minimum penalty for converting A into B: " << convertToB(A, B) << endl;
}
//--------------------- Code End Here ---------------------------
signed main()
{

    //---------------------------------------------------------------

    int t = 1;
    cout << "Enter the number of testcase: ";
    cin >> t;
    while (t--)
    {
        solve();
    }

    return 0;
}
