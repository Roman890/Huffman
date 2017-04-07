#include <deque>
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <arpa/inet.h>
#include <iomanip>


// для переворачивания uint64
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))
using namespace std;

class HUFF;
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

class Node {
    unsigned char symvol; // символ
    uint32_t num; // его колличество
    Node *left, *right; // указатели на правого и левого сына
public:
    Node(unsigned char s, uint32_t n) {
        left = NULL;
        right = NULL;
        symvol = s;
        num = n;
    }
    Node(Node *l, Node *r) {
        num = l->num + r->num;
        left = l;
        right = r;
        symvol = 0;
    }
    uint32_t get()
    {
        return num;
    }
    friend HUFF;
};

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
