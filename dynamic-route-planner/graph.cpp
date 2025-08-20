#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>
#include <cstdlib>
#include <ctime>
#include <cmath>

using namespace std;

class Graph {
    int V; 
    vector<vector<pair<int,int>>> adj;

public:
    Graph(int V = 0) : V(V), adj(V) {}

    int size() const { return V; }
    void resize(int n) { V = n; adj.assign(V, {}); }

    void addEdge(int u, int v, int w, bool directed = false) {
        if (u < 0 || v < 0 || u >= V || v >= V) return;
        adj[u].push_back({v, w});
        if (!directed) adj[v].push_back({u, w});
    }

    void updateWeight(int u, int v, int newWeight) {
        if (u < 0 || v < 0 || u >= V || v >= V) return;
        for (auto &p : adj[u]) if (p.first == v) p.second = newWeight;
        for (auto &p : adj[v]) if (p.first == u) p.second = newWeight;
    }

    const vector<pair<int,int>>& getAdj(int u) const { return adj[u]; }

    void printGraph() const {
        for (int i = 0; i < V; i++) {
            cout << i << " -> ";
            for (auto &p : adj[i]) cout << "(" << p.first << ", " << p.second << ") ";
            cout << "\n";
        }
    }

    vector<int> dijkstra(int src, int dest) const {
        if (src < 0 || dest < 0 || src >= V || dest >= V) return {};
        vector<int> dist(V, INT_MAX), parent(V, -1);
        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;

        dist[src] = 0;
        pq.push({0, src});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) continue;
            if (u == dest) break;

            for (auto &[v, w] : adj[u]) {
                if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }

        vector<int> path;
        if (parent[dest] == -1 && src != dest) return path;
        for (int at = dest; at != -1; at = parent[at]) path.push_back(at);
        reverse(path.begin(), path.end());
        return path;
    }

    vector<vector<int>> floydWarshall() const {
        const int INF = 1e9;
        vector<vector<int>> dist(V, vector<int>(V, INF));
        for (int i = 0; i < V; i++) dist[i][i] = 0;
        for (int u = 0; u < V; u++)
            for (auto &[v, w] : adj[u])
                dist[u][v] = min(dist[u][v], w);

        for (int k = 0; k < V; k++)
            for (int i = 0; i < V; i++)
                if (dist[i][k] < INF)
                    for (int j = 0; j < V; j++)
                        if (dist[k][j] < INF)
                            dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
        return dist;
    }

    vector<int> aStar(int src, int dest) const {
        if (src < 0 || dest < 0 || src >= V || dest >= V) return {};
        auto h = [&](int u, int v) { return abs(u - v); };

        vector<int> gScore(V, INT_MAX), fScore(V, INT_MAX), parent(V, -1);
        gScore[src] = 0;
        fScore[src] = h(src, dest);

        priority_queue<pair<int,int>, vector<pair<int,int>>, greater<>> pq;
        pq.push({fScore[src], src});

        while (!pq.empty()) {
            auto [f, u] = pq.top(); pq.pop();
            if (u == dest) break;

            for (auto &[v, w] : adj[u]) {
                int tentative = (gScore[u] == INT_MAX ? INT_MAX : gScore[u] + w);
                if (tentative < gScore[v]) {
                    parent[v] = u;
                    gScore[v] = tentative;
                    fScore[v] = gScore[v] + h(v, dest);
                    pq.push({fScore[v], v});
                }
            }
        }

        vector<int> path;
        if (parent[dest] == -1 && src != dest) return path;
        for (int at = dest; at != -1; at = parent[at]) path.push_back(at);
        reverse(path.begin(), path.end());
        return path;
    }

    static Graph generateRandom(int V, int E, int maxW = 20, bool directed = false) {
        Graph g(V);
        srand((unsigned)time(nullptr));
        for (int i = 0; i < E; i++) {
            int u = rand() % V;
            int v = rand() % V;
            while (v == u) v = rand() % V;
            int w = 1 + rand() % maxW;
            g.addEdge(u, v, w, directed);
        }
        return g;
    }

    //Graphviz DOT export 
    void exportDOT(const string& filename, bool directed = false, const vector<int>* path = nullptr) const {
        // Build a quick lookup for path edges if provided
        vector<pair<int,int>> pathEdges;
        if (path && path->size() >= 2) {
            for (size_t i = 1; i < path->size(); ++i) pathEdges.push_back({(*path)[i-1], (*path)[i]});
        }
        auto onPath = [&](int a, int b) {
            for (auto &e : pathEdges) if ((e.first == a && e.second == b) || (!directed && e.first == b && e.second == a)) return true;
            return false;
        };

        FILE* f = fopen(filename.c_str(), "w");
        if (!f) return;
        fprintf(f, "%s G {\n", directed ? "digraph" : "graph");
        const char* conn = directed ? "->" : "--";

        // Avoid duplicate undirected edges by enforcing u < v for printing
        for (int u = 0; u < V; ++u) {
            fprintf(f, "  %d;\n", u);
            for (auto &[v, w] : adj[u]) {
                if (!directed && u > v) continue;
                bool highlight = onPath(u, v);
                if (highlight)
                    fprintf(f, "  %d %s %d [label=\"%d\", color=red, penwidth=2.2];\n", u, conn, v, w);
                else
                    fprintf(f, "  %d %s %d [label=\"%d\"];\n", u, conn, v, w);
            }
        }
        fprintf(f, "}\n");
        fclose(f);
    }
};
