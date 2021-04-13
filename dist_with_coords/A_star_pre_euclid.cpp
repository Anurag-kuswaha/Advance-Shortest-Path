                               // Anurag Kushwaha - NIT Patna 

//  grader outcome - Good job! (Max time used: 26.40/50.00, max memory used: 82223104/2147483648.)

//  Implemented the Single Direction A* alogirthm; 
/*                        
                           ?? BASIC INTRODCTION  ??
*          # First I will calculate the potential function basically euclid distance of the each vertex from the
             destination vertex
*          # There can be two ways to calculate the potential funcion - either precompute or compute the
             potential of each vertex on the go;
*          # Here I have Implemented the precomputed Euclid distance;
*          starting from the query Function;
*/
/*
*  1- precomputing the euclid distance using eulicd_distance Function;
*  2- Now I will put the starting vertex in the priority queue as pair format of (distance_travelled+ euclid distance, vertex)
*  3- now popping from the priority queue with minimum potential function and running over its connected edges
*  4- visit Function will now visit over all of the connected edges and and try to relax the edges
*  5- Relaxing the edges means trying to update the minimum vertex potential;
*  6- After popped from the prority queue we first check whether this is the target vertex or not if yes return the dis_tavelled[curr_vertex];
*     which holds the till now distance travelled from the source vertex to target;
*/


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

    int n_;
    Adj adj_;
    Adj cost_;
    vector<vector<ll>> distance_;
    vector<int> workset_;
    vector<bool> visited_;
    // Coordinates of the nodes
    std::vector<std::pair<ll, ll>> xy_;
    vector<vector<double>> euclid;
    vector<vector<ll>> dis_travelled;

public:
    AStar(int n, Adj adj, Adj cost, std::vector<std::pair<ll, ll>> xy)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<ll>(n_, INFINITY)), visited_(n, 0), xy_(xy)
    {

        workset_.reserve(n); euclid.resize(2, vector<double>(n, 0)); dis_travelled.resize(2, vector<ll>(n, INFINITY));
    }


    void clear() {
        for (int i = 0; i < workset_.size(); ++i) {
            int v = workset_[i];
            distance_[0][v] = distance_[1][v] = INFINITY;
            visited_[v] = false;
            euclid[0][v] = euclid[1][v] = 0;
            dis_travelled[0][v] = dis_travelled[1][v] = INFINITY;
        }
        workset_.clear();
    }
    // pre-computing the potential value (euclid distance) of the each node 
    void euclidean_distance(int side, int s) {
        for (int i = 0;i < n_;i++) {
            ll x_ = (xy_[s].first - xy_[i].first);
            ll y_ = (xy_[s].second - xy_[i].second);
            ll d = x_ * x_ + y_ * y_;
            d = sqrt(d); //avoid use of pow() function it gives TLE;
            euclid[side][i] = d;
        }
    }
    //visiting each of the path of the removed vertex of the priority queue;
    void visit(Queue& q, int side, int v, ll dist) {

        for (int i = 0;i < adj_[side][v].size();i++) {
            int u = adj_[side][v][i];
            int cost = cost_[side][v][i];
            ll d = euclid[side][u] + cost + dis_travelled[side][v];  //potential + cost + distance till we travelled

            if (distance_[side][u] > d)
            {
                dis_travelled[side][u] = cost + dis_travelled[side][v];
                distance_[side][u] = d;
                q[side].push({ d,u });
                workset_.push_back(u);
            }
        }
    }
    // Returns the distance from s to t in the graph
    ll query(int s, int t) {
        clear();
        //if (s == t) return 0;

        euclidean_distance(0, t);  //calling to pre-compute the potential function(basically euclid distance); 
        dis_travelled[0][s] = 0;

        distance_[0][s] = euclid[0][s] + dis_travelled[0][s];
        Queue q(1);
        q[0].push({ distance_[0][s],s });

        workset_.push_back(s);
        while (true) {
            if (q[0].empty()) {
                break;
            }

            auto top1 = q[0].top();
            q[0].pop();

            if (top1.second == t)  return dis_travelled[0][top1.second]; 
            visit(q, 0, top1.second, top1.first);
            visited_[top1.second] = 1;

        }
        return -1;  
    }
};
int main() {
    //for dry run test the algorithm input as ans.txt. 
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
    std::vector<std::pair<ll, ll>> xy(n);
    for (int i = 0;i < n;++i) {
        int a, b;
        cin >> a >> b;
        xy[i] = make_pair(a, b);
    }
    Adj adj(2, vector<vector<int>>(n));
    Adj cost(2, vector<vector<int>>(n));
    for (int i = 0; i < m; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        adj[0][u - 1].push_back(v - 1);
        cost[0][u - 1].push_back(c);
        adj[1][v - 1].push_back(u - 1);
        cost[1][v - 1].push_back(c);
    }

    AStar astar(n, adj, cost, xy);

    int t;
    cin >> t;
    for (int i = 0; i < t; ++i) {
        int u, v;
        cin >> u >> v;
        // for dry run test;
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


