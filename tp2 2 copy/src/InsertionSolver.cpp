#include "InsertionSolver.h"
#include <limits>
#include <iostream>
#include <algorithm>

InsertionSolver::InsertionSolver(const VRPLIBReader& reader)
    : depot(reader.getDepotId()),
      capacity(reader.getCapacity()),
      n(reader.getDimension()),
      nodes(reader.getNodes()),
      demands(reader.getDemands()),
      dist(reader.getDistanceMatrix())
{
    visited.resize(n + 1, false);
    visited[depot] = true; // el depósito no es cliente
}

std::vector<Route> InsertionSolver::solve() {
    std::vector<Route> routes;
    int remaining = n - 1;

    while (remaining > 0) {
        Route route;
        route.nodes.push_back(depot);

        // Cliente más cercano al depósito
        int nearest = -1;
        double min_dist = std::numeric_limits<double>::max();
        for (int i = 1; i <= n; ++i) {
            if (!visited[i] && demands[i] <= capacity && dist[depot][i] < min_dist) {
                nearest = i;
                min_dist = dist[depot][i];
            }
        }

        if (nearest == -1) break;

        visited[nearest] = true;
        route.nodes.push_back(nearest);
        route.total_demand = demands[nearest];
        remaining--;

        route.nodes.push_back(depot);

        // Insertar clientes por cercanía, mientras no se exceda la capacidad
        bool inserted = true;
        while (inserted) {
            inserted = false;
            int best_client = -1;
            int best_pos = -1;
            double best_increase = std::numeric_limits<double>::max();

            for (int i = 1; i <= n; ++i) {
                if (visited[i] || demands[i] + route.total_demand > capacity) continue;

                for (size_t j = 0; j < route.nodes.size() - 1; ++j) {
                    int u = route.nodes[j];
                    int v = route.nodes[j + 1];
                    double increase = dist[u][i] + dist[i][v] - dist[u][v];
                    if (increase < best_increase) {
                        best_increase = increase;
                        best_client = i;
                        best_pos = j + 1;
                    }
                }
            }

            if (best_client != -1) {
                route.nodes.insert(route.nodes.begin() + best_pos, best_client);
                route.total_demand += demands[best_client];
                visited[best_client] = true;
                remaining--;
                inserted = true;
            }
        }

        routes.push_back(route);
    }

   // apply_2opt(routes); // 🔧 Aplicar mejora local

    return routes;
}
// 2-opt intra-ruta: invierte segmentos para reducir distancia
void InsertionSolver::apply_2opt(std::vector<Route>& routes) {
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


void InsertionSolver::apply_relocate(std::vector<Route>& routes) {
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

            std::cout << "Relocate: Cliente " << client
                      << " de ruta " << best_from
                      << " a ruta " << best_to
                      << " con mejora " << best_delta << std::endl;
        }
    }

    // Eliminar rutas vacías
    routes.erase(
        std::remove_if(routes.begin(), routes.end(),
            [](const Route& r) { return r.nodes.size() <= 2; }),
        routes.end()
    );
}

// InsertionSolver.cpp (agregar al final)

void InsertionSolver::apply_swap(std::vector<Route>& routes) {
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
