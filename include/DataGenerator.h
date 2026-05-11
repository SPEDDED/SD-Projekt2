#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <string>

    // Generowanie pliku tekstowego z parami: priorytet i wartość (int)
class DataGenerator {
public:

    static void generateRandomData(int count, int seed, const std::string& filename);
};

#endif