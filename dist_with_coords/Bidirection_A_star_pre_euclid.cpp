#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <cmath>
#include<iostream>
#include<fstream>

using namespace std;

// See the explanations of these typedefs and constants in the starter for friend_suggestion
typedef vector<vector<vector<int>>> Adj;
typedef long long ll;
typedef vector<priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>>> Queue;

#define INFINITY  1e13

class AStar {
    // See the descriptions of these fields in the starter for friend_suggestion
    int n_;
    Adj adj_;
    Adj cost_;
    vector<vector<ll>> distance_;
    vector<int> workset_;
    vector<vector<int>> visited_;
    // Coordinates of the nodes
    std::vector<std::pair<ll, ll>> xy_;
    vector<vector<double>> euclid;
    vector<vector<ll>> dis_travelled;

public:
    AStar(int n, Adj adj, Adj cost, std::vector<std::pair<ll, ll>> xy)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<ll>(n_, INFINITY)), visited_(2, vector<int>(n_, 0)), xy_(xy)
    {

        workset_.reserve(n); euclid.resize(2, vector<double>(n, 0)); dis_travelled.resize(2, vector<ll>(n, INFINITY));
    }

    // See the description of this method in the starter for friend_suggestion
    void clear() {
        for (int i = 0; i < workset_.size(); ++i) {
            int v = workset_[i];
            distance_[0][v] = distance_[1][v] = INFINITY;
            visited_[0][v] = false; visited_[1][v] = false;
            euclid[0][v] = euclid[1][v] = 0;
            dis_travelled[0][v] = dis_travelled[1][v] = INFINITY;
        }
        workset_.clear();
    }
    void euclidean_distance(int side, int s) {
        for (int i = 0;i < n_;i++) {
            double x_ = (xy_[s].first - xy_[i].first);
            double y_ = (xy_[s].second - xy_[i].second);
            double d = x_ * x_ + y_ * y_;
            d = sqrt(d);
            euclid[side][i] = d;
        }
    }
    void balance_potential() {
        for (int i = 0;i < n_;i++) {
            euclid[0][i] = ((euclid[0][i] - euclid[1][i]) / 2);
            euclid[1][i] = -(euclid[0][i]);
        }
    }
    // See the description of this method in the starter for friend_suggestion
    void visit(Queue& q, int side, int v, ll dist) {
        // Implement this method yourself
        //cout << "node visiting of v: " << v << "\n";
        for (int i = 0;i < adj_[side][v].size();i++) {
            int u = adj_[side][v][i];
            int cost = cost_[side][v][i];


            ll d = euclid[side][u] + cost + dis_travelled[side][v];
            if (distance_[side][u] > d)
            {
                dis_travelled[side][u] = cost + dis_travelled[side][v];
                distance_[side][u] = d;
                q[side].push({ d,u });
                workset_.push_back(u);
            }
        }
    }
    ll shortestpath() {
        //return dis_travelled[0][v] + dis_travelled[1][v];
        int n = workset_.size();
        ll min_ = 1000000000;
        for (int i = 0;i < n;i++) {
            int u = workset_[i];
            //cout << (dis_travelled[0][u]) <<" : "<<(dis_travelled[1][u]) << '\n';
          // cout << (dis_travelled[0][u] + dis_travelled[1][u]) << '\n';
            min_ = min(dis_travelled[0][u] + dis_travelled[1][u], min_);
        }
        return min_;
    }
    // Returns the distance from s to t in the graph
    ll query(int s, int t) {
        clear();
        //if (s == t) return 0;

        euclidean_distance(0, t); //O(n*(O(sqrt())
        euclidean_distance(1, s);//O(n*(O(sqrt())
        balance_potential(); //O(n)
        dis_travelled[0][s] = 0;
        dis_travelled[1][t] = 0;
        distance_[0][s] = euclid[0][s] + dis_travelled[0][s];
        distance_[1][t] = euclid[1][t] + dis_travelled[1][t];
        Queue q(2);
        q[0].push({ distance_[0][s],s });
        q[1].push({ distance_[1][t],t });

        workset_.push_back(s);
        workset_.push_back(t);
        // Implement the rest of the algorithm yourself
        while (true) { //O(log(n)*edges);
            if (q[0].empty() == true || q[1].empty() == true) {
                break;
            }

            auto top1 = q[0].top();
            q[0].pop();
            //cout << "1st top :" << top1.second << "\n";
            if (visited_[1][top1.second]) { return shortestpath(); }
           visit(q, 0, top1.second, top1.first);
            
            visited_[0][top1.second] = 1;

            auto top2 = q[1].top();
            q[1].pop();
            //cout << "2nd top :" << top2.second << "\n";
            if (visited_[0][top2.second]) { return shortestpath(); }
            visit(q, 1, top2.second, top2.first);
           
            visited_[1][top2.second] = 1;


        }
        return -1;
    }
};
int main() {
    //vector<ll> ans;
    //ifstream in("ans.txt");
   /* while (in) {
        int a;
        in >> a;
        /* ll temp = 0;
         int s = a.size();
             int i = 0;
         while (i< s) {
             temp += temp * 10; int(a[i]-'0');
         } 
        ans.push_back(a); */
   
    //for (auto it : ans) cout << it << " ";
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n >> m;
    //scanf("%d%d", &n, &m);
    std::vector<std::pair<ll, ll>> xy(n);
    for (int i = 0;i < n;++i) {
        int a, b;
        cin >> a >> b;
        //scanf("%d%d", &a, &b);
        xy[i] = make_pair(a, b);
    }
    Adj adj(2, vector<vector<int>>(n));
    Adj cost(2, vector<vector<int>>(n));
    for (int i = 0; i < m; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        // scanf("%d%d%d", &u, &v, &c);
        adj[0][u - 1].push_back(v - 1);
        cost[0][u - 1].push_back(c);
        adj[1][v - 1].push_back(u - 1);
        cost[1][v - 1].push_back(c);
    }

    AStar astar(n, adj, cost, xy);

    int t;
    cin >> t;
    //scanf("%d", &t);
    for (int i = 0; i < t; ++i) {
        int u, v;
        cin >> u >> v;
        //scanf("%d%d", &u, &v);
      /*  //printf("%lld\n", astar.query(u - 1, v - 1));
        if (ans[i] != astar.query(u - 1, v - 1)) {
            cout << i << "\n";
            cout << "mismatch  " << u << " : " << v << "\n";
            cout << "correct ans is " << ans[i] << '\n';
            cout << "our  ans is " << astar.query(u - 1, v - 1) << '\n';
            abort();

        } */
        cout << astar.query(u - 1, v - 1) << '\n';
    }
}