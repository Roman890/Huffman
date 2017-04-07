#ifndef PCODE_H
#define PCODE_H
#include <iostream>

class Bincode {
public:
    uint64_t bin= 0; // для последовательности 01
    unsigned char len = 0; // длина вектора
    Bincode()
    {
        bin = 0;
        len = 0;
    }
};

#endif
