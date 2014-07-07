#include "stats.hpp"

unsigned long popcountOps;
unsigned long testbitOps;

void clearStats(){
    popcountOps = 0;
    testbitOps = 0;
}
