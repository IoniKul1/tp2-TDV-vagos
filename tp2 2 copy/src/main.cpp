#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include "Testings/testings.h"

#include "Clases/VRPLIBReader.h"
#include "Heuristicas/CW.h"
#include "Heuristicas/InsertionSolver.h"
#include "Operadores_busqueda_local/relocate.h"
#include "Operadores_busqueda_local/swap.h"
#include "Operadores_busqueda_local/two_opt.h"

double computeTotalCost(const std::vector<Route>& routes, const std::vector<std::vector<double>>& dist) {
    double total = 0.0;
    for (const auto& r : routes) {
        for (size_t i = 0; i < r.nodes.size() - 1; ++i) {
            total += dist[r.nodes[i]][r.nodes[i + 1]];
        }
    }
    return total;
}

std::vector<std::string> split(const std::string& str) {
    std::stringstream ss(str);
    std::vector<std::string> result;
    std::string item;
    while (getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}

int main(int argc, char* argv[]) {
    try {
        std::cout << "[1] Iniciando programa...\n";

        if (argc < 3) {
            std::cerr << "Uso: " << argv[0] << " <archivo_vrp> <heuristica: cw|ni> [operadores: relocate,swap,2opt]\n";
            return 1;
        }

        std::string instancia = argv[1];
        std::string heuristica = argv[2];
        std::vector<std::string> operadores;

        if (argc >= 4) {
            operadores = split(argv[3]);
        }

        std::cout << "[2] Leyendo instancia...\n";
        VRPLIBReader reader(instancia);
        std::cout << "[3] Instancia leída correctamente.\n";

        auto dist = reader.getDistanceMatrix();
        auto demands = reader.getDemands();
        int capacity = reader.getCapacity();

        std::cout << "[4] 📦 Instancia: " << reader.getName() << "\n";

        std::vector<Route> solucion;

        if (heuristica == "cw") {
            std::cout << "[5] Ejecutando Clarke & Wright...\n";
            ClarkeWrightSolver cw(reader);
            solucion = cw.solve();
            std::cout << "[6] 🔧 Clarke & Wright Solution obtenida.\n";
        } else if (heuristica == "ni") {
            std::cout << "[5] Ejecutando Nearest Insertion...\n";
            InsertionSolver ni(reader);
            solucion = ni.solve();
            std::cout << "[6] 🔧 Nearest Insertion Solution obtenida.\n";
        } else {
            std::cerr << "Heurística no reconocida: " << heuristica << "\n";
            return 1;
        }

        std::cout << "[7] Aplicando operadores...\n";
        for (const auto& op : operadores) {
            if (op == "relocate") {
                std::cout << "   - Aplicando relocate\n";
                apply_relocate(solucion, reader.getDistanceMatrix(), reader.getDemands(), reader.getCapacity());   
            } else if (op == "swap") {
                std::cout << "   - Aplicando swap\n";
                apply_swap(solucion, reader.getDistanceMatrix(), reader.getDemands(), reader.getCapacity());
            } else if (op == "2opt") {
                std::cout << "   - Aplicando 2-opt\n";
                apply_2opt(solucion, reader.getDistanceMatrix());
            } else {
                std::cerr << "⚠️  Operador desconocido: " << op << "\n";
            }
        }

        std::cout << "[8] Mostrando solución...\n";
        for (size_t i = 0; i < solucion.size(); ++i) {
            std::cout << "Ruta " << i + 1 << " (demanda: " << solucion[i].total_demand << "): ";
            for (int node : solucion[i].nodes) std::cout << node << " ";
            std::cout << "\n";
        }

        std::cout << "Costo total: " << computeTotalCost(solucion, dist) << "\n";

        std::cout << "[9] Preguntando si desea testear...\n";
        std::cout << "\n¿Querés testear la validez de esta solución? (s/n): ";
        std::string respuesta;
        std::cin >> respuesta;

        std::cout << "[10] Validando...\n";
        if (respuesta == "s") {
            if (check_basic_validity(solucion, reader))
                std::cout << "✅ Solución válida.\n";
            else
                std::cout << "❌ Solución inválida.\n";
        }

        std::cout << "[11] Fin del programa.\n";
    } catch (const std::exception& e) {
        std::cerr << "\n💥 Error fatal: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
