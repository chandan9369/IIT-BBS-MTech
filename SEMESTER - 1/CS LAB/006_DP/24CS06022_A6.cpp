#include <bits/stdc++.h>
using namespace std;

#define forn(i, s, n) for (int i = s; i < n; i++)
#define all(x) (x).begin(), (x).end()

#define pb push_back
#define pi pair<int, int>
#define vi vector<int>
#define vb vector<bool>
#define vc vector<char>

#define endl '\n'
#define int long long

const int MOD = 1e9 + 7;
const int INF = LLONG_MAX >> 1;

// global values
int tSum;

// realizable function
bool realizable(vi &arr, int n, int T, vector<vb> &dp)
{
    // mark 0 sum as it will always be realizable
    dp[0][0 + tSum] = true;

    forn(i, 1, n + 1)
    {
        forn(j, -tSum, tSum + 1)
        {
            if ((j - arr[i] >= -tSum) && (dp[i - 1][j - arr[i] + tSum]))
            {
                dp[i][j + tSum] = true;
            }
            else if ((j + arr[i] <= tSum) && (dp[i - 1][j + arr[i] + tSum]))
            {
                dp[i][j + tSum] = true;
            }
        }
    }

    return dp[n][T + tSum];
}

void showone(vi &arr, int n, int T, vector<vb> &dp)
{
    cout << "One such solution:" << endl;
    int j = T;
    vector<char> op(n + 1);
    for (int i = n; i > 0; i--)
    {
        if ((j - arr[i] >= -tSum) && (dp[i - 1][j - arr[i] + tSum]))
        {
            op[i] = '+';
            j -= arr[i];
        }
        else if ((j + arr[i] <= tSum) && (dp[i - 1][j + arr[i] + tSum]))
        {
            op[i] = '-';
            j += arr[i];
        }
    }

    int res = 0;
    forn(i, 1, n + 1)
    {
        cout << "(" << op[i] << arr[i] << ") ";
        if (op[i] == '+')
            res += arr[i];
        else
            res -= arr[i];
    }
    cout << " = " << res << endl;
}

void solve()
{
    int n;
    cout << "Enter n: ";
    cin >> n;

    cout << "Enter elements of array: " << endl;
    vi arr(n + 1);
    forn(i, 1, n + 1)
    {
        cin >> arr[i];
    }

    tSum = accumulate(all(arr), 0);

    cout << "Enter T (between <" << (-tSum) << " , " << tSum << ">): ";
    int T;
    cin >> T;

    if (T < -tSum || T > tSum)
    {
        cout << "Realizable value is out of bound !!!";
        return;
    }

    cout << "\n--------------------- PART - 1 ---------------------------" << endl;
    vector<vb> dp(n + 1, vb(2 * tSum + 1, false));
    if (realizable(arr, n, T, dp))
    {
        cout << T << " can be realized by given array !!!" << endl;
    }
    else
    {
        cout << T << " can't be realized by given array !!!" << endl;
    }

    cout << "\n--------------------- PART - 2 ---------------------------" << endl;
    if (dp[n][T + tSum])
    {
        showone(arr, n, T, dp);
    }
    else
    {
        cout << T << " can't be realized by given array !!!" << endl;
    }
}

signed main()
{
    int t = 1;
    while (t--)
    {
        solve();
    }
    return 0;
}
