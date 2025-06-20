#ifndef SWAP_H
#define SWAP_H

#include "../Clases/route.h"
#include <vector>

void apply_swap(std::vector<Route>& routes,const std::vector<std::vector<double>>& dist,const std::vector<int>& demands,int capacity);

#endif
