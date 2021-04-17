//Contraction Hierarchies algorithm
#include<iostream>
#include<vector>
#include<set>
#include<queue>

using namespace std;


#define MAX 110005
typedef  pair<long long int, long long int> pa;


#define pb push_back
long long int INF = 1e18;

vector<pa>adj[MAX];   // Graph
vector<pa>adjr[MAX];  // Reversed Graph

bool  contracted[MAX];  //tells if the node is contracted or not
long long int delNeighbors[MAX]; //number of deleted neighbors
long long int order[MAX]; // priority_order of the node
long long int hops[MAX];  //number of hopes used to reach there
long long int d_dist[MAX];  //distance list for dijkstra during contraction
bool visited[MAX];      //visited array for Dijkstra during contraction
long long int src, dest, n, m;
set<long long int>workset; // containing nodes to be re initialised during preprocesing
set<long long int>worksetD;  // containing nodes to be re initialised during queries

long long int f_dist[MAX]; // distance array for forward dijkstra
long long int r_dist[MAX]; //distance array for backward dijkstra
bool f_vis[MAX];   //visited array for forward dijkstra
bool r_vis[MAX];   //visited array for reversed dijkstra
priority_queue<pa, vector<pa>, greater<pa>>imp_pq; //importance priority queue

//For faster input and output
void fastio()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
}

//to find witness paths
void dijkstra(long long int src, long long int tot_max)
{
    //reinitialising the values changed during previous query
    for (auto k : workset)
    {

        d_dist[k] = -1;
        hops[k] = 0;
        visited[k] = false;
    }
    workset.clear();
    d_dist[src] = 0;
    visited[src] = true;
    priority_queue<pa, vector<pa>, greater<pa>>d_pq;
    d_pq.push({ 0,src });
    workset.insert(src);
    while (!d_pq.empty())
    {
        long long int v = d_pq.top().second;
        long long int w = d_pq.top().first;
        d_pq.pop();
        visited[v] = true;
        if (d_dist[v] > tot_max)   // break because no witness path after that
            return;
        if (hops[v] > 5)    //if hops more, not traversing
            continue;
        for (auto jj : adj[v])
        {
            if (contracted[jj.second] || visited[jj.second])
                continue;

            if ((d_dist[jj.second] > d_dist[v] + jj.first) || d_dist[jj.second] == -1)
            {
                workset.insert(jj.second);
                d_dist[jj.second] = d_dist[v] + jj.first;
                hops[jj.second] = hops[v] + 1;
                d_pq.push({ d_dist[jj.second],jj.second });
            }
        }
    }
}

//function that contract the node v
void contract(long long int v)
{
    contracted[v] = 1;
    long long int in_max = 0;
    long long int out_max = 0;

    for (auto jj : adj[v])
    {
        delNeighbors[jj.second]++;
        out_max = max(out_max, jj.first);
    }

    for (auto jj : adjr[v])
    {
        delNeighbors[jj.second]++;
        in_max = max(in_max, jj.first);
    }


    long long int tot_max = in_max + out_max;

    for (auto jj : adjr[v])
    {
        if (contracted[jj.second])
            continue;
        //dijkstra for finding witness path criteria
        dijkstra(jj.second, tot_max);

        for (auto kk : adj[v])
        {
            if (contracted[kk.second])
                continue;

            long long int cost = jj.first + kk.first;

            if (d_dist[kk.second] > cost || d_dist[kk.second] == -1)
            {
                adj[jj.second].pb({ cost,kk.second });  //adding edges to the graph
                adjr[kk.second].pb({ cost,jj.second });
            }
        }
    }

}




//preprocessing part
void preprocessing()
{
    //computing the importance function for each node
    for (int i = 1;i <= n;i++)
    {
        worksetD.insert(i);
        workset.insert(i);
        long long  imp = ((adj[i].size() * adjr[i].size()) - adj[i].size() - adjr[i].size()) + 2 * (adj[i].size() + adjr[i].size());
        imp += 2 * delNeighbors[i];
        imp_pq.push({ imp,i });
    }

    long long int ord = 1;
    while (!imp_pq.empty())
    {
        long long int a = imp_pq.top().first;
        long long int b = imp_pq.top().second;
        imp_pq.pop();
        if (!imp_pq.empty())
        {
            long long int a1 = imp_pq.top().first;
            long long new_val = ((adj[b].size() * adjr[b].size()) - adj[b].size() - adjr[b].size()) + 2 * (adj[b].size() + adjr[b].size());
            new_val = new_val + 2 * delNeighbors[b];
            if (new_val > a1)
            {
                imp_pq.push({ new_val,b });
                continue;
            }
        }
        order[b] = ord++;
        contract(b);
    }
}

//bidirectional dijkstra for faster output
long long int bidijkstra()
{
    priority_queue<pa, vector<pa>, greater<pa>>f_pq; //priority queue for forward/original graph
    priority_queue<pa, vector<pa>, greater<pa>>r_pq; //priority queue for reversed graph

    //reinitialising the values changed during previous query
    for (auto k : worksetD)
    {
        f_dist[k] = INF;
        r_dist[k] = INF;
        f_vis[k] = false;
        r_vis[k] = false;
    }
    worksetD.clear();

    long long int estimate = INF;
    f_pq.push({ 0,src });
    r_pq.push({ 0,dest });
    f_dist[src] = 0;
    f_vis[src] = true;
    r_vis[dest] = true;
    r_dist[dest] = 0;
    worksetD.insert(src);
    worksetD.insert(dest);
    // bidijisktra with alternate forward and backward step 
    while (!f_pq.empty() || !r_pq.empty())
    {
        //forward_iteration
        if (!f_pq.empty())
        {
            long long int wt = f_pq.top().first;
            long long int v = f_pq.top().second;
            f_vis[v] = true;
            f_pq.pop();


            if (f_dist[v] <= estimate)
            {
                for (auto jj : adj[v])
                {
                    long long int edge_wt = jj.first;
                    long long int u = jj.second;
                    if (!f_vis[u] && f_dist[v] + edge_wt<f_dist[u] && order[u]>order[v])
                    {
                        worksetD.insert(u);
                        f_dist[u] = f_dist[v] + edge_wt;
                        estimate = min(estimate, f_dist[v] + r_dist[u] + edge_wt);
                        f_pq.push({ f_dist[u],u });
                    }
                }
                estimate = min(estimate, f_dist[v] + r_dist[v]);
            }


        }
        //backward_iteration
        if (!r_pq.empty())
        {
            long long int wt = r_pq.top().first;
            long long int v = r_pq.top().second;
            r_pq.pop();
            r_vis[v] = true;

            if (r_dist[v] <= estimate)
            {

                for (auto jj : adjr[v])
                {
                    long long int edge_wt = jj.first;
                    long long int u = jj.second;
                    if (!r_vis[u] && r_dist[v] + edge_wt<r_dist[u] && order[u]>order[v])
                    {
                        worksetD.insert(u);
                        r_dist[u] = r_dist[v] + edge_wt;
                        estimate = min(estimate, r_dist[v] + f_dist[u] + edge_wt);
                        r_pq.push({ r_dist[u],u });
                    }
                }
                estimate = min(estimate, f_dist[v] + r_dist[v]);

            }
        }


    }

    return estimate;

}

int main()
{
    fastio();
    long long int x, y, wt;
    cin >> n >> m;
    for (int i = 0;i < m;i++)
    {
        cin >> x >> y >> wt;
        adj[x].pb({ wt,y });    //adding edge to the graph
        adjr[y].pb({ wt,x });  //adding edge to the reversed Graph
    }

    preprocessing();
    cout << "Ready" << endl;

    long long int num_of_queries;
    cin >> num_of_queries;
    while (num_of_queries--)
    {

        cin >> src >> dest;
        if (src == dest)
        {
            cout << 0 << "\n";
            continue;
        }
        long long int ans = bidijkstra();

        if (ans >= INF)
        {
            cout << -1 << "\n";
        }
        else
            cout << ans << "\n";
    }

}
