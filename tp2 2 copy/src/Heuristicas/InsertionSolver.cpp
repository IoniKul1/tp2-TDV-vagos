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
      dist(reader.getDistanceMatrix()),
      max_vehicles(reader.getNumVehicles())
      
{
    visited.resize(n + 1, false);
    visited[depot] = true; // el depósito no es cliente
}

std::vector<Route> InsertionSolver::solve() {
    bool hasError = false;

    if (n <= 0) {
        std::cerr << "❌ Error: La dimensión de la instancia (DIMENSION) es inválida (<= 0)\n";
        hasError = true;
    }

    if (nodes.size() != n) {
        std::cerr << "❌ Error: La cantidad de nodos no coincide con la dimensión declarada (" << nodes.size() << " vs " << n << ")\n";
        hasError = true;
    }
    
    if (max_vehicles <= 0) {
        std::cerr << "❌ Error: La instancia tiene VEHICLES <= 0\n";
        hasError = true;
    }

    if (capacity <= 0) {
        std::cerr << "❌ Error: La capacidad del vehículo es inválida (<= 0)\n";
        hasError = true;
    }
    
    if (hasError) return {};

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

    if (routes.size() > max_vehicles) {
    std::cerr << "❌ Error: Se utilizaron más vehículos (" << routes.size()
                << ") que los permitidos (" << max_vehicles << ").\n";
    return {};
}
    return routes;
}
