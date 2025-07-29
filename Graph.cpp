#include "Graph.h"
#include <string>
#include <tuple>
#include <vector>
#include <utility>
#include <unordered_map>
#include <list>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <algorithm>
#include <climits>
using namespace std;

void Graph::add_edge(string u, string v, string weight) {
    if (adj_list[u].empty()) {
        num_of_nodes++;
        labels.push_back(u);
    }
    if (adj_list[v].empty()) {
        num_of_nodes++;
        labels.push_back(v);
    }

    adj_list[u].push_back(make_pair(v, weight));
    adj_list[v].push_back(make_pair(u, weight));

    num_of_edges++;
}

Graph::Graph(const char* const & edgelist_csv_fn) {
    // TODO
    file_name = edgelist_csv_fn;
    ifstream file(edgelist_csv_fn);
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string first, second, third;
        getline(ss, first, ',');
        getline(ss, second, ',');
        getline(ss, third, '\n');

        add_edge(first, second, third);
    }

    file.close();
}

unsigned int Graph::num_nodes() {
    // TODO
    return num_of_nodes;
}

vector<string> Graph::nodes() {
    // TODO
    return labels;
}

unsigned int Graph::num_edges() {
    // TODO
    return num_of_edges;
}

unsigned int Graph::num_neighbors(string const & node_label) {
    // TODO
    return adj_list[node_label].size();

}

double Graph::edge_weight(string const & u_label, string const & v_label) {
    // TODO
    list<pair<string, string>> neighbor_list = adj_list[u_label];

    for (auto const& i : neighbor_list) {
        if (i.first == v_label) {
            return stod(i.second);
        }
    }

    return -1;
}

vector<string> Graph::neighbors(string const & node_label) {
    // TODO
    list<pair<string, string>> neighbor_list = adj_list[node_label];
    vector<string> neighbors;

    for (auto const& i : neighbor_list) {
        neighbors.push_back(i.first);
    }

    return neighbors;
}

vector<string> Graph::shortest_path_unweighted(string const & start_label, string const & end_label) {
    // TODO
    unordered_map<string, bool> is_visited;
    unordered_map<string, int> dist;
    unordered_map<string, string> parent;
    vector<string> shortest_path;
    string curr;

    queue<string> q;

    if (start_label == end_label) {
        shortest_path.push_back(start_label);
        return shortest_path;
    }

    for (int i = 0; i < num_of_nodes; i++) {
        is_visited[labels[i]] = false;
    }

    is_visited[start_label] = true;
    dist[start_label] = 0;
    parent[start_label] = "";
    q.push(start_label);

    while (!(q.empty())) {
        curr = q.front();
        q.pop();

        for (auto const& i : adj_list[curr]) {
            if (!(is_visited[i.first])) {
                dist[i.first] = dist[curr] + 1;
                parent[i.first] = curr;
                is_visited[i.first] = true;
                q.push(i.first);

                if (i.first == end_label) {
                    string current = i.first;
                    while(true) {
                        shortest_path.push_back(current);
                        current = parent[current];

                        if (current == "") {
                            break;
                        }
                    }

                    std::reverse(shortest_path.begin(), shortest_path.end());
                    return shortest_path;
                }
            }
        }
    }

    return shortest_path;
}

vector<tuple<string,string,double>> Graph::shortest_path_weighted(string const & start_label, string const & end_label) {
    // TODO
    unordered_map<string, bool> is_visited;
    unordered_map<string, double> dist;
    unordered_map<string, string> parent;
    vector<tuple<string,string,double>> shortest_path;

    auto sort_lambda = [](tuple<string,string,double> left, tuple<string,string,double> right) {
        double left1 = get<2>(left);
        double right1 = get<2>(right);

        return left1 > right1;
    };

    priority_queue<tuple<string,string,double>, vector<tuple<string,string,double>>, decltype(sort_lambda)> pq(sort_lambda);

    if (start_label == end_label) {
        tuple<string,string,double> my_tuple(start_label,start_label,-1);
        shortest_path.push_back(my_tuple);
        return shortest_path;
    }

    for (int i = 0; i < num_of_nodes; i++) {
        is_visited[labels[i]] = false;
        dist[labels[i]] = INT_MAX;
        parent[labels[i]] = "";
    }

    //is_visited[start_label] = true;
    tuple<string,string,double> starting_vertex("",start_label,0);
    dist[start_label] = 0;
    parent[start_label] = "";
    pq.push(starting_vertex);

    while (!(pq.empty())) {
        tuple<string,string,double> current = pq.top();
        pq.pop();

        if (!(is_visited[get<1>(current)])) {
            is_visited[get<1>(current)] = true;

            for (auto const& i : adj_list[get<1>(current)]) {
                double total_dist = dist[get<1>(current)] + edge_weight(get<1>(current), i.first);

                if (total_dist < dist[i.first]) {
                    parent[i.first] = get<1>(current);
                    dist[i.first] = total_dist;
                    tuple<string,string,double> newly_added(get<1>(current),i.first,total_dist);
                    pq.push(newly_added);
                }
            }
        }
    }

    if (parent[end_label] == "") {
        vector<tuple<string,string,double>> empty;
        return empty;
    }

    tuple<string,string,double> curr(parent[end_label],end_label,edge_weight(parent[end_label], end_label));
    string curr_str = end_label;
    shortest_path.push_back(curr);
    while (parent[parent[curr_str]] != "") {
        curr_str = parent[curr_str];
        curr = make_tuple(parent[curr_str],curr_str,edge_weight(parent[curr_str], curr_str));
        shortest_path.push_back(curr);
    }

    reverse(shortest_path.begin(), shortest_path.end());

    return shortest_path;
}

void Graph::DFS_CC(string vertex, unordered_map<string, bool>& visited, vector<string>& component, double threshold) {
    visited[vertex] = true;
    component.push_back(vertex);

    for (auto const& i : adj_list[vertex]) {
        double weight = edge_weight(vertex, i.first);

        if (weight <= threshold && !visited[i.first]) {
            DFS_CC(i.first, visited, component, threshold);
        }
    }
}

vector<vector<string>> Graph::connected_components(double const & threshold) {
    // TODO
    vector<vector<string>> components;
    unordered_map<string, bool> is_visited;

    for (int i = 0; i < num_of_nodes; i++) {
        is_visited[labels[i]] = false;
    }

    for (auto const& i : labels) {
        if (!is_visited[i]) {
            vector<string> component;
            DFS_CC(i, is_visited, component, threshold);
            components.push_back(component);
        }
    }

    return components;
}

string Graph::find(string s) {
    if (parent_dj[s] == "") {
        return s;
    }
    else {
        return find(parent_dj[s]);
    }
}

void Graph::union_dj(string l, string r) {
    string l_rep = find(l);
    string r_rep = find(r);

    if (l_rep != r_rep) {
        parent_dj[l_rep] = r_rep;
    }
}

double Graph::smallest_connecting_threshold(string const & start_label, string const & end_label) {
    // TODO

    for (int i = 0; i < num_of_nodes; i++) {
        parent_dj[labels[i]] = "";
    }

    auto sort_lambda = [](tuple<string,string,double> left, tuple<string,string,double> right) {
        if (get<2>(left) == get<2>(right)) {

            if (get<0>(left) == get<0>(right)) {
                return get<1>(left) > get<1>(right);
            }

            return get<0>(left) > get<0>(right);
        }

        return get<2>(left) > get<2>(right);
    };

    priority_queue<tuple<string,string,double>, vector<tuple<string,string,double>>, decltype(sort_lambda)> edge_list(sort_lambda);

    ifstream file(file_name);
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        string first, second, third_s;
        double third;
        getline(ss, first, ',');
        getline(ss, second, ',');
        getline(ss, third_s, '\n');
        third = stod(third_s);

        tuple<string,string,double> edge(first,second,third);
        edge_list.push(edge);
    }

    file.close();   

    if (start_label == end_label) {
        return 0;
    }

    while(!(edge_list.empty())) {
        tuple<string,string,double> current = edge_list.top();
        edge_list.pop();

        union_dj(get<0>(current), get<1>(current));

        if (find(start_label) == find(end_label)) {
            return get<2>(current);
        }
    }

    return -1;
}
