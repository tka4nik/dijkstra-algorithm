#pragma once

#include <limits>
#include <vector>
#include "Graph.h"

namespace dijkstra {

    class DijkstraException : public std::runtime_error {
    public:
        explicit DijkstraException(const std::string& message) : std::runtime_error(message) {}
    };

    using node_name_t = size_t;
    using weight_t = double;
    struct NodeData {
        bool visited = false;
        weight_t distance = std::numeric_limits<weight_t>::infinity();
        node_name_t previous = 0;
        bool changed = false;
    };
    using node_data_t = NodeData;
    using graph_t = graph::Graph<node_name_t, node_data_t, weight_t>;
    using route_t = std::vector<node_name_t>;

    std::tuple<const char*, node_name_t, node_name_t> parse_args(int arg_count, char* arg_vars[]);
    graph_t read_graph(const char* filename);
    std::pair<weight_t, route_t> dijkstra_algorithm(graph_t& graph, node_name_t key_from, node_name_t key_to);
    void print_route(const weight_t& weight, const route_t& route);
}
