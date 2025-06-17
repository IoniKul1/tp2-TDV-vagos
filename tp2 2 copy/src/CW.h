#ifndef CW_H
#define CW_H

#include "VRPLIBReader.h"
#include <vector>
#include <tuple>

struct Saving {
    int i, j;
    double saving;
    bool operator<(const Saving& other) const {
        return saving > other.saving; // Para ordenar de mayor a menor
    }
};

struct Route {
    std::vector<int> nodes; // incluye depósito al principio y fin
    int total_demand;
};

class ClarkeWrightSolver {
public:
    ClarkeWrightSolver(const VRPLIBReader& reader);
    std::vector<Route> solve();

private:
    int depot;
    int capacity;
    std::vector<std::vector<double>> dist;
    std::vector<int> demands;
    int n;

    std::vector<Route> routes;
    std::vector<int> node_to_route;

    void initialize_routes();
    std::vector<Saving> compute_savings();
    bool merge_routes(int i, int j, const std::vector<Saving>& savings);
};

#endif // CW_H