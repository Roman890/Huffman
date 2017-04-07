
#ifndef NODE_H
#define NODE_H

#include "hufman.h"
#include <iostream>
using namespace std;

class HUFF;
class Node
{
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

#endif
