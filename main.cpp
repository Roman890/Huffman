#include "hufman.h"
#include "node.h"

#include <iostream>
#include <fstream>
#include <string.h>

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
