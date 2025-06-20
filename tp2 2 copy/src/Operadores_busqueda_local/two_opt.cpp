#include "two_opt.h"
#include <algorithm>

// 2-opt intra-ruta: invierte segmentos para reducir distancia
void apply_2opt(std::vector<Route>& routes, const std::vector<std::vector<double>>& dist) {
    for (auto& route : routes) {
        bool improved = true;
        while (improved) {
            improved = false;
            int sz = route.nodes.size();
            for (int i = 1; i < sz - 2; ++i) {
                for (int j = i + 1; j < sz - 1; ++j) {
                    int A = route.nodes[i - 1];
                    int B = route.nodes[i];
                    int C = route.nodes[j];
                    int D = route.nodes[j + 1];

                    double before = dist[A][B] + dist[C][D];
                    double after  = dist[A][C] + dist[B][D];

                    if (after < before) {
                        std::reverse(route.nodes.begin() + i, route.nodes.begin() + j + 1);
                        improved = true;
                    }
                }
            }
        }
    }
}




