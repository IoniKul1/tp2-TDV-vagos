#ifndef INSERTION_SOLVER_H
#define INSERTION_SOLVER_H

#include "VRPLIBReader.h"
#include "CW.h"  // usamos la misma struct Route

class InsertionSolver {
public:
    InsertionSolver(const VRPLIBReader& reader);
    std::vector<Route> solve();
    void apply_relocate(std::vector<Route>& routes);  // 🔁 mejora inter-ruta
    void apply_swap(std::vector<Route>& routes);     // 🔀 inter-ruta
    void apply_2opt(std::vector<Route>& routes);  // 🔧 mejora intra-ruta

private:
    int depot;
    int capacity;
    int n;
    std::vector<Node> nodes;
    std::vector<int> demands;
    std::vector<std::vector<double>> dist;
    std::vector<bool> visited;

};

#endif // INSERTION_SOLVER_H