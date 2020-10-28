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


        fin.open("binary.txt");
        if (!fin.is_open())
            throw '1';

        fout.open(*output + ".txt");
        if (!fout.is_open())
            throw '2';

        encodeBinaryData (&fin, &fout, n, method);


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
    unsigned short j;
    unsigned short aux;

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

    if (*method == 1) {

        unsigned short index = 1;
        bool flag = 1;
        unsigned short lastOnes = 0;
        unsigned short actualOnes = 0;
        char aux;
        fin->seekg(0);

        while (fin->good()) {
            aux = fin->get();

            if (fin->good()) {

                if (flag) {
                    if (aux == '1') {
                        *fout << '0';
                        lastOnes++;
                    } else {
                        *fout << '1';
                    }


                    if (++index > *n) {
                        flag = 0;
                        index = 1;
                    }
                } else {
                    if (lastOnes == (*n - lastOnes)) {

                        if (aux == '1') {
                            *fout << '0';
                            actualOnes++;
                        } else {
                            *fout << '1';
                        }

                    } else if ((lastOnes > *n - lastOnes) || ((*n - lastOnes) == 0)) {

                        if (aux == '1')
                            actualOnes++;

                        if ((index % 3) == 0) {

                            if (aux == '1') {
                                *fout << '0';
                            } else {
                                *fout << '1';
                            }
                        }

                    } else {

                        if (aux == '1')
                            actualOnes++;

                        if ((index % 2) == 0) {

                            if (aux == '1') {
                                *fout << '0';
                            } else {
                                *fout << '1';
                            }
                        }

                    }

                    if (++index > *n) {
                        index = 1;
                        lastOnes = actualOnes;
                        actualOnes = 0;
                    }
                }


            }
        }
    } else if (*method == 2) {

        //        fin->seekg(0);

        //        while (fin->good()) {
        //            j = 0;
        //            aux = fin->get();

        //            if (fin->good()) {
        //                while (aux > 0) {
        //                    binary[7 - j++] = char(aux % 2) + 48;
        //                    aux /= 2;
        //                }
        //                *fout << binary;
        //            }
        //        }
    }
}
