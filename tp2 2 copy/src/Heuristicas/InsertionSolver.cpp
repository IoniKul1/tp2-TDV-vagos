#include "InsertionSolver.h"
#include <limits>
#include <iostream>
#include <algorithm>

InsertionSolver::InsertionSolver(const VRPLIBReader& reader): depot(reader.getDepotId()), capacity(reader.getCapacity()), n(reader.getDimension()), nodes(reader.getNodes()), demands(reader.getDemands()), dist(reader.getDistanceMatrix()), max_vehicles(reader.getNumVehicles()){
    visited.resize(n + 1, false); // O(n)
    visited[depot] = true; // O(1)
}
//Complejidad total: O(1)+O(n) = O(n)

std::vector<Route> InsertionSolver::solve() {
    bool hasError = false; // O(1)

    if (n <= 0) { // O(1)
        std::cerr << "❌ Error: La dimensión de la instancia (DIMENSION) es inválida (<= 0)\n"; // O(1)
        hasError = true; // O(1)
    }

    if (nodes.size() != n) { // O(1)
        std::cerr << "❌ Error: La cantidad de nodos no coincide con la dimensión declarada (" << nodes.size() << " vs " << n << ")\n"; // O(1)
        hasError = true; // O(1)
    }
    
    if (max_vehicles <= 0) { // O(1)
        std::cerr << "❌ Error: La instancia tiene VEHICLES <= 0\n"; // O(1)
        hasError = true; // O(1)
    }

    if (capacity <= 0) { // O(1)
        std::cerr << "❌ Error: La capacidad del vehículo es inválida (<= 0)\n"; // O(1)
        hasError = true; // O(1)
    }
    
    if (hasError) return {}; // O(1)

    std::vector<Route> routes; // O(1)
    int remaining = n - 1; // O(1)

    while (remaining > 0) { //O(n)
        Route route; // O(1)
        route.nodes.push_back(depot); // O(1)

        // Cliente más cercano al depósito
        int nearest = -1; // O(1)
        double min_dist = std::numeric_limits<double>::max(); // O(1)
        for (int i = 1; i <= n; ++i) { // O(n)
            if (!visited[i] && demands[i] <= capacity && dist[depot][i] < min_dist) { // O(1)
                nearest = i; // O(1)
                min_dist = dist[depot][i]; // O(1)
            }
        }

        if (nearest == -1) break; // O(1)

        visited[nearest] = true; // O(1)
        route.nodes.push_back(nearest); // O(1)
        route.total_demand = demands[nearest]; // O(1)
        remaining--; // O(1)

        route.nodes.push_back(depot); // O(1)

        // Insertar clientes por cercanía, mientras no se exceda la capacidad
        bool inserted = true; // O(1)
        while (inserted) { // O(n)
            inserted = false; // O(1)
            int best_client = -1; // O(1)
            int best_pos = -1; // O(1)
            double best_increase = std::numeric_limits<double>::max(); // O(1)

            for (int i = 1; i <= n; ++i) { // O(n)
                if (visited[i] || demands[i] + route.total_demand > capacity) continue; // O(1)

                for (size_t j = 0; j < route.nodes.size() - 1; ++j) { // O(m), m ≤ n
                    int u = route.nodes[j]; // O(1)
                    int v = route.nodes[j + 1]; // O(1)
                    double increase = dist[u][i] + dist[i][v] - dist[u][v]; // O(1)
                    if (increase < best_increase) { // O(1)
                        best_increase = increase; // O(1)
                        best_client = i; // O(1)
                        best_pos = j + 1; // O(1)
                    }
                }
            }

            if (best_client != -1) { // O(1)
                route.nodes.insert(route.nodes.begin() + best_pos, best_client); // O(n)
                route.total_demand += demands[best_client]; // O(1)
                visited[best_client] = true; // O(1)
                remaining--; // O(1)
                inserted = true; // O(1)
            }
        }

        routes.push_back(route); // O(1)
    }

    if (routes.size() > max_vehicles) { // O(1)
    std::cerr << "❌ Error: Se utilizaron más vehículos (" << routes.size() // O(1)
                << ") que los permitidos (" << max_vehicles << ").\n"; // O(1)
    return {}; // O(1)
}
    return routes; // O(1)
}

//Bucle principal × [Búsqueda inicial + Bucle de inserción]
//= O(n) × [O(n) + O(n) × O(n) × O(n) × O(n)]
//= O(n) × [O(n) + O(n⁴)]
//= O(n) × O(n⁴)
//= O(n⁵)
//Complejidad total:: O(n⁵)