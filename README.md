# Dynamic Route Planner

## Overview
**Dynamic Route Planner** is a C++ project that demonstrates classic graph algorithms with support for **dynamic updates**, **benchmarking**, and **visualization**.  
It is designed to showcase concepts applicable to real-world **transit systems** and **network routing**.  

The system supports multiple input formats, random graph generation, shortest path computation using different algorithms, performance analysis, and graph export for visualization.  

---

## System Architecture
<p align="center">
  <img src="screenshot/diagram.png" alt="System Diagram" width="650"/>
</p>

---

## Features

### Graph Input
- Load graphs from **TXT files** (`V E + edges` format)  
- Load graphs from **CSV files** (`u, v, w` format)  
- Generate random graphs with configurable size  

### Graph Operations
- Print adjacency list representation  
- Update edge weights dynamically to simulate real-time changes  

### Algorithms Implemented
- **Dijkstra’s Algorithm** → Single-source shortest path  
- **Floyd–Warshall Algorithm** → All-pairs shortest path  
- **A\* Search Algorithm** → Heuristic-based pathfinding  

### Performance Analysis
- Benchmark execution time of each algorithm  
- Generate a **comparative analysis table**  

### Visualization
- Export graph to **DOT format** for visualization with Graphviz  
- Export computed shortest path to DOT format (render as PNG/PDF)  

---

## Example Workflow

### 1. Load Graph
<p align="center">
  <img src="screenshot/Load_graph.png" alt="Load Graph" width="600"/>
</p>

### 2. Generate Random Graph
<p align="center">
  <img src="screenshot/Random_graph_generate.png" alt="Random Graph" width="600"/>
</p>

### 3. Print Graph
<p align="center">
  <img src="screenshot/print_graph.png" alt="Print Graph" width="600"/>
</p>

### 4. Run Dijkstra
<p align="center">
  <img src="screenshot/Run_dijkstra.png" alt="Run Dijkstra" width="600"/>
</p>

### 5. Run Floyd–Warshall
<p align="center">
  <img src="screenshot/floyd_warshall.png" alt="Floyd Warshall" width="600"/>
</p>

### 6. Run A\* Search
<p align="center">
  <img src="screenshot/run_a_star.png" alt="Run A Star" width="600"/>
</p>

### 7. Benchmark Algorithms
<p align="center">
  <img src="screenshot/Benchmark_comparison.png" alt="Benchmark Comparison" width="600"/>
</p>

---

## Technical Details
- **Language**: C++  
- **Libraries**: Standard Template Library (STL), `<chrono>` for benchmarking  
- **External Tools**: Graphviz (for visualization)  
- **Build**: Compatible with `g++` / `clang++`  

---

## How to Build and Run

### Compilation and Execution
```bash
g++ main.cpp graph.cpp -o planner
./planner
```

### Visualization with Graphviz
``` bash
dot -Tpng graph.dot -o graph.png
dot -Tpng sp.dot -o path.png
