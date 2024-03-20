#include <iostream>
#include "dijkstra.h"

int main(int arg_count, char *arg_vars[]) {
    /*
    dijkstra::graph_t graph1;
    graph1.insert_node(0,{});
    graph1.insert_node(2, {});
    graph1.insert_node(4, {});
    graph1.insert_node(3, {});
    graph1.insert_node(1, {});
    graph1.insert_edge({0,0},5.0);
    graph1.insert_edge({2,0}, 1.2);
    graph1.insert_edge({2,3}, 15.0);
    graph1.insert_edge({2,4}, 10.5);
    graph1.insert_edge({4,3}, 1.0);
    graph1.insert_edge({4,1}, 8.0);
    graph1.insert_edge({3,1}, 3.0);
    graph1.insert_edge({1,3}, 2.0);
     */

    /*
    auto [weight, route] = dijkstra::dijkstra_algorithm(graph1,2,1);
    dijkstra::print_route(weight, route);
     */

    try {
        auto [file, from, to] = dijkstra::parse_args(arg_count, arg_vars);
        dijkstra::graph_t graph = dijkstra::read_graph(file);
        auto [route, vec] = dijkstra::dijkstra_algorithm(graph, from, to);
        dijkstra::print_route(route, vec);
    } catch (std::exception& error) {
        std::cout << error.what();
        return 2;
    }

}
