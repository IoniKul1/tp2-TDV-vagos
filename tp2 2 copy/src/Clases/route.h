#ifndef ROUTE_H
#define ROUTE_H

#include "VRPLIBReader.h"
#include <vector>
#include <tuple>

struct Route {
    std::vector<int> nodes; // incluye depósito al principio y fin
    int total_demand;
};

#endif // ROUTE_H