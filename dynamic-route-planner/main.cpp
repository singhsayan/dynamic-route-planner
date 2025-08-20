#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <chrono>
#include <sstream>
#include "graph.cpp"

using namespace std;

// ---- Load "graph.txt" with first line: V E, then lines: u v w ----
Graph loadGraphTXT(const string &filename, bool directed = false) {
    ifstream file(filename);
    if (!file.is_open()) throw runtime_error("Could not open file: " + filename);

    int V, E;
    file >> V >> E;
    Graph g(V);

    int u, v, w;
    while (file >> u >> v >> w) g.addEdge(u, v, w, directed);
    return g;
}

// ---- Load CSV: each line "u,v,w". V inferred from max node id ----
Graph loadGraphCSV(const string& filename, bool directed = false) {
    ifstream in(filename);
    if (!in.is_open()) throw runtime_error("Could not open CSV: " + filename);

    vector<tuple<int,int,int>> edges;
    string line;
    int maxNode = -1;

    while (getline(in, line)) {
        if (line.empty()) continue;
        // Skip header lines containing non-digit chars at start
        if (!isdigit(line[0]) && line[0] != '-') continue;

        replace(line.begin(), line.end(), ';', ','); // tolerate ';'
        stringstream ss(line);
        string a,b,c;
        if (!getline(ss, a, ',')) continue;
        if (!getline(ss, b, ',')) continue;
        if (!getline(ss, c, ',')) continue;

        int u = stoi(a), v = stoi(b), w = stoi(c);
        edges.emplace_back(u, v, w);
        maxNode = max(maxNode, max(u, v));
    }
    Graph g(maxNode + 1);
    for (auto &e : edges) {
        int u, v, w; tie(u, v, w) = e;
        g.addEdge(u, v, w, directed);
    }
    return g;
}

void printPath(const vector<int> &path) {
    if (path.empty()) { cout << "(no path)\n"; return; }
    for (size_t i = 0; i < path.size(); i++) {
        cout << path[i];
        if (i + 1 < path.size()) cout << " -> ";
    }
    cout << "\n";
}

void benchmark(Graph &g, int runs = 5) {
    cout << "\nRunning performance benchmark...\n";
    cout << "\n=== Algorithm Comparison Report ===\n";
    cout << "Algorithm              Avg Time (µs)\n";
    cout << "------------------------------------\n";

    int V = g.size();

    // Dijkstra all-pairs
    long long totalD = 0;
    for (int r = 0; r < runs; r++) {
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                g.dijkstra(i, j);
        auto end = chrono::high_resolution_clock::now();
        totalD += chrono::duration_cast<chrono::microseconds>(end - start).count();
    }
    cout << "Dijkstra (all pairs) : " << (totalD / runs) << "\n";

    // Floyd–Warshall
    long long totalF = 0;
    for (int r = 0; r < runs; r++) {
        auto start = chrono::high_resolution_clock::now();
        g.floydWarshall();
        auto end   = chrono::high_resolution_clock::now();
        totalF += chrono::duration_cast<chrono::microseconds>(end - start).count();
    }
    cout << "Floyd-Warshall       : " << (totalF / runs) << "\n";

    // A* all-pairs
    long long totalA = 0;
    for (int r = 0; r < runs; r++) {
        auto start = chrono::high_resolution_clock::now();
        for (int i = 0; i < V; i++)
            for (int j = 0; j < V; j++)
                g.aStar(i, j);
        auto end = chrono::high_resolution_clock::now();
        totalA += chrono::duration_cast<chrono::microseconds>(end - start).count();
    }
    cout << "A* (all pairs)       : " << (totalA / runs) << "\n";
    cout << "------------------------------------\n";
}

int main() {
    Graph g(0);
    bool graphLoaded = false;

    while (true) {
        cout << "\n==== Dynamic Route Planner ====\n";
        cout << "1. Load Graph from TXT (V E + edges)\n";
        cout << "2. Load Graph from CSV (u,v,w)\n";
        cout << "3. Generate Random Graph\n";
        cout << "4. Print Graph\n";
        cout << "5. Run Dijkstra\n";
        cout << "6. Run Floyd-Warshall\n";
        cout << "7. Run A*\n";
        cout << "8. Update Edge Weight\n";
        cout << "9. Benchmark Algorithms\n";
        cout << "10. Export Graph to DOT\n";
        cout << "11. Export Shortest Path to DOT\n";
        cout << "12. Exit\n";
        cout << "Choice: ";

        int choice; 
        if (!(cin >> choice)) break;

        if (choice == 1) {
            string fname; char dir;
            cout << "TXT filename (default graph.txt): ";
            cin >> fname; if (fname == "") fname = "graph.txt";
            cout << "Directed? (y/n): "; cin >> dir;
            g = loadGraphTXT(fname, (dir=='y'||dir=='Y'));
            graphLoaded = true;
            cout << "Graph loaded from TXT!\n";
        }
        else if (choice == 2) {
            string fname; char dir;
            cout << "CSV filename: ";
            cin >> fname;
            cout << "Directed? (y/n): "; cin >> dir;
            g = loadGraphCSV(fname, (dir=='y'||dir=='Y'));
            graphLoaded = true;
            cout << "Graph loaded from CSV!\n";
        }
        else if (choice == 3) {
            int V, E; char dir;
            cout << "Enter V and E: ";
            cin >> V >> E;
            cout << "Directed? (y/n): "; cin >> dir;
            g = Graph::generateRandom(V, E, 20, (dir=='y'||dir=='Y'));
            graphLoaded = true;
            cout << "Random graph generated!\n";
        }
        else if (choice == 4) {
            if (!graphLoaded) { cout << "Load or generate graph first!\n"; continue; }
            g.printGraph();
        }
        else if (choice == 5) {
            if (!graphLoaded) { cout << "Load or generate graph first!\n"; continue; }
            int s, t; cout << "source dest: "; cin >> s >> t;
            auto path = g.dijkstra(s, t);
            cout << "Dijkstra path: "; printPath(path);
        }
        else if (choice == 6) {
            if (!graphLoaded) { cout << "Load or generate graph first!\n"; continue; }
            auto dist = g.floydWarshall();
            cout << "All-Pairs distances (showing INF as 1e9):\n";
            for (int i = 0; i < (int)dist.size(); i++) {
                for (int j = 0; j < (int)dist.size(); j++) {
                    if (dist[i][j] >= 1e9) cout << "INF ";
                    else cout << dist[i][j] << " ";
                }
                cout << "\n";
            }
        }
        else if (choice == 7) {
            if (!graphLoaded) { cout << "Load or generate graph first!\n"; continue; }
            int s, t; cout << "source dest: "; cin >> s >> t;
            auto path = g.aStar(s, t);
            cout << "A* path: "; printPath(path);
        }
        else if (choice == 8) {
            if (!graphLoaded) { cout << "Load or generate graph first!\n"; continue; }
            int u, v, w; cout << "u v newWeight: "; cin >> u >> v >> w;
            g.updateWeight(u, v, w);
            cout << "Edge updated.\n";
        }
        else if (choice == 9) {
            if (!graphLoaded) { cout << "Load or generate graph first!\n"; continue; }
            benchmark(g);
        }
        else if (choice == 10) {
            if (!graphLoaded) { cout << "Load or generate graph first!\n"; continue; }
            string out; char dir;
            cout << "Output DOT filename (e.g., graph.dot): ";
            cin >> out;
            cout << "Directed? (y/n): "; cin >> dir;
            g.exportDOT(out, (dir=='y'||dir=='Y'), nullptr);
            cout << "DOT exported. (Use: dot -Tpng " << out << " -o graph.png)\n";
        }
        else if (choice == 11) {
            if (!graphLoaded) { cout << "Load or generate graph first!\n"; continue; }
            int s, t; string out; char dir; int alg;
            cout << "source dest: "; cin >> s >> t;
            cout << "Algorithm (1=Dijkstra, 2=A*): "; cin >> alg;
            vector<int> path = (alg==2) ? g.aStar(s,t) : g.dijkstra(s,t);
            cout << "Path: "; printPath(path);
            cout << "Output DOT filename (e.g., sp.dot): "; cin >> out;
            cout << "Directed? (y/n): "; cin >> dir;
            g.exportDOT(out, (dir=='y'||dir=='Y'), &path);
            cout << "DOT with highlighted path exported. (dot -Tpng " << out << " -o sp.png)\n";
        }
        else if (choice == 12) {
            cout << "Exiting...\n";
            break;
        }
        
        else {
            cout << "Invalid choice.\n";
        }
    }
    return 0;
}
