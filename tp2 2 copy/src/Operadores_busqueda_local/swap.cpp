#include "swap.h"
#include <limits>
#include <iostream>
#include <algorithm>

void apply_swap(std::vector<Route>& routes,const std::vector<std::vector<double>>& dist,const std::vector<int>& demands,int capacity) {
    bool improved = true;
    while (improved) {
        improved = false;

        for (size_t i = 0; i < routes.size(); ++i) {
            for (size_t j = i + 1; j < routes.size(); ++j) {
                Route& r1 = routes[i];
                Route& r2 = routes[j];

                for (size_t idx1 = 1; idx1 < r1.nodes.size() - 1; ++idx1) {
                    for (size_t idx2 = 1; idx2 < r2.nodes.size() - 1; ++idx2) {
                        int c1 = r1.nodes[idx1];
                        int c2 = r2.nodes[idx2];
                        int d1 = demands[c1];
                        int d2 = demands[c2];

                        if (r1.total_demand - d1 + d2 > capacity ||
                            r2.total_demand - d2 + d1 > capacity) continue;

                        // Costo actual
                        int a1 = r1.nodes[idx1 - 1], b1 = r1.nodes[idx1 + 1];
                        int a2 = r2.nodes[idx2 - 1], b2 = r2.nodes[idx2 + 1];

                        double cost_before =
                            dist[a1][c1] + dist[c1][b1] +
                            dist[a2][c2] + dist[c2][b2];

                        double cost_after =
                            dist[a1][c2] + dist[c2][b1] +
                            dist[a2][c1] + dist[c1][b2];

                        if (cost_after < cost_before) {
                            std::swap(r1.nodes[idx1], r2.nodes[idx2]);
                            r1.total_demand = r1.total_demand - d1 + d2;
                            r2.total_demand = r2.total_demand - d2 + d1;
                            improved = true;
                            goto next_iteration;
                        }
                    }
                }
            }
        }
        next_iteration:;
    }
}
