#include "relocate.h"
#include <iostream>
#include <algorithm>

void apply_relocate(std::vector<Route>& routes,const std::vector<std::vector<double>>& dist,
                    const std::vector<int>& demands, int capacity) {
    bool improved = true;

    while (improved) {
        improved = false;
        double best_delta = 0.0;
        size_t best_from = -1, best_to = -1;
        size_t best_i = -1, best_j = -1;

        for (size_t from_idx = 0; from_idx < routes.size(); ++from_idx) {
            for (size_t to_idx = 0; to_idx < routes.size(); ++to_idx) {
                if (from_idx == to_idx) continue;

                Route& from = routes[from_idx];
                Route& to = routes[to_idx];

                for (size_t i = 1; i < from.nodes.size() - 1; ++i) {
                    int client = from.nodes[i];
                    int demand = demands[client];

                    if (to.total_demand + demand > capacity) continue;

                    for (size_t j = 1; j < to.nodes.size(); ++j) {
                        int u = to.nodes[j - 1];
                        int v = to.nodes[j];

                        double delta =
                            - dist[from.nodes[i - 1]][client]
                            - dist[client][from.nodes[i + 1]]
                            + dist[from.nodes[i - 1]][from.nodes[i + 1]]
                            - dist[u][v]
                            + dist[u][client] + dist[client][v];

                        if (delta < best_delta) {
                            best_delta = delta;
                            best_from = from_idx;
                            best_to = to_idx;
                            best_i = i;
                            best_j = j;
                            improved = true;
                        }
                    }
                }
            }
        }

        if (improved) {
            Route& from = routes[best_from];
            Route& to = routes[best_to];

            int client = from.nodes[best_i];
            int demand = demands[client];

            from.nodes.erase(from.nodes.begin() + best_i);
            from.total_demand -= demand;

            to.nodes.insert(to.nodes.begin() + best_j, client);
            to.total_demand += demand;

        }
    }

    // Eliminar rutas vacías
    routes.erase(
        std::remove_if(routes.begin(), routes.end(),
            [](const Route& r) { return r.nodes.size() <= 2; }),
        routes.end()
    );
}
