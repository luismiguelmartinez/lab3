#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

void write ();
void toBinary (string, char *, unsigned short *);

int main()
{
    write();

    return 0;
}

void write () {

    try {
        ofstream fout;
        fout.open("temp2.txt");

        if (!fout.is_open())
            throw '1';

        cout << "Ingrese una palabra" << endl;
        string buffer;
        cin >> buffer;
        char binary[9];
        memset(binary, '0', 8);

        for (unsigned short i = 0; i < buffer.length(); i++) {
            toBinary(buffer, binary, &i);
            fout << binary;
        }
        fout.close();


    }  catch (char c) {
        if (c == '1')
            cout << "e: Error archivo de escritura" << endl;
        else
            cout << "e: Error inesperado" << endl;
    }
}

void toBinary(string word, char *auxBinary, unsigned short *i) {
    short aux = word[*i];

    short j = 0;
    while (aux > 0) {
        *(auxBinary + 7 - j) = char(aux % 2) + 48;
        j++;
        aux /= 2;
    }
}
