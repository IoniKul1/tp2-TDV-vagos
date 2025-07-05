#include "swap.h"
#include <limits>
#include <iostream>
#include <algorithm>

void apply_swap(std::vector<Route>& routes,const std::vector<std::vector<double>>& dist,const std::vector<int>& demands,int capacity) {
    bool improved = true; // O(1)
    while (improved) { // O(n²)
        improved = false; // O(1)

        for (size_t i = 0; i < routes.size(); ++i) { // O(r)
            for (size_t j = i + 1; j < routes.size(); ++j) { // O(r)
                Route& r1 = routes[i]; // O(1)
                Route& r2 = routes[j]; // O(1)

                for (size_t idx1 = 1; idx1 < r1.nodes.size() - 1; ++idx1) { // O(n)
                    for (size_t idx2 = 1; idx2 < r2.nodes.size() - 1; ++idx2) { // O(n)
                        int c1 = r1.nodes[idx1]; // O(1)
                        int c2 = r2.nodes[idx2]; // O(1)
                        int d1 = demands[c1]; // O(1)
                        int d2 = demands[c2]; // O(1)

                        if (r1.total_demand - d1 + d2 > capacity ||
                            r2.total_demand - d2 + d1 > capacity) continue; // O(1)

                        // Costo actual
                        int a1 = r1.nodes[idx1 - 1], b1 = r1.nodes[idx1 + 1]; // O(1)
                        int a2 = r2.nodes[idx2 - 1], b2 = r2.nodes[idx2 + 1]; // O(1)

                        double cost_before =
                            dist[a1][c1] + dist[c1][b1] +
                            dist[a2][c2] + dist[c2][b2]; // O(1)

                        double cost_after =
                            dist[a1][c2] + dist[c2][b1] +
                            dist[a2][c1] + dist[c1][b2]; // O(1)

                        if (cost_after < cost_before) { // O(1)
                            std::swap(r1.nodes[idx1], r2.nodes[idx2]); // O(1)
                            r1.total_demand = r1.total_demand - d1 + d2; // O(1)
                            r2.total_demand = r2.total_demand - d2 + d1; // O(1)
                            improved = true; // O(1)
                            goto next_iteration; // O(1)
                        }
                    }
                }
            }
        }
        next_iteration:; // O(1)
    }
}
//while iterations × work per iteration
//= O(r² × n²) × O(r² × n²)
//= O(r⁴ × n⁴) como r <= n 
//Complejidad total: O(n⁴)