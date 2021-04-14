                              // Anurag Kushwaha - NIT Patna 

//  Coursera grader outcome - Failed case #8/8: time limit exceeded (Time used: 100.02/50.00, memory used: 83046400/2147483648.
                                       
//            Implemented the Bidirectional A* alogirthm 
/*                        
                           ?? BASIC INTRODCTION  ??
*          # First I will calculate the potential function basically euclid distance of the each vertex from the
             destination vertex
*          # There can be two ways to calculate the potential funcion - either precompute or compute the
             potential of each vertex on the go;
*          #  # Here I have Implemented the  Euclid distance on the go;
*          #  starting from the query Function;
*/
                                    /*working *\
                          /*------------------------------*\
1- it starts from the function "query" where it will first take source and destination point as s and t.
2- Now I will put the sourece vertex for the forward search and destination vertex for the backward search in the priority queue 
    as pair format of (distance_travelled+ euclid distance, vertex)
3- now I alternativeley remove the forward and backward vertex from the priority queue and each time i 
     will check whether this node visited or not if not then i will call visit function and this will
     visit all the neighbour of this node;
4- whenver i will get visited node then i will stop my query function and call the "shortestpath" function
     to return the shortest path
5- now shortest path will run a loop over every visited node and calculate sum of the forward and backward 
     distance and select the minumim out of it and return the result;
                         /*------------------------------*\ */



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
    ll euclid_single(int source,int dest, int curr,int side) {

        ll x_1 = (xy_[source].first - xy_[curr].first);
        ll y_1 = (xy_[source].second - xy_[curr].second);
        ll dis1 = x_1 * x_1 + y_1 * y_1;
        ll x_2 = (xy_[dest].first - xy_[curr].first);
        ll y_2 = (xy_[dest].second - xy_[curr].second);
        ll dis2 = x_2 * x_2 + y_2 * y_2;
        dis1 = sqrt(dis1);
        dis2 = sqrt(dis2);
        if (side == 1) {
            //reverse traverse then return the neagtive of forward potential
            return -(balance_potential(dis1, dis2));
        }
        return balance_potential(dis1, dis2);
    }
  
    // See the description of this method in the starter for friend_suggestion
    void visit(Queue& q, int side, int v, ll dist,int s,int t) {
        for (int i = 0;i < adj_[side][v].size();i++) {
            int u = adj_[side][v][i];
            int cost = cost_[side][v][i];


            ll d = euclid_single(s, t, u, side) + cost + dis_travelled[side][v];
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
        int n = workset_.size();
        ll min_ = 1000000000;
        for (int i = 0;i < n;i++) {
            int u = workset_[i];
            min_ = min(dis_travelled[0][u] + dis_travelled[1][u], min_);
        }
        return min_;
    }
    // Returns the distance from s to t in the graph
    ll query(int s, int t) {
        clear();
        dis_travelled[0][s] = 0;
        dis_travelled[1][t] = 0;
        distance_[0][s] = euclid_single(s,t,s,0) + dis_travelled[0][s];
        distance_[1][t] = euclid_single(s, t, t, 1) + dis_travelled[1][t];
        Queue q(2);
        q[0].push({ distance_[0][s],s });
        q[1].push({ distance_[1][t],t });

        workset_.push_back(s);
        workset_.push_back(t);
        while (true) { //O(log(n)*edges);
            if (q[0].empty() == true || q[1].empty() == true) {
                break;
            }

            auto top1 = q[0].top();
            q[0].pop();
          
            if (visited_[1][top1.second]) { return shortestpath(); }
           visit(q, 0, top1.second, top1.first,s,t);
            
            visited_[0][top1.second] = 1;

            auto top2 = q[1].top();
            q[1].pop();
            if (visited_[0][top2.second]) { return shortestpath(); }
            visit(q, 1, top2.second, top2.first,s,t);
           
            visited_[1][top2.second] = 1;


        }
        return -1;
    }
};
int main() {
   
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
        cout << astar.query(u - 1, v - 1) << '\n';
    }
}
