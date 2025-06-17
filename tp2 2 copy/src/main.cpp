#include <iostream>
#include "VRPLIBReader.h"
#include "CW.h"
#include "InsertionSolver.h"

// Función para calcular el costo total de las rutas
double computeTotalCost(const std::vector<Route>& routes, const std::vector<std::vector<double>>& dist) {
    double total = 0.0;
    for (const auto& r : routes) {
        for (size_t i = 0; i < r.nodes.size() - 1; ++i) {
            total += dist[r.nodes[i]][r.nodes[i + 1]];
        }
    }
    return total;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_vrp_file>" << std::endl;
        return 1;
    }

    // 🗂️ Leer instancia
    VRPLIBReader reader(argv[1]);
    std::vector<std::vector<double>> dist_matrix = reader.getDistanceMatrix();

    std::cout << "Instance Name: " << reader.getName() << "\n";
    std::cout << "Dimension: " << reader.getDimension() << "\n";
    std::cout << "Vehicles: " << reader.getNumVehicles() << "\n";
    std::cout << "Capacity: " << reader.getCapacity() << "\n";
    std::cout << "Depot ID: " << reader.getDepotId() << "\n";

    // 🚚 Clarke & Wright ---------------------
    ClarkeWrightSolver cw(reader);
    auto cw_solution = cw.solve();

    std::cout << "\n--- Clarke & Wright Solution ---\n";
    for (size_t i = 0; i < cw_solution.size(); ++i) {
        std::cout << "Route " << i + 1 << " (demand: " << cw_solution[i].total_demand << "): ";
        for (int node : cw_solution[i].nodes) std::cout << node << " ";
        std::cout << "\n";
    }
    std::cout << "Total cost (CW): " << computeTotalCost(cw_solution, dist_matrix) << "\n";


    // 🚛 Nearest Insertion + Local Search -----
    InsertionSolver insertion(reader);
    auto ni_solution = insertion.solve();

    // ✅ Fase 1: relocate
    insertion.apply_relocate(ni_solution);

    // ✅ Fase 2: swap
    insertion.apply_swap(ni_solution);

    // ✅ Fase 3: 2-opt
    insertion.apply_2opt(ni_solution);

    std::cout << "\n--- Nearest Insertion + Relocate + Swap + 2-opt Solution ---\n";
    for (size_t i = 0; i < ni_solution.size(); ++i) {
        std::cout << "Route " << i + 1 << " (demand: " << ni_solution[i].total_demand << "): ";
        for (int node : ni_solution[i].nodes) std::cout << node << " ";
        std::cout << "\n";
    }
    std::cout << "Total cost (NI + Relocate + Swap + 2-opt): " << computeTotalCost(ni_solution, dist_matrix) << "\n";

    return 0;
}