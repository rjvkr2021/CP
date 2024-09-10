#pragma GCC optimize("O3")
#include <bits/stdc++.h>

using namespace std;

#define ll long long
#define vll vector<ll>
#define vvll vector<vll>
#define pll pair<ll, ll>
#define all(v) v.begin(), v.end()
#define pb push_back
#define rep(i, a, b) for(ll i = a; i <= b; i++)
#define repr(i, a, b) for(ll i = a; i >= b; i--)

const ll MOD = 1e9 + 7;
const ll INF = 1e18;

// Merge Sort Tree
class MST{
public:
    ll n;
    vvll tree;
    vvll sum;

    vll merge(vll &v1, vll &v2){
        ll n = v1.size(), m = v2.size();
        vll v;
        ll i = 0, j = 0;
        while(i < n && j < m){
            if(v1[i] <= v2[j]){
                v.pb(v1[i++]);
            }
            else{
                v.pb(v2[j++]);
            }
        }
        while(i < n){
            v.pb(v1[i++]);
        }
        while(j < m){
            v.pb(v2[j++]);
        }
        return v;
    }

    void build_sum(ll curr){
        sum[curr].pb(tree[curr][0]);
        rep(i, 1, tree[curr].size() - 1){
            sum[curr].pb(sum[curr][i - 1] + tree[curr][i]);
        }
    }

    void build_tree(vll &v, ll curr, ll l, ll r){
        if(l == r){
            tree[curr].pb(v[l]);
            sum[curr].pb(v[l]);
            return;
        }
        ll m = (l + r)/2;
        build_tree(v, 2*curr + 1, l, m);
        build_tree(v, 2*curr + 2, m + 1, r);
        tree[curr] = merge(tree[2*curr + 1], tree[2*curr + 2]);
        build_sum(curr);
    }

    MST(vll &v){
        n = v.size();
        tree.resize(5*n);
        sum.resize(5*n);
        build_tree(v, 0, 0, n - 1);
    }

    // {number, sum} of elements < k in range [l..r]
    pll _query(ll curr, ll l, ll r, ll x, ll y, ll k){
        if(r < x || l > y){
            return {0, 0};
        }
        if(x <= l && r <= y){
            ll i = lower_bound(all(tree[curr]), k) - tree[curr].begin();
            if(i){
                return {i, sum[curr][i - 1]};
            }
            else{
                return {0, 0};
            }
        }
        ll m = (l + r)/2;
        pll left = _query(2*curr + 1, l, m, x, y, k),
            right = _query(2*curr + 2, m + 1, r, x, y, k);
        return {left.first + right.first, left.second + right.second};
    }

    pll query(ll l, ll r, ll k){
        return _query(0, 0, n - 1, l, r, k);
    }
};

vll manacher(string &s){
    string t = "@";
    for(char &ch: s){
        t.pb('#');
        t.pb(ch);
    }
    t.pb('#');
    t.pb('$');
    ll n = t.size();
    vll dp(n);
    dp[0] = 0;
    ll c = 0, r = 0;
    rep(i, 1, n - 1){
        ll i_image = 2*c - i;
        if(i < r){
            dp[i] = min(r - i, dp[i_image]);
        }
        while(t[i - dp[i] - 1] == t[i + dp[i] + 1]){
            dp[i]++;
        }
        if(i + dp[i] > r){
            c = i;
            r = i + dp[i];
        }
    }
    return dp;
}

void solve()
{
    string s;
    cin >> s;
    vll dp = manacher(s);
    ll n = dp.size();
    vll dp1(n), dp2(n);
    rep(i, 0, n - 1){
        dp1[i] = dp[i] - i;
        dp2[i] = dp[i] + i;
    }
    MST mst1(dp1);
    MST mst2(dp2);
    // number of palindromic substrings in s[l..r] (0-based indexing)
    auto query = [&](ll l, ll r){
        l = 2*(l + 1);
        r = 2*(r + 1);
        ll m = (l + r)/2, c1 = 1 - l, c2 = r + 1;
        pll left = mst1.query(l, m, c1), right = mst2.query(m + 1, r, c2);
        ll count = ((m - l + 1)*(l + m))/2 - ((r - m)*(m + 1 + r))/2;
        count += left.second + (m - l + 1 - left.first)*c1;
        count += right.second + (r - m - right.first)*c2;
        return count;
    };
    ll q;
    cin >> q;
    while(q--){
        ll l, r;
        cin >> l >> r;
        l--, r--;
        cout << query(l, r) << '\n';
    }
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    ll t = 1;
    // cin >> t;
    while(t--) solve();
    return 0;
}
