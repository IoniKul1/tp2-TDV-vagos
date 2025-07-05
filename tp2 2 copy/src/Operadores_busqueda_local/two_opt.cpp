#include "two_opt.h"
#include <algorithm>

// 2-opt intra-ruta: invierte segmentos para reducir distancia
void apply_2opt(std::vector<Route>& routes, const std::vector<std::vector<double>>& dist) {
    for (auto& route : routes) { // O(r), donde r es la cantidad de rutas
        bool improved = true; // O(1)
        while (improved) { // O(n²)
            improved = false; // O(1)
            int sz = route.nodes.size(); // O(1)
            for (int i = 1; i < sz - 2; ++i) { // O(n)
                for (int j = i + 1; j < sz - 1; ++j) { // O(n)
                    int A = route.nodes[i - 1]; // O(1)
                    int B = route.nodes[i]; // O(1)
                    int C = route.nodes[j]; // O(1)
                    int D = route.nodes[j + 1]; // O(1)

                    double before = dist[A][B] + dist[C][D]; // O(1)
                    double after  = dist[A][C] + dist[B][D]; // O(1)

                    if (after < before) { // O(1)
                        std::reverse(route.nodes.begin() + i, route.nodes.begin() + j + 1); // O(n)
                        improved = true; // O(1)
                    }
                }
            }
        }
    }
}
//O(r) × O(n⁵) = O(r × n⁵)
//Complejidad total: O(n⁶)


