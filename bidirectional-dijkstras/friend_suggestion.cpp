#include<iostream>
#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
using namespace std;              //Anurag Kushwaha - NIT Patna
                                    /*working *\
                          /*------------------------------*\
1- it starts from the function "query" where it will first take starting and destination point as s and t.
2- now i make my 's' and 't' distance as 0 and push them in the priority queue to visit.
3- now I alternativeley remove the forward and backward vertex from the priority queue and each time i 
     will check whether this node visited or not if not then i will call visit function and this will
     visit all the neighbour of this node;
4- whenver i will get visited node then i will stop my query function and call the "shortestpath" function
     to return the shortest path
5- now shortest path will run a loop over every visited node and calculate sum of the forward and backward 
     distance and select the minumim out of it and return the result;
                         /*------------------------------*\

                                    
// External vector of size 2 - for forward and backward search.
// Internal 2-dimensional vector is vector of adjacency lists for each node.*/
typedef long long ll;
typedef vector<vector<vector<ll>>> Adj;



// Vector of two priority queues - for forward and backward searches.
// Each priority queue stores the closest unprocessed node in its head.
typedef vector<priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<pair<ll, int>>>> Queue;
#define  INFINITY  1e15

class Bidijkstra {
    // Number of nodes
    int n_;
    // Graph adj_[0] and cost_[0] correspond to the initial graph,
    // adj_[1] and cost_[1] correspond to the reversed graph.
    // Graphs are stored as vectors of adjacency lists corresponding
    // to nodes.
    // Adjacency list itself is stored in adj_, and the corresponding
    // edge costs are stored in cost_.
    Adj adj_;
    Adj cost_;
    // distance_[0] stores distances for the forward search,
    // and distance_[1] stores distances for the backward search.
    vector<vector<ll>> distance_;
    
    vector<ll> workset_; // Stores all the nodes visited either by forward or backward search.
   
    vector<bool> visited_;  // Stores a flag for each node which is True iff the node was visited
                           // either by forward or backward search.

public:
    Bidijkstra(ll n, Adj adj, Adj cost)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<ll>(n, INFINITY)), visited_(n)
    {
        workset_.reserve(n);
    }

    // Initialize the data structures before new query,
    // clear the changes made by the previous query.
    void clear() {
        //cout << "work set size is :" << workset_.size() << "\n";
        for (int i = 0; i < workset_.size(); ++i) {
            int v = workset_[i];
          //  cout << workset_[i] << " \n";
            distance_[0][v] = distance_[1][v] = INFINITY;
            visited_[v] = false;
        }
        workset_.clear();
    }

    // Processes visit of either forward or backward search 
    // (determined by value of side), to node v trying to
    // relax the current distance by dist.
    void visit(Queue& q, int side, int v, ll dist) {
       

        for (int i = 0;i < adj_[side][v].size();i++) {
            int u = adj_[side][v][i];
            ll cost_u = cost_[side][v][i];
            if ( distance_[side][u] > (distance_[side][v] + cost_u)) {
                distance_[side][u] = distance_[side][v] + cost_u;
                q[side].push({ distance_[side][u] ,u });
                workset_.push_back(u);
              
            }
        }
       
       
    }
    ll shortestpath(){
        ll best = INFINITY;
        for (auto i : workset_) {
            if (distance_[0][i] + distance_[1][i] < best)
                best = distance_[0][i] + distance_[1][i];
        }
        return best;
    }
    // Returns the distance from s to t in the graph.
    ll query(int s, int t) {
        clear();
        Queue q(2);
        distance_[0][s] = 0;   // forward
        distance_[1][t] = 0;  // backward
        q[0].push({ 0,s });  // forward
        q[1].push({ 0,t }); // backward
        workset_.push_back(s);
        workset_.push_back(t);
          while(true){
              if (q[0].empty() == 1 || q[1].empty() == 1) break;
                auto top1 = q[0].top();
                  q[0].pop();
             if (visited_[top1.second]) return shortestpath(); 
             visit(q, 0, top1.second ,top1.first);
             visited_[top1.second] = 1;

                auto top2 = q[1].top();
                  q[1].pop();
             if (visited_[top2.second])  return shortestpath();
             visit(q, 1, top2.second, top2.first);
             visited_[top2.second] = 1;

        } 
       
     

        return -1;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    ll n, m;
    cin >> n >> m;
    //scanf_s("%d%d", &n, &m);
    
    Adj adj(2, vector<vector<ll>>(n));
    Adj cost(2, vector<vector<ll>>(n));
    for (int i = 0; i < m; ++i) {
        int u, v, c;
        cin >> u >> v >> c;
        //scanf_s("%d%d%d", &u, &v, &c);
        adj[0][u - 1].push_back(v - 1);
        cost[0][u - 1].push_back(c);
        adj[1][v - 1].push_back(u - 1);
        cost[1][v - 1].push_back(c);
        
    }

    Bidijkstra bidij(n, adj, cost);

    int t; cin >> t;
    //scanf_s("%d", &t);
    for (int i = 0; i < t; ++i) {
        int u, v;
        cin >> u >> v;
        //scanf_s("%d%d", &u, &v);
        printf("%lld\n", bidij.query(u - 1, v - 1));
    }
}

