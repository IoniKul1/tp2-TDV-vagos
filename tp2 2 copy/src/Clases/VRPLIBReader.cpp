#include "VRPLIBReader.h"
#include <fstream>
#include <stdexcept>
#include <cmath>
#include <sstream>
#include <iostream> // ✅ Para debug

// Constructor: Initializes the reader and starts the parsing process.
VRPLIBReader::VRPLIBReader(const std::string& filePath) {
    parse(filePath);
}

// Main parsing method
void VRPLIBReader::parse(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("❌ Error: No se pudo abrir el archivo " + filePath);
    }

    std::string line;
    std::string current_section;

    while (getline(file, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "NAME") {
            iss.ignore(2);
            std::getline(iss, name);
        } else if (keyword == "DIMENSION") {
            iss.ignore(2);
            iss >> dimension;
        } else if (keyword == "CAPACITY") {
            iss.ignore(2);
            iss >> capacity;
        } else if (keyword == "VEHICLES") {
            iss.ignore(2);
            iss >> numVehicles;
        } else if (keyword == "NODE_COORD_SECTION") {
            current_section = "NODE_COORD_SECTION";
            nodes.reserve(dimension);
        } else if (keyword == "DEMAND_SECTION") {
            current_section = "DEMAND_SECTION";
            demands.resize(dimension + 1, 0); // 1-indexed
        } else if (keyword == "DEPOT_SECTION") {
            current_section = "DEPOT_SECTION";
        } else if (keyword == "EOF") {
            break;
        } else if (!keyword.empty() && std::isdigit(keyword[0])) {
            std::istringstream data_line_stream(line);
            if (current_section == "NODE_COORD_SECTION") {
                int id;
                double x, y;
                data_line_stream >> id >> x >> y;
                nodes.push_back({id, x, y});
            } else if (current_section == "DEMAND_SECTION") {
                int id, demand;
                data_line_stream >> id >> demand;
                if (id >= 1 && id <= dimension) {
                    demands[id] = demand;
                }
            } else if (current_section == "DEPOT_SECTION") {
                int id;
                data_line_stream >> id;
                if (id != -1) {
                    depotId = id;
                } else {
                    current_section = "";
                }
            }
        }
    }
    file.close();

    // --- VALIDACIONES DE DEBUG ---
    if (dimension <= 0) {
        throw std::runtime_error("❌ DIMENSION inválida: debe ser mayor que 0.");
    }

    if (nodes.size() != static_cast<size_t>(dimension)) {
        std::cerr << "⚠️  Advertencia: cantidad de nodos (" << nodes.size()
                  << ") no coincide con DIMENSION (" << dimension << ")\n";
        throw std::runtime_error("❌ Error crítico: falta información en NODE_COORD_SECTION.");
    }

    if (demands.size() != static_cast<size_t>(dimension + 1)) {
        std::cerr << "⚠️  Advertencia: tamaño de demands (" << demands.size()
                  << ") no coincide con DIMENSION+1 (" << dimension + 1 << ")\n";
    }

    if (depotId <= 0 || depotId > dimension) {
        std::cerr << "⚠️  Advertencia: depotId fuera de rango: " << depotId << "\n";
        throw std::runtime_error("❌ depotId inválido.");
    }

    computeDistanceMatrix();
}

// Computes the Euclidean distance matrix.
void VRPLIBReader::computeDistanceMatrix() {
    if (nodes.empty()) return;

    distanceMatrix.resize(dimension + 1, std::vector<double>(dimension + 1, 0.0));

    for (size_t i = 0; i < nodes.size(); ++i) {
        for (size_t j = i; j < nodes.size(); ++j) {
            double dist = std::sqrt(std::pow(nodes[i].x - nodes[j].x, 2) +
                                    std::pow(nodes[i].y - nodes[j].y, 2));
            distanceMatrix[nodes[i].id][nodes[j].id] = dist;
            distanceMatrix[nodes[j].id][nodes[i].id] = dist;
        }
    }
}

// --- Getter Implementations ---

const std::string& VRPLIBReader::getName() const { return name; }
int VRPLIBReader::getDimension() const { return dimension; }
int VRPLIBReader::getCapacity() const { return capacity; }
int VRPLIBReader::getNumVehicles() const { return numVehicles; }
const std::vector<Node>& VRPLIBReader::getNodes() const { return nodes; }
const std::vector<int>& VRPLIBReader::getDemands() const { return demands; }
int VRPLIBReader::getDepotId() const { return depotId; }
const std::vector<std::vector<double>>& VRPLIBReader::getDistanceMatrix() const { return distanceMatrix; }
