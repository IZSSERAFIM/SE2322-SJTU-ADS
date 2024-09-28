#include <iostream>
#include "base.hpp"
#include "../util/util.hpp"
#include "../util/parameter.hpp"
#include <vector>
#include <cstring>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <random>
#include <cassert>
#include <algorithm>

namespace HNSWLab {

    class HNSW : public AlgorithmInterface {
    private:
        int enter_point = -1, enter_level = -1;

        std::vector<int> search_layer(const int *q, int ep, int ef, int lc);

        std::vector<int> select_neighbors(const int *q, std::vector<int> &W, int M, int lc);

        void init_neighbors(int l, int maxL, int label);

        std::vector<std::vector<std::vector<int>>> neighbors;

        std::unordered_map<int, const int *> Node;
    public:
        // you can add more parameter to initialize HNSW
        HNSW() {}

        void insert(const int *item, int label);

        std::vector<int> query(const int *query, int k);

        ~HNSW() {}

    };

    /**
     * input:
     * iterm: the vector to be inserted
     * label: the label(id) of the vector
    */
    void HNSW::insert(const int *item, int label) {
        //TODO
        Node[label] = item;
        std::vector<int> W;
        int ep = enter_point;
        int maxL = enter_level;
        int L = get_random_level();
        int lc;
        init_neighbors(L, maxL, label);
        if (enter_level == -1) {
            enter_level = L;
            enter_point = label;
            return;
        }
        for (lc = maxL; lc > L; lc--) {
            W.push_back(search_layer(item, ep, 1, lc).back());
            for (int i = 0; i < W.size(); i++) {
                if (l2distance(Node[W[i]], item, 128) < l2distance(Node[ep], item, 128)) {
                    ep = W[i];
                }
            }
        }
        for (lc = std::min(maxL, L); lc >= 0; lc--) {
            W = search_layer(item, ep, ef_construction, lc);
            neighbors[lc][label] = select_neighbors(item, W, M, lc);
            for (int i = 0; i < neighbors[lc][label].size(); i++) {
                int e = neighbors[lc][label][i];
                neighbors[lc][e].push_back(label);
                if (neighbors[lc][e].size() > M_max) {
                    neighbors[lc][e] = select_neighbors(Node[e], neighbors[lc][e], M_max, lc);
                }
            }
            ep = W.back();
        }
        if (L > maxL) {
            enter_point = label;
            enter_level = L;
        }
    }

    /**
     * input:
     * query: the vector to be queried
     * k: the number of nearest neighbors to be returned
     *
     * output:
     * a vector of labels of the k nearest neighbors
    */
    std::vector<int> HNSW::query(const int *query, int k) {
        std::vector<int> res;
        //TODO
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> W;
        int ep = enter_point;
        int maxL = enter_level;
        for (int lc = maxL; lc > 0; lc--) {
            int p = search_layer(query, ep, 100, lc).back();
            W.push(std::make_pair(l2distance(query, Node[p], 128), p));
            ep = W.top().second;
        }
        std::vector<int> l0;
        l0 = search_layer(query, ep, ef_construction, 0);
        for (int i = 0; i < k; i++) {
            res.push_back(l0[l0.size() - 1 - i]);
        }
        return res;
    }

    std::vector<int> HNSW::search_layer(const int *q, int ep, int ef, int lc) {
        std::unordered_map<int, bool> V;
        V[ep] = true;
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::greater<std::pair<int, int>>> C;
        C.push(std::make_pair(l2distance(q, Node[ep], 128), ep));
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int>>, std::less<std::pair<int, int>>> W;
        W.push(std::make_pair(l2distance(q, Node[ep], 128), ep));
        while (!C.empty()) {
            std::pair<int, int> c = C.top();
            C.pop();
            std::pair<int, int> f = W.top();
            if (c.first > f.first) {
                break;
            }
            for (int i = 0; i < neighbors[lc][c.second].size(); i++) {
                if (V.find(neighbors[lc][c.second][i]) == V.end()) {
                    V[neighbors[lc][c.second][i]] = true;
                    if (l2distance(Node[neighbors[lc][c.second][i]], q, 128) < f.first || W.size() < ef) {
                        C.push(std::make_pair(l2distance(Node[neighbors[lc][c.second][i]], q, 128),
                                              neighbors[lc][c.second][i]));
                        W.push(std::make_pair(l2distance(Node[neighbors[lc][c.second][i]], q, 128),
                                              neighbors[lc][c.second][i]));
                        if (W.size() > ef) {
                            W.pop();
                        }
                    }
                }
            }
        }
        std::vector<int> res;
        while (!W.empty()) {
            res.push_back(W.top().second);
            W.pop();
        }
        return res;
    }

    std::vector<int> HNSW::select_neighbors(const int *q, std::vector<int> &W, int M, int lc) {
        std::vector<std::pair<int, int>> distances;
        for (int i = 0; i < W.size(); i++) {
            distances.push_back(std::make_pair(l2distance(Node[W[i]], q, 128), W[i]));
        }

        if (M < distances.size()) {
            std::nth_element(distances.begin(), distances.begin() + M, distances.end());
        }

        std::vector<int> res;
        for (int i = 0; i < M && i < distances.size(); i++) {
            res.push_back(distances[i].second);
        }

        return res;
    }

    void HNSW::init_neighbors(int L, int maxL, int label) {
        int newSize = std::max(L, maxL) + 1;
        neighbors.resize(newSize);
        for (auto &neighbor: neighbors) {
            neighbor.resize(label + 1);
        }
    }
}