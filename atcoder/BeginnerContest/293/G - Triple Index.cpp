// https://atcoder.jp/contests/abc293/tasks/abc293_g
// https://atcoder.jp/contests/abc293/tasks/abc293_g
// https://atcoder.jp/contests/abc293/submissions/39611628 - reference
#include<bits/stdc++.h>

typedef long long ll;
const ll mod = 1e9 + 7;
#define ld long double

using namespace std;

#ifdef DEBUG
#include "debug.cpp"
#else
#define dbg(...)
#define destructure(a) #a
#endif

const ll blockSize = 300;

struct Query {
    ll l, r, index;
    friend bool operator<(Query a, Query b) {
        // những cái thuộc cùng 1 block sẽ đứng cạnh nhau, những cái cùng block thì r sẽ xếp từ bé tới lớn
        if (a.l/blockSize == b.l/blockSize) return a.r < b.r;
        return a.l/blockSize < b.l/blockSize;
    }
    friend std::ostream& operator<<(std::ostream& os, const Query &s) { return os << destructure(s);}
};
struct Ans {
    ll index, val;
    friend bool operator<(Ans a, Ans b) {
        return a.index < b.index;
    }
    friend std::ostream& operator<<(std::ostream& os, const Ans &s) { return os << destructure(s);}
};

struct Mo {
private:
    vector<ll> a; // mảng đầu vào
    vector<Query> queries; // l, r, index
public:
    vector<ll> data; // lưu trạng thái hiện tại - lưu tần số của từng số có trong range [left, right] hiện tại
    vector<Ans> ans; // index, val
    ll cur_result = 0;
    Mo(vector<ll> a, vector<Query> queries) {
        this->a = a;
        data.resize(200005, 0);
        this->queries = queries;
    }
    void add(ll index) {
        cur_result -= data[a[index]] * (data[a[index]]-1) * (data[a[index]]-2) /6;
        data[a[index]]++;
        cur_result += data[a[index]] * (data[a[index]]-1) * (data[a[index]]-2) /6;
    }
    void remove(ll index) {
        cur_result -= data[a[index]] * (data[a[index]]-1) * (data[a[index]]-2) /6;
        data[a[index]]--;
        cur_result += data[a[index]] * (data[a[index]]-1) * (data[a[index]]-2) /6;
    }
    ll getResult() {
        return this->cur_result;
    }
    void solve() {
        sort(queries.begin(), queries.end());
        ll left = 0, right = -1;
        for (auto query: queries) {
            while (left > query.l) left--, add(left);
            while (right < query.r) right++, add(right);
            while (left < query.l) remove(left), left++;
            while (right > query.r) remove(right), right--;
            ans.push_back({query.index, getResult()});
        }
        sort(ans.begin(), ans.end());
    }
};


int main(){
    ios::sync_with_stdio(0); cin.tie(0);
    #ifdef DEBUG
        freopen("inp.txt", "r", stdin);
        freopen("out.txt", "w", stdout);
    #endif
    ll n, q;
    cin >> n >> q;
    vector<ll> a(n);
    for (ll i=0;i<n;i++) cin >> a[i];
    vector<Query> queries;
    for (ll i=0;i<q;i++) {
        ll l, r;
        cin >> l >> r;l--;r--;
        queries.push_back(Query{l, r, i});
    }
    sort(queries.begin(), queries.end());
    Mo mo(a, queries);
    mo.solve();
    for (auto v: mo.ans) {
        cout << v.val << '\n';
    }
    cerr << "Time : " << (double)clock() / (double)CLOCKS_PER_SEC << "s\n";
}
/*
Dùng Mo's algorithm nhưng đoạn kia nhất thiết không được dùng map để xử lý, map -> vector + compress
*/
