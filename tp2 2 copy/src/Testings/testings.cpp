#include "testings.h"
#include <iostream>

bool check_basic_validity(const std::vector<Route>& solucion, const VRPLIBReader& reader) {
    std::unordered_set<int> visitados;
    int depot = reader.getDepotId();
    int expected_clients = reader.getDimension() - 1;
    int total_visitados = 0;

    for (const auto& r : solucion) {
        if (r.nodes.front() != depot || r.nodes.back() != depot) {
            std::cerr << "❌ Ruta no empieza/termina en el depósito.\n";
            return false;
        }

        if (r.total_demand > reader.getCapacity()) {
            std::cerr << "❌ Ruta excede la capacidad.\n";
            return false;
        }

        for (size_t i = 1; i < r.nodes.size() - 1; ++i) {
            int cliente = r.nodes[i];
            if (visitados.count(cliente)) {
                std::cerr << "❌ Cliente " << cliente << " visitado más de una vez.\n";
                return false;
            }
            visitados.insert(cliente);
            total_visitados++;
        }
    }

    if (total_visitados != expected_clients) {
        std::cerr << "❌ No se visitaron todos los clientes. Visitados: " << total_visitados
                  << ", Esperados: " << expected_clients << "\n";
        return false;
    }

    return true;
}

bool check_cost_consistency(const std::vector<Route>& solucion, const VRPLIBReader& reader) {
    double calculated_cost = 0.0;
    const auto& dist = reader.getDistanceMatrix();

    for (const auto& r : solucion) {
        for (size_t i = 0; i < r.nodes.size() - 1; ++i) {
            int u = r.nodes[i];
            int v = r.nodes[i + 1];
            calculated_cost += dist[u][v];
        }
    }

    std::cout << "✅ Costo total calculado de la solución: " << calculated_cost << "\n";
    return true;
}


bool check_total_demand(const std::vector<Route>& solucion, const VRPLIBReader& reader) {
    const auto& demands = reader.getDemands();
    for (const auto& r : solucion) {
        int total = 0;
        for (int i = 1; i < (int)r.nodes.size() - 1; ++i) {
            total += demands[r.nodes[i]];
        }
        if (total != r.total_demand) {
            std::cerr << "❌ Demanda total mal calculada para una ruta. Esperado: " << total << ", Reportado: " << r.total_demand << "\n";
            return false;
        }
    }
    return true;
}

bool check_depot_positions(const std::vector<Route>& solucion, int depot) {
    for (const auto& r : solucion) {
        if (r.nodes.front() != depot || r.nodes.back() != depot) {
            std::cerr << "❌ Una ruta no comienza y termina en el depósito.\n";
            return false;
        }
    }
    return true;
} 

