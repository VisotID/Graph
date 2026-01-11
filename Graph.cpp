#include <iostream>
#include "Class_Graph.h"

    int main()
    {
        test_vertex_operations();
        test_edge_operations();
        test_bfs();
        test_dfs();
        test_bellman_ford();
        test_get_neighbors();
        test_get_weight();
        test_saveGraphML();
        test_loadGraphML();
        std::cout << "Hello, World!";
}