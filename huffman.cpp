#include "hufman.h"
#include "node.h"
#include "pcode.h"

// для переворачивания uint64
#define htonll(x) ((1==htonl(1)) ? (x) : ((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | htonl((x) >> 32))

//прочитываем файл
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

//строим дерево
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

//находим минимальный узел
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

//строим последовательности
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
//символ надо вычесть
            --b.len;
            N[r->symvol] = new Bincode(b);
            break;
        }
    }
}

// упаковка
void HUFF::pack(ifstream & fin, ofstream & fout) {
    readfile(fin);
    bildtree();
    BinSequence(root,Bincode()); // построить последовательности 01
    fin.clear();
    fin.seekg(0, std::ios::beg);
    fout << "HUFF";
    union{
        uint32_t bufer;
        char bufByte[4];
    }un;

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
        cout << "PACK" << endl;
}

//запаковка
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
    cout << "UNPACK" << endl;
}
