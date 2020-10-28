#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

void encode (string *, string *, short *, short *);
void toBinary (ifstream *, ofstream *);
void encodeBinaryData (ifstream *, ofstream *, short *, short *);

int main()
{
    string input, output;
    short n, method;

    cout << "Ingrese nombre archivo de entrada" << endl;
    //getline(cin, input);
    input = "original";

    cout << "Ingrese nombre archivo de salida" << endl;
    //getline(cin, output);
    output = "codificado";

    cout << "Ingrese semilla n" << endl;
    //cin >> n;

    cout << "Ingrese metodo de codificacion (1 o 2)" << endl;
    //cin >> method;

    encode(&input, &output, &n, &method);

    return 0;
}

void encode (string *input, string *output, short *n, short *method) {

    try {
        ifstream fin;
        fin.open(*input + ".txt");
        if (!fin.is_open())
            throw '1';

        ofstream fout;
        fout.open("binary.txt");
        if (!fout.is_open())
            throw '2';

        toBinary(&fin, &fout);


        fout.open("binary.txt");
        if (!fin.is_open())
            throw '1';

        fout.open(*output + ".txt");
        if (!fout.is_open())
            throw '2';




        fin.close();
        fout.close();


    }  catch (char c) {
        if (c == '1')
            cout << "e: Error archivo de lectura" << endl;
        else if (c == '2') {
            cout << "e: Error archivo de escritura" << endl;
        }
        else
            cout << "e: Error inesperado" << endl;
    }
}

void toBinary(ifstream *fin, ofstream *fout) {
    short j;
    short aux;

    char binary[9];
    memset(binary, '0', 8);

    fin->seekg(0);

    while (fin->good()) {
        j = 0;
        aux = fin->get();

        if (fin->good()) {
            while (aux > 0) {
                binary[7 - j++] = char(aux % 2) + 48;
                aux /= 2;
            }
            *fout << binary;
        }
    }
}

void encodeBinaryData (ifstream *fin, ofstream *fout, short *n, short *method) {

}
