// https://codeforces.com/contest/1806/problem/E
// https://codeforces.com/contest/1806/my
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

const ll blockSize = 500; // 300, 700

struct Query {
    ll l, r, index;
    friend bool operator<(Query a, Query b) {
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

struct MoOnTree {
private:
    vector<ll> eulertour, first, second;
    template<typename LL>class LCA{struct Euler{LL vertex, height, index;};template<typename T>class LCASegmentTree{private:ll n;vector<T>dat;public:T merge(T a,T b){if(a.height>b.height)return b;return a;}LCASegmentTree(vector<T>v){int _n=v.size();n=1;while(n<_n)n*=2;dat.resize(2*n-1);for(int i=0;i<_n;i++)dat[n+i-1]=v[i];for(int i=n-2;i>=0;i--)dat[i]=merge(dat[i*2+1],dat[i*2+2]);} LCASegmentTree(int _n){n=1;while(n<_n)n*=2;dat.resize(2*n-1); } void set_val(int i,T x){i+=n-1;dat[i]=x;while(i>0){i=(i-1)/2;dat[i]=merge(dat[i*2+1],dat[i*2+2]);}}T query(int l,int r){r++;T left=T{INT_MAX,INT_MAX,INT_MAX},right=T{INT_MAX,INT_MAX,INT_MAX};l+=n-1;r+=n-1;while(l<r){if((l&1)==0)left=merge(left,dat[l]);if((r&1)==0)right=merge(dat[r-1],right);l=l/2;r=(r-1)/2;}return merge(left,right);}};public:int n;vector<vector<LL>> graph;vector<bool> visited;vector<Euler> eulertour, first;LCASegmentTree<Euler> *seg;LCA() {}LCA(vector<vector<LL>> graph, LL root = 0){build(graph, root);}void build(vector<vector<LL>> graph, LL root = 0) {this->graph = graph;n = graph.size();visited.resize(n);first.resize(n);makeEuler(root);}void makeEuler(LL root){std::fill(visited.begin(), visited.end(), false);int height =0;std::function<void(int)> explore = [&](int u){visited[u] = true;height++;eulertour.push_back(Euler{u, height, (int) eulertour.size()});for (auto v: this->graph[u]){if (!visited[v]) {explore(v);height--;eulertour.push_back(Euler{u, height, (int) eulertour.size()});}}};explore(root);std::fill(visited.begin(), visited.end(), false);for (auto e: eulertour){if (!visited[e.vertex]){visited[e.vertex] = true;first[e.vertex] = e;}}this->seg = new LCASegmentTree<Euler>(eulertour);}LL lca(LL u, LL v){LL uidx = first[u].index;LL vidx = first[v].index;if (uidx > vidx) swap(uidx, vidx);Euler a = seg->query(uidx, vidx);return a.vertex;}vector<LL> height(){vector<LL> h(this->n, 0);for (auto e: eulertour) h[e.vertex] = e.height;return h;}LL lca(LL r, LL u, LL v){LL ru = lca(r, u);LL rv = lca(r, v);LL uv = lca(u, v);if (ru == rv) return uv;if (ru == uv) return rv;return ru;}};
    vector<ll> makeEulerTour(vector<vector<ll>> adj) {
        vector<ll> eulertour;
        vector<bool> vis((int) adj.size(), false);
        std::function<void(ll)> dfs = [&](ll u){
            eulertour.push_back(u);
            vis[u] = true;
            for (auto v: adj.at(u)) {
                if (!vis[v]) dfs(v);
            }
            eulertour.push_back(u);        
        };
        dfs(0);
        first.resize(n); second.resize(n); // first[v] - thời điểm duyệt v, second[v] - thời điểm ra khỏi v 
        vector<int> cnt(n, 0);
        for (int i=0;i<(int) eulertour.size();i++) {
            if (cnt[eulertour[i]] == 0) {
                first.at(eulertour.at(i)) = i;
            } else {
                second.at(eulertour.at(i)) = i;
            }
            cnt.at(eulertour.at(i))++;
        }
        return eulertour;
    }
    vector<Query> fromEdgeToEuler(vector<Query> queries) {
        // Thực hiện việc chuyển query về query trên đỉnh sang vị trí trên eulertour (flatten tree)
        vector<Query> res;
        for (auto uv: queries) {
            ll u = uv.l, v= uv.r;
            ll p = lca.lca(u, v);
            if (p == u || p == v) {
                // First->first
                if (first.at(u) < first.at(v)) res.push_back({first[u], first[v], uv.index});
                else res.push_back({first.at(v), first.at(u), uv.index});
                continue;
            }
            // Second -> first + cần xét tới giá trị lca của 2 điểm đó
            if (second.at(u) < first.at(v)) {
                res.push_back({second.at(u), first.at(v), uv.index});
            } else {
                res.push_back({second.at(v), first[u], uv.index});
            }
        }
        return res;
    }
    void calDepth() {
        depth[0] = 0;
        std::function<void(ll, ll)> dfs = [&](ll u, ll p){
            for (auto v: graph[u]) {
                if (v == p) continue;
                depth[v] = depth[u]+ 1;
                dfs(v, u);
            }
        };
        dfs(0, 0);
    }
public:
    ll n;
    vector<vector<ll>> graph; // mảng đầu vào
    vector<ll> weight;
    vector<Query> queries; // l, r, index
    LCA<ll> lca;
    vector<bool> vis; // đánh dấu số lần đỉnh đó đã đi qua
    vector<Ans> ans; // index, val
    ll cur_result = 0; // giá trị hiện tại, đối với những bài giá trị tuyến tính khi add, remove thì dùng, ko thì tính trực tiếp từ mảng data
    vector<ll> depth_left, depth_right, depth;
    vector<ll> touch_times;
    vector<ll> points;
    MoOnTree(vector<vector<ll>> g, vector<ll> weight, vector<Query> queries) {
        this->n = (ll) g.size();
        this->graph = g;
        this->weight = weight;
        lca.build(g);
        eulertour = makeEulerTour(graph);
        this->queries = fromEdgeToEuler(queries);
        vis.resize(n, false);
        depth.resize(this->n, 0);
        depth_left.resize(this->n, 0);
        depth_right.resize(this->n, 0);
        calDepth();
        touch_times.resize(this->n, 0);
        points.resize(this->n, 0);
        calPoints();
    }
    void calPoints() {
        std::function<void(ll u, ll p)> dfs = [&](ll u, ll p){
            points[u] = weight[u] * weight[u] + points[p];
            for (auto v: graph[u]) {
                if (v == p) continue;
                dfs(v, u);
            }
        };
        dfs(0, 0);
    }
    void resolve(ll index, vector<ll> *depthVal) {
        ll edge = eulertour[index];
        vis.at(edge) = !vis.at(edge); // nếu 1 đỉnh visit 1 lần thì nó nằm trong khoảng, 2 lần là nằm ngoài khoảng
        ll dep = depth.at(edge);
        if (!vis[edge]) remove(edge);
        (*depthVal)[dep] = weight[edge];
        if (vis[edge]) add(edge);
    }
    void add(ll edge) {
        touch_times[depth[edge]]++;
        if (touch_times[depth[edge]] == 2) {
            cur_result += depth_left.at(depth.at(edge)) * depth_right.at(depth.at(edge));
        }
    }
    void remove(ll edge) {
        if (touch_times[depth[edge]] == 2) {
            cur_result -= depth_left.at(depth.at(edge)) * depth_right[depth[edge]];
        }
        touch_times[depth[edge]]--;
    }
    void solve() {
        sort(queries.begin(), queries.end());
        ll cur_l = 0, cur_r = -1; depth_left[0] = weight[0];
        for (auto query: queries) {
            // Chỉ thêm sửa giá trị khi layer đó xuất hiện 2 lần
            while (cur_l > query.l) resolve(--cur_l, &depth_left);
            while (cur_r < query.r) resolve(++cur_r, &depth_right);
            while (cur_l < query.l) resolve(cur_l++, &depth_left);
            while (cur_r > query.r) resolve(cur_r--, &depth_right);
            // Xét tới lca.lca(u, v)
            ll p = lca.lca(eulertour[query.l], eulertour[query.r]);
            ans.push_back({query.index, cur_result + points[p]});
        }
        sort(ans.begin(), ans.end());
        for (auto v: ans) cout << v.val <<'\n';
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
    vector<ll> w(n);
    for (auto &v: w) cin >> v;
    vector<vector<ll>> adj(n);
    for (ll i=2;i<=n;i++) {
        ll p; cin >> p;
        adj[p-1].push_back(i-1);
        adj[i-1].push_back(p-1);
    }
    vector<Query> queries;
    for (ll i=0;i<q;i++) {
        ll u, v; cin >> u >> v;
        u--; v--;
        queries.push_back(Query{u, v, i});
    }
    MoOnTree mo(adj, w, queries);
    mo.solve();

    cerr << "Time : " << (double)clock() / (double)CLOCKS_PER_SEC << "s\n";
}
/*
Bài này sử dụng Mo's algorithm on tree, hiện tại lời giải đang khá rắc rối,
hơi khó để xử lý nhanh khi gặp 1 bài khác tương tự.
Tư tưởng:
bài này query luôn thuộc dạng 2 (dạng 1 là first->first khi u là cha của v, dạng 2 là u,v có lca != u, v)
Duy trì 1 mảng depth_left, depth_right. với depth_left[2] là giá trị của node có depth=2 nằm trên đường đi từ u->v
Mỗi khi add 1 node mới thì tùy vào đó là left, right mà sẽ thay đổi giá trị depth_l_or_r[depth[node]] = weight[node];
Dùng touch_times[] để lưu lại số lần depth đó được duyệt.
Do left, right không cùng thay đổi đồng thời, ví dụ query cần 2 node có dep = 5.
Hiện tại l, r đang có dep = 3. Khi này l sẽ dịch dần trên eulertour tới dep=5.
l dịch tới dep=4, dep=5, touch_times[dep=4,5]++ -> 1. khi này không thay đổi giá trị gì cả
r dịch tới dep=4, dep=5, touch_times[dep=4,5]++ -> 2. Khi này giá trị sẽ được cộng depth_left[4] * depth_right[4], depth_left[4] * depth_right[4] 
Bài này thuộc dạng 2 nên không cần add rồi remove lca.lca(l, r) mà chỉ cần thêm giá trị lúc in ra là xong
*/