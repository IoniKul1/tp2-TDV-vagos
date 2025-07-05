#include "relocate.h"
#include <iostream>
#include <algorithm>

void apply_relocate(std::vector<Route>& routes,const std::vector<std::vector<double>>& dist,
                    const std::vector<int>& demands, int capacity) {
    bool improved = true; // O(1)

    while (improved) { // O(n²)
        improved = false; // O(1)
        double best_delta = 0.0; // O(1)
        size_t best_from = -1, best_to = -1; // O(1)
        size_t best_i = -1, best_j = -1; // O(1)

        for (size_t from_idx = 0; from_idx < routes.size(); ++from_idx) { // O(r), con r = cantidad de rutas activas (puede ser hasta n en el peor caso)
            for (size_t to_idx = 0; to_idx < routes.size(); ++to_idx) { // O(r)
                if (from_idx == to_idx) continue; // O(1)

                Route& from = routes[from_idx]; // O(1)
                Route& to = routes[to_idx]; // O(1)

                for (size_t i = 1; i < from.nodes.size() - 1; ++i) { // O(c)
                    int client = from.nodes[i]; // O(1)
                    int demand = demands[client]; // O(1)

                    if (to.total_demand + demand > capacity) continue; // O(1)

                    for (size_t j = 1; j < to.nodes.size(); ++j) {  // O(c), con c = cantidad de clientes en la ruta (≲ n)
                        int u = to.nodes[j - 1]; // O(1)
                        int v = to.nodes[j]; // O(1)

                        double delta =
                            - dist[from.nodes[i - 1]][client]
                            - dist[client][from.nodes[i + 1]]
                            + dist[from.nodes[i - 1]][from.nodes[i + 1]]
                            - dist[u][v]
                            + dist[u][client] + dist[client][v]; // O(1)

                        if (delta < best_delta) { // O(1)
                            best_delta = delta; // O(1)
                            best_from = from_idx; // O(1)
                            best_to = to_idx; // O(1)
                            best_i = i; // O(1)
                            best_j = j; // O(1)
                            improved = true; // O(1)
                        }
                    }
                }
            }
        }

        if (improved) { // O(1)
            Route& from = routes[best_from]; // O(1)
            Route& to = routes[best_to]; // O(1)

            int client = from.nodes[best_i]; // O(1)
            int demand = demands[client]; // O(1)

            from.nodes.erase(from.nodes.begin() + best_i); // O(c)
            from.total_demand -= demand; // O(1)

            to.nodes.insert(to.nodes.begin() + best_j, client);  // O(c)
            to.total_demand += demand; // O(1)

        }
    }

    // Eliminar rutas vacías
    routes.erase( 
        std::remove_if(routes.begin(), routes.end(),
            [](const Route& r) { return r.nodes.size() <= 2; }),
        routes.end() // O(r)
    );
}
//O(n² × n² × n²) = O(n⁶), Considerando que r ≤ n y c ≤ n:
//Complejidad total: O(n⁶)