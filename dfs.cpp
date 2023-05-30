#include <iostream>
#include <vector>
#include <stack>
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

    // Sequential DFS
    void sequentialDFS(int startVertex) {
        std::vector<bool> visited(V, false);
        std::stack<int> stk;

        stk.push(startVertex);

        while (!stk.empty()) {
            int currentVertex = stk.top();
            stk.pop();

            if (!visited[currentVertex]) {
                std::cout << currentVertex << " ";
                visited[currentVertex] = true;

                for (int adjacentVertex : adj[currentVertex]) {
                    if (!visited[adjacentVertex]) {
                        stk.push(adjacentVertex);
                    }
                }
            }
        }
    }

    // Parallel DFS
    void parallelDFS(int startVertex) {
        std::vector<bool> visited(V, false);
        std::stack<int> stk;

        stk.push(startVertex);

        while (!stk.empty()) {
            #pragma omp parallel
            {
                #pragma omp for
                for (int i = 0; i < stk.size(); ++i) {
                    int currentVertex;
                    #pragma omp critical
                    {
                        currentVertex = stk.top();
                        stk.pop();
                    }

                    if (!visited[currentVertex]) {
                        #pragma omp critical
                        {
                            std::cout << currentVertex << " ";
                            visited[currentVertex] = true;
                        }

                        for (int adjacentVertex : adj[currentVertex]) {
                            if (!visited[adjacentVertex]) {
                                #pragma omp critical
                                {
                                    stk.push(adjacentVertex);
                                }
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

    // Sequential DFS
    clock_t startTime = clock();
    std::cout << "Sequential DFS: ";
    graph.sequentialDFS(0); // Perform DFS starting from vertex 0
    std::cout << std::endl;
    clock_t endTime = clock();
    double sequentialTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

    std::cout << "Execution time (Sequential DFS): " << sequentialTime << " seconds" << std::endl;

    // Parallel DFS
    startTime = clock();
    std::cout << "\nParallel DFS: ";
    graph.parallelDFS(0); // Perform DFS starting from vertex 0
    std::cout << std::endl;
    endTime = clock();
    double parallelTime = static_cast<double>(endTime - startTime) / CLOCKS_PER_SEC;

    std::cout << "Execution time (Parallel DFS): " << parallelTime << " seconds" << std::endl;

    return 0;
}
