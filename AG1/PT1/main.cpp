#ifndef __PROGTEST__
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum Point : size_t {};

struct Path {
    Point from, to;
    unsigned length;

    Path(size_t f, size_t t, unsigned l) : from{f}, to{t}, length{l} {}

    friend bool operator == (const Path& a, const Path& b) {
        return std::tie(a.from, a.to, a.length) == std::tie(b.from, b.to, b.length);
    }

    friend bool operator != (const Path& a, const Path& b) { return !(a == b); }
};

#endif

void TopSort(int v, std::vector<bool>& visited, std::stack<int>& stack, const std::vector<std::vector<std::pair<int, int>> > &Graph) {
    visited[v] = true;
    for (auto& neighbor : Graph[v]) {
        int u = neighbor.first;
        if (!visited[u]) {
            TopSort(u, visited, stack, Graph);
        }
    }
    stack.push(v);
}
std::vector<Path> longest_track(size_t points, const std::vector<Path>& all_paths){
    //first=to, second=length
    std::vector<std::vector<std::pair<int, int >> > Graph;
    Graph.resize(points);

    std::vector<int> dist(points, -1);
    std::vector<int> parent(points, -1);
    std::vector<bool> visited(points, false);
    std::stack<int> m_stack;

    std::map<std::pair<int,int>,unsigned > dists;

    for (auto i = all_paths.begin(); i !=all_paths.end() ; ++i) {
        if(dists.count({i->from,i->to})){
            if(i->length>dists.at({i->from,i->to})){
                dists[{i->from,i->to}]=i->length;
                for (auto j =  Graph[i->from].begin(); j !=Graph[i->from].end() ; ++j) {
                    if(j->first==(int)i->to){
                        j->second=(int)i->length;
                        break;
                    }
                }

            }
        }
        else{
            dists[{i->from,i->to}]=i->length;
            Graph[i->from].emplace_back(i->to,i->length);


        }


    }

    for (int i = 0; i < (int)  points; i++) {
        if (!visited[i]) {
            TopSort(i, visited, m_stack, Graph);
        }
    }

    while (!m_stack.empty()) {
        int curr = m_stack.top();
        m_stack.pop();
        for (auto& neighbor : Graph[curr]) {
            int n = neighbor.first;
            int weight = neighbor.second;
            if (dist[curr] + weight > dist[n]) {
                dist[n] = dist[curr] + weight;
                parent[n] = curr;
            }
        }
    }

    int max_weight = -10000000;
    int endpoint = -1;


    for (int i = 0; i < (int) points; i++) {
        if (dist[i] > max_weight) {

            max_weight = dist[i];

            endpoint = i;
        }
    }

    std::vector<Path> longestPath;
    while (endpoint != -1) {
        if (parent[endpoint] != -1) {
            longestPath.emplace_back(parent[endpoint], endpoint, dist[endpoint]-dist[parent[endpoint]]);
        }
        endpoint = parent[endpoint];
    }

    reverse(longestPath.begin(), longestPath.end());

    return longestPath;
}

#ifndef __PROGTEST__


struct Test {
    unsigned longest_track;
    size_t points;
    std::vector<Path> all_paths;
};

inline const Test TESTS[] = {
        {13, 5, { {3,2,10}, {3,0,9}, {0,2,3}, {2,4,1} } },
        {11, 5, { {3,2,10}, {3,1,4}, {1,2,3}, {2,4,1} } },
        {16, 8, { {3,2,10}, {3,1,1}, {1,2,3}, {1,4,15} } },
};

#define CHECK(cond, ...) do { \
    if (cond) break; \
    printf("Fail: " __VA_ARGS__); \
    printf("\n"); \
    return false; \
  } while (0)

bool run_test(const Test& t) {
    auto sol = longest_track(t.points, t.all_paths);

    unsigned length = 0;
    for (auto [ _, __, l ] : sol) length += l;

    CHECK(t.longest_track == length,
          "Wrong length: got %u but expected %u", length, t.longest_track);

    for (size_t i = 0; i < sol.size(); i++) {
        CHECK(std::count(t.all_paths.begin(), t.all_paths.end(), sol[i]),
              "Solution contains non-existent path: %zu -> %zu (%u)",
              sol[i].from, sol[i].to, sol[i].length);

        if (i > 0) CHECK(sol[i].from == sol[i-1].to,
                         "Paths are not consecutive: %zu != %zu", sol[i-1].to, sol[i].from);
    }

    return true;
}
#undef CHECK

int main() {
    int ok = 0, fail = 0;

    for (auto&& t : TESTS) (run_test(t) ? ok : fail)++;

    if (!fail) printf("Passed all %i tests!\n", ok);
    else printf("Failed %u of %u tests.\n", fail, fail + ok);
}

#endif
