
//                  Anurag kushwaha NIT Patna                         




/*    implemented Contraction Hierarchies algorithm and apply it to preprocess large
      road networks and then compute distances in them much faster;
*/
//grader result - Good job! (Max time used: 1.82/2.00, max preprocess time used: 2.65/10.00, max memory used: 0/2147483648.)


#include <iostream>
#include<fstream>
#include <vector>
#include<queue>
#include<set>
#include <algorithm>
#include <cmath>
#include<stdio.h>
using namespace std;

#define INF 1e12
#define ll long long
#define MAX 110001
typedef pair<ll, ll> pr;
typedef priority_queue<pr, vector<pr>, greater<pr>> Queue; // define minimum heap priority queue;

Queue order_pq;
vector<ll> outgoing[MAX];                      //forward edges 
vector<ll> incoming[MAX];                      // backward edges
vector<ll> out_cost[MAX];                      //forward edges cost
vector<ll> in_cost[MAX];                       // backward edges cost
ll order[MAX];
vector<ll> contracted_neighbour(MAX,0);        // number of neighbour contracted
vector<bool> contracted(MAX, 0);               // vertices is contracted or not ;
set<ll> workset;                               //workset for storing the work done on which vertices
ll n, m;
vector<ll> dist_u(MAX, INF);                   //distance from vertices u in dijkstras;
vector<ll> hops_u(MAX, 0);                     // stores the number of hops 
vector<bool> visited(MAX, 0);
Queue pq;                                      //dijkstras priority queue;


void dijkstras_clear() {
	     for(auto i: workset) {
		        dist_u[i] = INF;
		        hops_u[i]  = visited[i] = 0;
	     }
	     while (!pq.empty()) pq.pop();
	     workset.clear();
}
//------3---------------running dijkstras to check the witness path--------------------------------
void dijkstras(ll u, ll max_tot) {
	//1- create pq which stores the {dist, vertices} format and distacne from the u vertices;
	     dijkstras_clear();

	     dist_u[u] = 0;
	     hops_u[u] = 0;
	     visited[u] = 1;
	     pq.push({ 0,u });
	     while (!pq.empty()) {
		         pr p = pq.top();
		         pq.pop();
		         ll u1 = p.second;
		         visited[u1] = 1;
		         workset.insert(u1);
		         if (dist_u[u1] >max_tot) break; // now time to stop witness path search as it crossed max path 
		         if (hops_u[u1] > 5) continue;
		
		         for (int i = 0;i < outgoing[u1].size();i++) {
		                	ll v1 = outgoing[u1][i];
			                ll cost = out_cost[u1][i];
			                if (contracted[v1] || visited[v1]) continue;  // if something is already contacted or visited;

			                if (dist_u[v1] > dist_u[u1] + cost || dist_u[v1]==INF) {
			                       	dist_u[v1] = dist_u[u1] + cost;
			                    	hops_u[v1] = 1 + hops_u[u1];
			                     	pq.push({ dist_u[v1],v1 });
			                     	workset.insert(v1);
			                }
		         }
	     }

}

// -----------2-----------------contracting each of the vertices---------------------------------
void contract(ll v) {
	      // 1- first calculate the maximum incoming and outgoing from the vertices v
	     // to stop the witness path to go further if witness path found more the sum of max_in+ max_out
	    //2 now run dijkstras over each of the incoming vertices coming at v and try to find the witness path is there or not 
	     contracted[v] = 1;
	     ll max_in = 0;
	     ll max_out = 0;
	     for (int i = 0;i < incoming[v].size();i++) {
		          contracted_neighbour[incoming[v][i]]+=1;
		          max_in = max(max_in, in_cost[v][i]);
	     }
	     for (int i = 0;i < outgoing[v].size();i++) {
		          contracted_neighbour[outgoing[v][i]]+=1;
		          max_out = max(max_out, out_cost[v][i]);
	     }

	     ll max_tot = max_in + max_out;

	     for (int i = 0;i < incoming[v].size();i++) {
		          ll u = incoming[v][i];

		          if (contracted[u]) continue;

		          dijkstras(u, max_tot);
		          //going over the the outgoing edges to check whether any shortest path exist from the 
		          //incoming edges to outgoing edges surpassing the v;
		          for (ll j = 0;j < outgoing[v].size();j++) {
			                 ll out_v = outgoing[v][j]; //outgoing from v 

			                 if (contracted[out_v]) continue;
			                 ll bypass_cost = out_cost[v][j] + in_cost[v][i];

			                 if (dist_u[out_v] > bypass_cost) {
				             //add shortest path in the graph;
				                     outgoing[u].push_back(out_v); //now path is coming to out_v from u diirectly i.e
			                         out_cost[u].push_back(bypass_cost); // before u-->--v-->--out_v  after u-->--out_v 
				                     incoming[out_v].push_back(u);     // now the reverse part;
				                     in_cost[out_v].push_back(bypass_cost);  // u-- < --v-- < --out_v  now u-- < --out_v
			                  //	cout << "added eges are ->: "<< u+1 << " ->: " << out_v+1<<" cost is :"<<bypass_cost<< "\n";
			                 }
		          }
	     }
}
ll recompute_importance(ll v) {
	      ll edge_diff = (incoming[v].size() * outgoing[v].size()) - incoming[v].size() - outgoing[v].size();
	      ll short_cover = 2 * (incoming[v].size() + outgoing[v].size());
	      ll contracte_neighbour = 2*contracted_neighbour[v];
	      ll imp = edge_diff + short_cover +contracte_neighbour;
	      return imp;
}
//----------1---------------------preprocess starts from here--------------------------------------------

void preprocess() {
	//1- calculate the node order;
	//2- contract the node;
	// for this try to find the witness path
	
	     for (ll i = 0;i < n;i++) {
		         ll edge_diff = (incoming[i].size() * outgoing[i].size()) - incoming[i].size() - outgoing[i].size();
		         ll short_cover = 2 * (incoming[i].size() + outgoing[i].size());
		         ll contract_neighbour = 2* contracted_neighbour[i];
		         ll imp = edge_diff + short_cover + contract_neighbour;
		         order_pq.push({ imp,i });
	     }
	     int ordr = 1;
	     while (!order_pq.empty()) {
		         pr p = order_pq.top();
		            order_pq.pop();
		         ll imp = p.first;
		         ll v = p.second;
		        
		         ll new_imp = recompute_importance(v);
			     if (new_imp > imp) {   //importnce change becuse of neighbour is contracted and we don't want to contract the neighbour which are close to each other
				            order_pq.push({ new_imp,v });
				            continue; // so again push in the queue and remove according to importance;
			     }
		         
		         contract(v);
		         order[v] = ordr++;
	     }
}
/////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------- QUERY part  -------------------------------------------//////
///////////////////////////////////////////////////////////////////////////////////////////
vector<ll> f_distance(MAX, INF);                   // forward dist. of bidirectional dijkstras
vector<ll> b_distance(MAX, INF);                   // backward dist. of bidirectional dijkstras
ll f_visited[MAX];                                 // forward visited
ll b_visited[MAX];                                 // backward visited
					                               // workset for cleaning purpose;
set<ll> workset_f;                                 // forward and backward workset for faster clearence
set<ll> workset_b;                                 //   of the previous work of the query
Queue q[2];                                        // store the pair as distance and vertex;  q[0]-forward and q[1]- backward


void process_for(ll src) {

	     for (int i = 0;i < outgoing[src].size();i++) {
		           ll v = outgoing[src][i];
		           ll cost = out_cost[src][i];
		           if (f_visited[v]) continue;
		                   if (f_distance[v] > cost + f_distance[src] && order[v] >= order[src]) { // if order is greater then only visit
			                        f_distance[v] = cost + f_distance[src];
			                        q[0].push({ f_distance[v],v });
			                         workset_f.insert(v);
		                   }
	     }
}
void process_back(ll src) {

	     for (int i = 0;i < incoming[src].size();i++) {
		           ll v = incoming[src][i];
		           ll cost = in_cost[src][i];
		           if (b_visited[v]) continue;
		                   if (b_distance[v] > cost + b_distance[src] && order[v] >= order[src]) { // if order is greater then only visit
		                           	b_distance[v] = cost + b_distance[src];
			                        q[1].push({ b_distance[v],v });
		                           	workset_b.insert(v);
		                   }
	     }
}
ll  bidijkstras(ll s, ll t) {
	// running bidirectional dijkstras to find shortest path between s and t;
	
	     for (auto i : workset_f) {
			     f_distance[i] = INF;
			     f_visited[i] = 0;
		
	     }
         for (auto i : workset_b) {
		         b_distance[i] = INF;
		         b_visited[i] = 0;
	     }
	
	     workset_f.clear(); workset_b.clear();
	
	     ll estimate = INF;
	     f_distance[s] = 0; b_distance[t] = 0;
	     f_visited[s] = 1; b_visited[t] = 1;
	
	     q[0].push({ 0,s }); // for forward search
	     q[1].push({ 0,t }); // for backward search 
	     workset_f.insert(s); workset_b.insert(t);
	
	     while (true) {
	            	if (q[0].empty() && q[1].empty())   break;

		                    if (!q[0].empty()) {
			                        // forward search;
			                         auto top = q[0].top();
			                         q[0].pop();
			                         ll forward_move = top.second;
			                         f_visited[forward_move] = 1;
			                         if (f_distance[forward_move] <= estimate)
			          	                        process_for(forward_move);
	
			                         estimate = min(estimate, f_distance[forward_move] + b_distance[forward_move]);    
		                    }
		                    if (!q[1].empty()) {
			                         // backward search;
			                         auto top = q[1].top();
			                         q[1].pop();
			                         ll back_move = top.second;
			                         b_visited[back_move] = 1;
			                         if (b_distance[back_move] <= estimate) 
				                                process_back(back_move);
			            
			                         estimate = min(estimate, f_distance[back_move] + b_distance[back_move]);  
		                    }
	     }
	     return estimate;
}

int main() {
	     ios_base::sync_with_stdio(false);
	     cin.tie(NULL);
	     cin >> n >> m; //n is vertices and m is edges
	     ll u, v, c;
	     for (ll i = 0;i < m;i++) {
		        cin >> u >> v >> c;
		        outgoing[u - 1].push_back(v - 1);
		        out_cost[u - 1].push_back(c);
		        incoming[v - 1].push_back(u - 1);
		        in_cost[v - 1].push_back(c);

	     }

	     preprocess();
	     cout << "Ready"<<endl;
	     ll query; cin >> query;
	     ll s, t;
	     int k = 0;
	     while (query--) {
		        cin >> s >> t;
		        ll distance = bidijkstras(s - 1, t - 1);
		        if (distance == INF) cout << "-1\n";
		        else cout << distance << '\n';
	     }
}
