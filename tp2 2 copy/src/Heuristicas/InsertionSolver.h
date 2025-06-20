#ifndef INSERTION_SOLVER_H
#define INSERTION_SOLVER_H

#include "../Clases/VRPLIBReader.h"
#include "CW.h"  
#include "../Clases/route.h"
#include "../Clases/saving.h"

class InsertionSolver {
public:
    InsertionSolver(const VRPLIBReader& reader);
    std::vector<Route> solve();
    
private:
    int depot;
    int capacity;
    int n;
    std::vector<Node> nodes;
    std::vector<int> demands;
    std::vector<std::vector<double>> dist;
    int max_vehicles;
    std::vector<bool> visited;
    

};

#endif // INSERTION_SOLVER_H