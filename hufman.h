#ifndef HUFMAN_H
#define HUFMAN_H
#include <deque>
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <arpa/inet.h>
#include <iomanip>
#include "pcode.h"
#include "node.h"

using namespace std;


class HUFF {
private:
    Node* root;
    Bincode* N[256]; // массив из последовательностей 01 связанных с символом
    uint32_t from_file[256]; // массив встречаемости символов из файла

public:
    HUFF()
    {
        for (int i = 0; i < 256; i++)
        {
            from_file[i] = 0;
        }
    }
    void readfile(ifstream &fin);     //прочтение файла
    void bildtree();                  // построение дерева
    void pack(ifstream & fin, ofstream & fout);//архиватор
    Node* findmin(deque<Node*> &vec);          //поиск мимнимального
    int BinSequence(Node *r, Bincode b);
    void unpack(ifstream & fin, ofstream & fout);
};

#endif
