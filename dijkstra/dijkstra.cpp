#include <algorithm>
#include <sstream>
#include <fstream>
#include <cstring>

#include "dijkstra.h"


std::tuple<const char*, dijkstra::node_name_t, dijkstra::node_name_t>
        dijkstra::parse_args(int arg_count, char* arg_vars[]) {
    using namespace std::string_literals;

    if (arg_count != 7) {
        throw dijkstra::DijkstraException("Invalid number of arguments");
    }
    auto equal = [](const char *s1, const char *s2) { return strcmp(s1, s2) == 0; };
    auto hash = [](const char *s) { return strlen(s); };
    std::unordered_map<const char *, const char *, decltype(hash), decltype(equal)> args(3, hash, equal);
    args = {{"--file",       nullptr},
            {"--from", nullptr},
            {"--to",    nullptr}};
    for (size_t i = 1; i < 7; i += 2) {
        auto it = args.find(arg_vars[i]);
        if (it == args.end()) {
            throw dijkstra::DijkstraException("Invalid argument: "s + arg_vars[i]);
        }
        if (it->second) {
            throw dijkstra::DijkstraException("Dublicated argument: "s + arg_vars[i]);
        }
        it->second = arg_vars[i + 1];
    }
    dijkstra::node_name_t from;
    dijkstra::node_name_t to;

    std::stringstream sstream;
    sstream << args["--from"] << ' ' << args["--to"];
    sstream >> from >> to;
    if (!sstream.eof())
        throw dijkstra::DijkstraException("Invalid parameters of arguments");

    return {args["--file"], from, to};
}

void parse_string(std::string& line) {
    line.erase(line.begin(), std::find_if_not(line.begin(), line.end(), [](char s){return s == '|';}));
    line.erase(std::find_if_not(line.rbegin(), line.rend(), [](char s){return s == '|';}).base(), line.end());
}

size_t insert_nodes(dijkstra::graph_t& graph, std::string& line) {
    std::stringstream sstream(line);
    dijkstra::weight_t weight = 0;
    size_t nodes_count = 0;
    while (sstream >> weight) {
        graph.insert_node(nodes_count, {});
        ++nodes_count;
    }
    return nodes_count;
}

size_t insert_edges(dijkstra::graph_t& graph, std::string& line, size_t row) {
    std::stringstream sstream(line);
    dijkstra::weight_t weight = 0;
    size_t to = 0;
    while (sstream >> weight) {
        if (weight < 0)
            throw dijkstra::DijkstraException("Invalid weight");
        if (weight > 0) {
            graph.insert_edge({row, to}, weight);
        }
        ++to;
    }
    return to;
}

dijkstra::graph_t dijkstra::read_graph(const char* filename) {
    using namespace std::string_literals;

    std::fstream file(filename);
    if (!file.is_open()) throw dijkstra::DijkstraException("File not found: "s + filename);

    std::string line;
    size_t row_length = 0;
    size_t row = 0;

    graph_t graph;

    for (; std::getline(file, line); ++row) {
        parse_string(line);
        if (line.empty()) continue;

        if (row == 0)
            row_length = insert_nodes(graph, line);

        if (row_length != insert_edges(graph, line, row))
            throw dijkstra::DijkstraException("Not a square Matrix");
    }
    if (row != row_length)
        throw dijkstra::DijkstraException("Not a square Matrix");

    return graph;
}

void dijkstra::print_route(const weight_t& weight, const route_t& route) {
    std::cout << "route:";
    for (auto const& key: route)
        std::cout << " " << key;
    std::cout << std::endl;
    std::cout << "weight: " << weight << std::endl;
}

struct node_distance_comparator {
    bool operator()(std::pair<dijkstra::node_name_t, dijkstra::graph_t::Node> left,
            std::pair<dijkstra::node_name_t, dijkstra::graph_t::Node> right) {
        if (left.second.getvalue().visited)
            return false;
        if (right.second.getvalue().visited)
            return true;
        return left.second.getvalue().distance < right.second.getvalue().distance;
    }
};

void find_shortest_paths(dijkstra::graph_t& graph) {
    auto min_node = std::min_element(graph.begin(), graph.end(), node_distance_comparator());
    while (!min_node->second.getvalue().visited) {
        min_node->second.getvalue().visited = true;
        for (auto& edge : min_node->second.getedges()) {
            auto& node = graph[edge.first];
            if (edge.second < 0)
                throw dijkstra::DijkstraException("Invalid weight");

            auto new_distance = min_node->second.getvalue().distance + edge.second;
            if (!node.getvalue().visited && new_distance < node.getvalue().distance) {
                node.getvalue().distance = new_distance;
                node.getvalue().previous = min_node->first;
                node.getvalue().changed = true;
            }
        }
        min_node = std::min_element(graph.begin(), graph.end(), node_distance_comparator());
    }
}

std::pair<dijkstra::weight_t , dijkstra::route_t>
        find_route(const dijkstra::graph_t& graph, dijkstra::node_name_t key_from, dijkstra::node_name_t key_to) {
    auto it_from = graph.find(key_from);
    auto it_to = graph.find(key_to);
    dijkstra::weight_t route_length = it_to->second.getvalue().distance;
    if (route_length == std::numeric_limits<dijkstra::weight_t>::infinity())
        throw dijkstra::DijkstraException("No route found");

    dijkstra::route_t route;

    for (auto it = it_to; it != it_from || it->second.getvalue().changed;) {
        route.push_back(it->first);
        it = graph.find(it->second.getvalue().previous);
    }
    route.push_back(it_from->first);
    std::reverse(route.begin(), route.end());

    return { route_length, route };
}

std::pair<dijkstra::weight_t , dijkstra::route_t>
dijkstra::dijkstra_algorithm(dijkstra::graph_t& graph, dijkstra::node_name_t key_from, dijkstra::node_name_t key_to) {
    auto it_from = graph.find(key_from);
    auto it_to = graph.find(key_to);

    if (it_from == graph.end() || it_to == graph.end())
        throw dijkstra::DijkstraException("Invalid node name");

    it_from->second.getvalue().distance = 0;

    find_shortest_paths(graph);
    return find_route(graph, key_from, key_to);
}
