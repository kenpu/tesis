#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <iostream>
#include <cmath>
#include <string.h>
#include <sys/time.h>
#include <iomanip>

#include "VCTrie64.hpp"
#include "stats.hpp"


int main(int argc, char** argv)
{
    float elapsedTime;
    timeval begin, end;

    if (argc < 6)
        return EXIT_SUCCESS;

    VCTrie64 cbt;

    uint64_t count = atol(argv[1]);
    uint64_t* vectors = (uint64_t*) malloc(sizeof(uint64_t) * count);
    FILE* fp = fopen(argv[2], "rb");
    assert(fp != NULL);
    uint64_t r = fread(vectors, sizeof(uint64_t), count, fp);
    assert(r == count);
    fclose(fp);

    for (uint64_t i = 0; i < count; i++)
        cbt.insert(i, vectors[i]);

    free(vectors);

    count = atol(argv[3]);
    vectors = (uint64_t*) malloc(sizeof(uint64_t) * count);
    fp = fopen(argv[4], "rb");
    assert(fp != NULL);
    r = fread(vectors, sizeof(uint64_t), count, fp);
    assert(r == count);
    fclose(fp);

    uint8_t d = atoi(argv[5]);

//    clearStats();
//    gettimeofday( &begin, NULL );
//    for (uint64_t i = 0; i < count; i++)
//        cbt.search1(vectors[i],d);
//    gettimeofday( &end, NULL );
//    elapsedTime =  end.tv_sec - begin.tv_sec + (end.tv_usec - begin.tv_usec)/1000000.0;
//    std::cout << "search1," << (unsigned int) d << "," << std::fixed << std::setprecision (6) << elapsedTime << "," << popcountOps << "," << testbitOps << std::endl;

    clearStats();
    gettimeofday( &begin, NULL );
    for (uint64_t i = 0; i < count; i++)
        cbt.search2(vectors[i],d);
    gettimeofday( &end, NULL );
    elapsedTime =  end.tv_sec - begin.tv_sec + (end.tv_usec - begin.tv_usec)/1000000.0;
    std::cout << "search2," << (unsigned int) d << "," << std::fixed << std::setprecision (6) << elapsedTime << "," << popcountOps << "," << testbitOps << std::endl;

    clearStats();
    gettimeofday( &begin, NULL );
    for (uint64_t i = 0; i < count; i++)
        cbt.search3(vectors[i],d);
    gettimeofday( &end, NULL );
    elapsedTime =  end.tv_sec - begin.tv_sec + (end.tv_usec - begin.tv_usec)/1000000.0;
    std::cout << "search3," << (unsigned int) d << "," << std::fixed << std::setprecision (6) << elapsedTime << "," << popcountOps << "," << testbitOps << std::endl;

    free(vectors);
    return EXIT_SUCCESS;
}
