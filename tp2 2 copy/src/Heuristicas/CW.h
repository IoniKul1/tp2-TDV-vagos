#ifndef CW_H
#define CW_H

#include "../Clases/VRPLIBReader.h"
#include <vector>
#include <tuple>
#include "../Clases/route.h"
#include "../Clases/saving.h"


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
    int max_vehicles;
    std::vector<Node> Nodes;

    std::vector<Route> routes;
    std::vector<int> node_to_route;

    void initialize_routes();
    std::vector<Saving> compute_savings();
    bool merge_routes(int i, int j, const std::vector<Saving>& savings);
};

#endif // CW_H