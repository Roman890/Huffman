#include "hufman.h"
#include <deque>
#include <iostream>
#include <fstream>
#include <inttypes.h>
#include <arpa/inet.h>
#include <iomanip>
#include <vector>
#include <stdlib.h>
#include <string.h>

void HUFF::readfile(ifstream &fin)
{
    unsigned char str;
    while (1)
    {
        fin >> str;
        from_file[str]++;
        if (fin.eof()) { break; }
    }
}

Node* HUFF::findmin(deque<Node*> &deq)
{
    Node * part;
    deque <Node*>:: iterator i = deq.begin();
    for (deque <Node*>::iterator j = deq.begin(); j != deq.end(); j++)
    {
        if ((**i).get() > (**j).get()){	i = j;}
    }
    part = (*i);
    deq.erase(i);
    return part;
}


void HUFF::bildtree()
{
    deque <Node*> forest;
    for (unsigned int i = 0; i < 256; i++)
    {
        Node *p = new Node(i, from_file[i]);
        forest.push_back(p);
    }
    while (1)
    {
        Node *k = new Node(findmin(forest), findmin(forest));
        forest.push_front(k);
        if (forest.size() == 1) {break;} // если в стеке остался один элемент, то это корень дерева
    }
    root = forest.front();
}


int HUFF::BinSequence(Node *r, Bincode b)
{
    while (1) {
        if (r->num == 0)
        {
            return 0;
        }
        ++b.len;
        if (r->left != 0) // если уже хранится 0 то просто увеличиваем длину
        {
            BinSequence(r->left, b);
        }
        if (r->right != 0)
        {
            b.bin |= (0x8000000000000000 >> (b.len - 1)); // так как последовательность из 64 бит,то нужно 8 пар по 2 цифры
            r = r->right;
            continue;
        }
        if (r->left == 0 && r->right == 0)
        {
// раз не добавили символ надо вычесть
            --b.len;
            N[r->symvol] = new Bincode(b);
            break;
        }
    }
}


void HUFF::pack(ifstream & fin, ofstream & fout) {
    readfile(fin);
    bildtree();
    BinSequence(root,Bincode());
    // построить последовательности 01
    fin.clear();
    fin.seekg(0, std::ios::beg);
    fout << "HUFF";
    union{
        uint32_t bufer;
        char bufByte[4];
    }un;
    cout << "PACK" << endl;
    for(unsigned short i = 0; i < 256 ; i++){

        un.bufer = htonl(from_file[i]);
        for(char j = 0; j < 4; ++j){
            fout << un.bufByte[j];
        }
    }


    unsigned char c;
    int lenbuf=0;
    uint64_t buf=0;
  while (1)
    {
        fin >> c;
        if (fin.eof()) {break;}
        buf |= (*N[c]).bin >>lenbuf;
        lenbuf += (*N[c]).len;
        if (lenbuf > 63)
        {
            buf = htonll(buf);
            fout.write((char*)&buf,8);
            lenbuf -= 64;
            buf = (*N[c]).bin << (((*N[c]).len - lenbuf));
        }
    }
//проверка на последнюю последовательность
        buf = htonll(buf);
        int i = (lenbuf / 8) + ((lenbuf%8) ? 1: 0);
        fout.write((char*)&buf,i);
}

void HUFF::unpack(ifstream & fin, ofstream & fout) //
{
    //считать HUFF
    unsigned short i = 0;
    char S[4];
    for (i = 0; i < 4;++i) {
        fin >> S[i];
    }
    //считать встречаемость цикл на 256 записей
    union{
        uint32_t bufer;
        char bufByte[4];
    }un;

    for(i = 0; i < 256 ; i++){
        for(char j=0; j < 4; ++j){
            fin >> un.bufByte[j];
        }
        from_file[i] = ntohl(un.bufer);
    }
    //строить дерево
    bildtree();
    unsigned char c;
    Node *p = root;
    while (1)
    {
        fin >> c;
        if (fin.eof()) { break; }
        for (int i = 0; i < 8; i++)
        {
            if (c & (0x80 >> i)) {
                p = p->right;
            }
            else
            {
                p = p->left;
            }
            if (p->right == p->left)
            {
                if (from_file[p->symvol] == 0)
                {
                    break;
                }
                from_file[p->symvol]--;
                fout << p->symvol;
                p = root;
            }
        }
    }
}

int main()
{
    HUFF num1;
    int var;

    ifstream fin;
    fin.unsetf (std::ios::skipws);
    ofstream f2;

    string name1;
    string name2;

    setlocale(LC_ALL, "Russian");
    while(1){
    cout << "Меню" << endl;
    cout << "1: Упаковать" << endl;
    cout << "2: Распаковать" << endl;
    cout << "3: Выход" << endl;
    cin >> var;
    switch (var)
    {
    case 1:
    {
        cout << "Введите имя файла на вход: ";
        cin >> name1;
        fin.open(name1);
        if (!fin){cout << "Can not open" << endl;break;}
        cout << endl << "Введите имя файла на выход: ";
        cin >> name2;
        f2.open(name2);
        if (!f2){cout << "Can not open" << endl;break;}
        num1.pack(fin, f2);
        fin.close();
        f2.close();
        break;
    }
    case 2:
    {
        cout << "Введите имя файла на вход: ";
        cin >> name1;
        fin.open(name1);
        if (!fin){cout << "Can not open" << endl;break;}
        cout << endl << "Введите имя файла на выход: ";
        cin >> name2;
        f2.open(name2);
        if (!f2){cout << "Can not open" << endl;break;}
        num1.unpack(fin, f2);
        fin.close();
        f2.close();
        break;
    }
    case 3:
    {
        return 0;

    }
    default:
         cout << "ERROR" <<endl;
    }
}

}
