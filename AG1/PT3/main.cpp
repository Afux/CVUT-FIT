#ifndef __PROGTEST__

#include <cassert>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>

using Price = unsigned long long;
using Employee = size_t;
inline constexpr Employee NO_EMPLOYEE = -1;
using Gift = size_t;

#endif

struct EmployeeNode {
    int id;
    std::vector<EmployeeNode *> subordinates;
    EmployeeNode *parr;
    EmployeeNode *neight;
    Price price;
    EmployeeNode(int id) : id(id),parr(nullptr),neight(nullptr),price(0) {}

};

struct comp {
    bool operator()(const std::pair<int, std::vector<Gift>> &p1,
                    const std::pair<int, std::vector<Gift>> &p2) const {
        return p1.first < p2.first;
    }
};

EmployeeNode *findNode(EmployeeNode *root,int id) {
    if (root == nullptr) {
        return nullptr;
    }
    if (root->id == id) {
        return root;
    }
    for (EmployeeNode* subordinate : root->subordinates) {
        EmployeeNode* ret = findNode(subordinate, id);
        if (ret != nullptr) {
            return ret;
        }
    }
    return nullptr;
}


void deleteTree(EmployeeNode *root) {
    if (root == nullptr) {
        return;
    }

    for (EmployeeNode *subordinate: root->subordinates) {
        deleteTree(subordinate);
    }

    delete root;
}

void preOrder(EmployeeNode *root, std::vector<int> &s){
    if (root == NULL) {
        return;
    }

    s.push_back(root->id);
    for (size_t i = 0; i < root->subordinates.size(); i++) {
        preOrder(root->subordinates[i],s);
    }
    return;

}
void assignValues(EmployeeNode *node, const std::vector<Price> &values,std::set<std::pair<Price , std::vector<Gift>>, comp> &retAll, Price &ret,
                  std::vector<Gift> &assigned_gifts) {
    int counter = 0;
    for (Price p: values) {
        if(node->parr){
            if (p == node->parr->price||(!retAll.empty()&&ret+p>retAll.begin()->first)) {
                counter++;
                continue;
            }
        }
        ret += p;
        node->price=p;
        assigned_gifts[node->id] = counter;
        if(node->neight)
              assignValues(node->neight, values, retAll, ret, assigned_gifts);
        else
            retAll.insert({ret, assigned_gifts});

        ret -= p;
        counter++;
    }
}


EmployeeNode *makeTree(const std::vector<Employee> &bosses, size_t id) {
    EmployeeNode *node = new EmployeeNode(id);
    for (long unsigned int i = 0; i < bosses.size(); ++i) {
        if (bosses[i] == id) {
            EmployeeNode *subordinate = makeTree(bosses, i);
            node->subordinates.push_back(subordinate);
            subordinate->parr=node;
        }
    }
    return node;
}



std::pair<Price, std::vector<Gift>>
optimize_gifts(const std::vector<Employee> &boss, const std::vector<Price> &gift_price) {

    int total_costS = 0;
    std::vector<EmployeeNode *> forest;
     std::vector<Price> sortedGifts=gift_price;
    std::sort(sortedGifts.begin(), sortedGifts.end());

    for (long unsigned int i = 0; i < boss.size(); ++i) {
        if (boss[i] == NO_EMPLOYEE) {
            EmployeeNode *root = makeTree(boss, i);
            forest.push_back(root);
        }
    }
    for (EmployeeNode *root: forest) {
        std::vector<int> neigh;
        if(!root->subordinates.empty()){
            preOrder(root,neigh);

                for (long unsigned int i = 0; i < neigh.size()-1 ; ++i) {
                    findNode(root,neigh[i])->neight=findNode(root,neigh[i+1]);
                }

        }

    }
    std::vector<Gift> assigned_gifts(boss.size(), -1);
    std::vector<Gift> dummyvec(boss.size(), -1);

    for (EmployeeNode *root: forest) {
        std::set<std::pair<Price , std::vector<Gift>>, comp> all;
        Price dumm = 0;
        assignValues(root, sortedGifts, all, dumm, dummyvec);
        if(!all.empty()){
            total_costS += all.begin()->first;
            for (long unsigned int i = 0; i < all.begin()->second.size(); ++i) {
                assigned_gifts[i] = all.begin()->second[i];
            }
        }

    }
    for (EmployeeNode *root: forest) {
        deleteTree(root);
    }
    return std::make_pair(total_costS, assigned_gifts);
}

#ifndef __PROGTEST__

const std::tuple<Price, std::vector<Employee>, std::vector<Price>> EXAMPLES[] = {


     {17, {1,           2,           3,           4, NO_EMPLOYEE},       {25, 4, 18, 3}},

        {16, {4,           4,           4,           4, NO_EMPLOYEE},       {25, 4, 18, 3}},
             {17, {4,           4,           3,           4, NO_EMPLOYEE},       {25, 4, 18, 3}},
            {24, {4,           4,           3,           4, NO_EMPLOYEE, 3, 3}, {25, 4, 18, 3}},
            {3,  {NO_EMPLOYEE},                                                 {3}},
               {21, {NO_EMPLOYEE, NO_EMPLOYEE, NO_EMPLOYEE, 0, 1,           2},    {3,  4}},




};

#define CHECK(cond, ...) do { \
    if (cond) break; \
    printf("Test failed: " __VA_ARGS__); \
    printf("\n"); \
    return false; \
  } while (0)

bool test(Price p, const std::vector<Employee> &boss, const std::vector<Price> &gp) {
    auto&&[sol_p, sol_g] = optimize_gifts(boss, gp);
    CHECK(sol_g.size() == boss.size(),
          "Size of the solution: expected %zu but got %zu.", boss.size(), sol_g.size());

    Price real_p = 0;
    for (Gift g: sol_g) real_p += gp[g];
    CHECK(real_p == sol_p, "Sum of gift prices is %llu but reported price is %llu.", real_p, sol_p);
    if (0) {
        for (Employee e = 0; e < boss.size(); e++) printf(" (%zu)%zu", e, sol_g[e]);
        printf("\n");
    }

    for (Employee e = 0; e < boss.size(); e++)
        CHECK(boss[e] == NO_EMPLOYEE || sol_g[boss[e]] != sol_g[e],
              "Employee %zu and their boss %zu has same gift %zu.", e, boss[e], sol_g[e]);

    CHECK(p == sol_p, "Wrong price: expected %llu got %llu.", p, sol_p);

    return true;
}

#undef CHECK

int main() {
    int ok = 0, fail = 0;
    for (auto&&[p, b, gp]: EXAMPLES) (test(p, b, gp) ? ok : fail)++;

    if (!fail) printf("Passed all %d tests!\n", ok);
    else printf("Failed %d of %d tests.", fail, fail + ok);
}

#endif


