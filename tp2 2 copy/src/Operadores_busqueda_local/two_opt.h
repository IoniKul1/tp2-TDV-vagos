#ifndef TWO_OPT_H
#define TWO_OPT_H

#include "../Clases/route.h"
#include <vector>
#include <cmath>

void apply_2opt(std::vector<Route>& routes, const std::vector<std::vector<double>>& dist);

#endif
