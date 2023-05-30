#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <omp.h>

// Graph class
class Graph {
    int V; // Number of vertices
    std::vector<std::vector<int>> adj; // Adjacency list

public:
    // Constructor
    Graph(int vertices) : V(vertices) {
        adj.resize(V);
    }

    // Add an edge to the graph
    void addEdge(int u, int v) {
        adj[u].push_back(v);
    }

    // Sequential BFS
    void sequentialBFS(int startVertex) {
        std::vector<bool> visited(V, false);
        std::queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            int currentVertex = q.front();
            std::cout << currentVertex << " ";
            q.pop();

            for (int adjacentVertex : adj[currentVertex]) {
                if (!visited[adjacentVertex]) {
                    visited[adjacentVertex] = true;
                    q.push(adjacentVertex);
                }
            }
        }
    }

    // Parallel BFS
    void parallelBFS(int startVertex) {
        std::vector<bool> visited(V, false);
        std::queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        while (!q.empty()) {
            #pragma omp parallel
            {
                #pragma omp for
                for (int i = 0; i < q.size(); ++i) {
                    int currentVertex;
                    #pragma omp critical
                    {
                        currentVertex = q.front();
                        q.pop();
                    }

                    std::cout << currentVertex << " ";

                    for (int adjacentVertex : adj[currentVertex]) {
                        if (!visited[adjacentVertex]) {
                            #pragma omp critical
                            {
                                visited[adjacentVertex] = true;
                                q.push(adjacentVertex);
                            }
                        }
                    }
                }
            }
        }
    }
};

// Function to generate a random graph
Graph generateRandomGraph(int numVertices, int numEdges) {
    srand(time(0));
    Graph graph(numVertices);

    for (int i = 0; i < numEdges; ++i) {
        int u = rand() % numVertices;
        int v = rand() % numVertices;
        graph.addEdge(u, v);
    }

    return graph;
}

int main() {
    const int numVertices = 1000; // Number of vertices in the graph
    const int numEdges = 5000; // Number of edges in the graph

    // Generate random graph
    Graph graph = generateRandomGraph(numVertices, numEdges);

    // Sequential BFS
    clock_t startTime = clock();
    std::cout << "Sequential BFS: ";
    graph.sequentialBFS(0); // Perform BFS starting from vertex 0
    std::cout << std::endl;
    clock_t endTime = clock();
    double sequentialTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

    std::cout << "Execution time (Sequential BFS): " << sequentialTime << " seconds" << std::endl;

    // Parallel BFS
    startTime = clock();
    std::cout << "\nParallel BFS: ";
    graph.parallelBFS(0); // Perform BFS starting from vertex 0
    std::cout << std::endl;
    endTime = clock();
    double parallelTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

    std::cout << "Execution time (Parallel BFS): " << parallelTime << " seconds" << std::endl;

    return 0;
}
