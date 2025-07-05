#include "CW.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

ClarkeWrightSolver::ClarkeWrightSolver(const VRPLIBReader& reader): depot(reader.getDepotId()), capacity(reader.getCapacity()), dist(reader.getDistanceMatrix()), demands(reader.getDemands()), n(reader.getDimension()), max_vehicles(reader.getNumVehicles()), Nodes(reader.getNodes()) {
    node_to_route.resize(n + 1, -1); // O(n)
}
//Complejidad total constructor: O(n)

void ClarkeWrightSolver::initialize_routes() {
    for (int i = 1; i <= n; ++i) { // O(n)
        if (i == depot) continue; // O(1)

        if (demands[i] > capacity) { // O(1)
            continue; // O(1)
        }

        Route r; // O(1)
        r.nodes = {depot, i, depot}; // O(1)
        r.total_demand = demands[i]; // O(1)
        node_to_route[i] = routes.size(); // O(1)
        routes.push_back(r); // O(1)
    }
}
//O(n) × O(1) = O(n)
//Complejidad total: O(n)

std::vector<Saving> ClarkeWrightSolver::compute_savings() {
    std::vector<Saving> savings;
    for (int i = 1; i <= n; ++i) { // O(n)
        if (i == depot) continue; // O(1)
        for (int j = i + 1; j <= n; ++j) { // O(n)
            if (j == depot) continue; // O(1)
            double s = dist[depot][i] + dist[depot][j] - dist[i][j]; // O(1)
            savings.push_back({i, j, s}); // O(1)
        }
    }
    std::sort(savings.begin(), savings.end()); // O(n² log n)
    return savings; // O(1)
}
// O(n²) + O(n² log n) = O(n² log n)
//Complejidad total: O(n² log n)

bool ClarkeWrightSolver::merge_routes(int i, int j, const std::vector<Saving>& savings) {
    int ri = node_to_route[i]; // O(1)
    int rj = node_to_route[j]; // O(1)

    if (ri == -1 || rj == -1 || ri == rj) return false; // O(1)

    Route& route_i = routes[ri]; // O(1)
    Route& route_j = routes[rj]; // O(1)

    // Verificamos si i está al final de su ruta y j al comienzo de la suya
    if (route_i.nodes[route_i.nodes.size() - 2] != i ||
        route_j.nodes[1] != j) return false; // O(1)

    if (route_i.total_demand + route_j.total_demand > capacity) return false; // O(1)

    // fusionar rj en ri
    route_i.nodes.pop_back(); // quitar depósito final, O(1)
    route_i.nodes.insert(route_i.nodes.end(), route_j.nodes.begin() + 1, route_j.nodes.end());  // O(m), siendo m la cantidad de nodos en route_j
    route_i.total_demand += route_j.total_demand; // O(1)

    // actualizar node_to_route
    for (int k = 1; k < route_j.nodes.size() - 1; ++k) { // O(m)
        node_to_route[route_j.nodes[k]] = ri; // O(1)
    }

    // invalidar ruta j
    routes[rj].nodes.clear(); // O(1)
    routes[rj].total_demand = 0; // O(1)
    return true; // O(1)
}
//O(1) + O(m) + O(m) + O(1) = O(m)
//Complejidad total: O(m)

std::vector<Route> ClarkeWrightSolver::solve() {
    bool hasError = false;

    if (max_vehicles <= 0) { // O(1)
        std::cerr << "❌ Error: La instancia tiene VEHICLES <= 0\n"; // O(1)
        hasError = true; // O(1)
    }

    if (capacity <= 0) { // O(1)
        std::cerr << "❌ Error: La capacidad del vehículo es inválida (<= 0)\n"; // O(1)
        hasError = true; // O(1)
    }

    if (Nodes.size() != n) { // O(1)
        std::cerr << "❌ Error: La cantidad de nodos no coincide con la dimensión declarada (" << Nodes.size() << " vs " << n << ")\n"; // O(1)
        hasError = true; // O(1)
    }

 

    if (hasError) return {}; // O(1)


    initialize_routes(); // O(n)
    std::vector<Saving> savings = compute_savings(); // O(n² log n)

    for (const auto& s : savings) { // O(n²)
        merge_routes(s.i, s.j, savings); // O(m)
    }

    // Devolver solo rutas válidas
    std::vector<Route> final_routes; // O(1)
    for (const auto& r : routes) { // O(n)
        if (!r.nodes.empty()) { // O(1)
            final_routes.push_back(r); // O(1)
        }
    }


    return final_routes; // O(1)
}
//O(1) + O(n) + O(n² log n) + O(n³) + O(n) = O(nr)
//Complejidad total de solve: O(n³)
