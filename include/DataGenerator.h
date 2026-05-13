#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <string>

class DataGenerator {
public:

    static void generateRandomData(int count, int seed, const std::string& filename);
};

#endif
