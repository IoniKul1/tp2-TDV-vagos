#include "CW.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>

ClarkeWrightSolver::ClarkeWrightSolver(const VRPLIBReader& reader)
    : depot(reader.getDepotId()),
      capacity(reader.getCapacity()),
      dist(reader.getDistanceMatrix()),
      demands(reader.getDemands()),
      n(reader.getDimension()),
      max_vehicles(reader.getNumVehicles()), // agregado para verificar límite de vehículos
      Nodes(reader.getNodes())
{
    node_to_route.resize(n + 1, -1); // indexa desde 1
}

void ClarkeWrightSolver::initialize_routes() {
    for (int i = 1; i <= n; ++i) { 
        if (i == depot) continue;

        if (demands[i] > capacity) {
            continue;
        }

        Route r;
        r.nodes = {depot, i, depot};
        r.total_demand = demands[i];
        node_to_route[i] = routes.size();
        routes.push_back(r);
    }
}

std::vector<Saving> ClarkeWrightSolver::compute_savings() {
    std::vector<Saving> savings;
    for (int i = 1; i <= n; ++i) {
        if (i == depot) continue;
        for (int j = i + 1; j <= n; ++j) {
            if (j == depot) continue;
            double s = dist[depot][i] + dist[depot][j] - dist[i][j];
            savings.push_back({i, j, s});
        }
    }
    std::sort(savings.begin(), savings.end());
    return savings;
}

bool ClarkeWrightSolver::merge_routes(int i, int j, const std::vector<Saving>& savings) {
    int ri = node_to_route[i];
    int rj = node_to_route[j];

    if (ri == -1 || rj == -1 || ri == rj) return false;

    Route& route_i = routes[ri];
    Route& route_j = routes[rj];

    // Verificamos si i está al final de su ruta y j al comienzo de la suya
    if (route_i.nodes[route_i.nodes.size() - 2] != i ||
        route_j.nodes[1] != j) return false;

    if (route_i.total_demand + route_j.total_demand > capacity) return false;

    // fusionar rj en ri
    route_i.nodes.pop_back(); // quitar depósito final
    route_i.nodes.insert(route_i.nodes.end(), route_j.nodes.begin() + 1, route_j.nodes.end());
    route_i.total_demand += route_j.total_demand;

    // actualizar node_to_route
    for (int k = 1; k < route_j.nodes.size() - 1; ++k) {
        node_to_route[route_j.nodes[k]] = ri;
    }

    // invalidar ruta j
    routes[rj].nodes.clear();
    routes[rj].total_demand = 0;
    return true;
}

std::vector<Route> ClarkeWrightSolver::solve() {
    bool hasError = false;

    if (max_vehicles <= 0) {
        std::cerr << "❌ Error: La instancia tiene VEHICLES <= 0\n";
        hasError = true;
    }

    if (capacity <= 0) {
        std::cerr << "❌ Error: La capacidad del vehículo es inválida (<= 0)\n";
        hasError = true;
    }

    if (Nodes.size() != n) {
        std::cerr << "❌ Error: La cantidad de nodos no coincide con la dimensión declarada (" << Nodes.size() << " vs " << n << ")\n";
        hasError = true;
    }

 

    if (hasError) return {};


    initialize_routes();
    std::vector<Saving> savings = compute_savings();

    for (const auto& s : savings) {
        merge_routes(s.i, s.j, savings);
    }

    // Devolver solo rutas válidas
    std::vector<Route> final_routes;
    for (const auto& r : routes) {
        if (!r.nodes.empty()) {
            final_routes.push_back(r);
        }
    }


    return final_routes;
}
