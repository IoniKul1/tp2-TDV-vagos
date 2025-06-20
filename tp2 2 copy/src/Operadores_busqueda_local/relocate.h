#ifndef RELOCATE_H
#define RELOCATE_H

#include "../Clases/route.h"
#include <vector>

void apply_relocate(std::vector<Route>& routes,const std::vector<std::vector<double>>& dist,const std::vector<int>& demands,int capacity);

#endif
