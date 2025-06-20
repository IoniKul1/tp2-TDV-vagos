#ifndef SAVING_H
#define SAVING_H

#include "VRPLIBReader.h"
#include <vector>
#include <tuple>
#include "route.h"

struct Saving {
    int i, j;
    double saving;
    bool operator<(const Saving& other) const {
        return saving > other.saving; // Para ordenar de mayor a menor
    }
};

#endif //saving_h