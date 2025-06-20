#ifndef TESTING_H
#define TESTING_H

#include <vector>
#include <unordered_set>
#include "../Clases/VRPLIBReader.h"
#include "../Clases/route.h" // o el header donde tengas definida la struct Route

bool check_basic_validity(const std::vector<Route>& solucion, const VRPLIBReader& reader);

#endif
