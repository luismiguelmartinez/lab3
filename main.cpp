#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

int processData (unsigned short *);
void encode (string *, string *, short *, short *);
void decode (string *, string *, short *, short *);
void textToBinary (ifstream *, ofstream *);
void binaryToText (ifstream *, ofstream *);
void encodeBinaryData (ifstream *, ofstream *, short *, short *);
void decodeBinaryData (ifstream *, ofstream *, short *, short *);

int main()
{
    unsigned short process;

    while (1) {
        cout << "Que desea hacer " << endl;
        cin >> process;
        processData(&process);
    }

    return 0;
}

int processData (unsigned short *process) {

     if ((*process != 1) && (*process != 2)) {
         cout << "Proceso no valido" << endl;
         return 0;
     }

    string input, output;
    short n, method;

    cout << "Ingrese nombre archivo de entrada" << endl;
    getline(cin, input);
    getline(cin, input);

    cout << "Ingrese nombre archivo de salida" << endl;
    getline(cin, output);

    cout << "Ingrese semilla n" << endl;
    cin >> n;

    cout << "Ingrese metodo de codificacion (1 o 2)" << endl;
    cin >> method;

    if (*process == 1)
        encode(&input, &output, &n, &method);
    else
        decode(&input, &output, &n, &method);

    return 0;
}

void encode (string *input, string *output, short *n, short *method) {

    try {
        ifstream fin;
        fin.open(*input + ".txt", ios::in);
        if (!fin.is_open())
            throw '1';

        ofstream fout;
        fout.open("temp.txt");
        if (!fout.is_open())
            throw '2';

        textToBinary(&fin, &fout);

        fin.close();
        fout.close();


        fin.open("temp.txt");
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

void textToBinary(ifstream *fin, ofstream *fout) {
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
                        } else {
                            *fout << aux;
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
                        } else {
                            *fout << aux;
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
        int index = 0;
        char aux[*n + 1], temp[*n + 1];

        while (fin->good()) {
            aux[index] = fin->get();

            if (fin->good()) {

                if (++index > (*n - 1)) {

                    temp[0] = aux[*n - 1];
                    for (short i = 1; i < *n; i++) {
                        temp[i] = aux[i - 1];
                    }

                    *fout << temp;
                    index = 0;
                }
            }
        }
    }
}

void decode (string *input, string *output, short *n, short *method) {

    try {
        ifstream fin;
        fin.open(*input + ".txt");
        if (!fin.is_open())
            throw '1';

        ofstream fout;
        fout.open("temp2.txt");
        if (!fout.is_open())
            throw '2';

        decodeBinaryData (&fin, &fout, n, method);

        fin.close();
        fout.close();


        fin.open("temp2.txt");
        if (!fin.is_open())
            throw '1';

        fout.open(*output + ".txt");
        if (!fout.is_open())
            throw '2';

        binaryToText(&fin, &fout);

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

void decodeBinaryData (ifstream *fin, ofstream *fout, short *n, short *method) {

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
                    } else {
                        *fout << '1';
                        lastOnes++;
                    }


                    if (++index > *n) {
                        flag = 0;
                        index = 1;
                    }
                } else {
                    if (lastOnes == (*n - lastOnes)) {

                        if (aux == '1') {
                            *fout << '0';
                        } else {
                            *fout << '1';
                            actualOnes++;
                        }

                    } else if ((lastOnes > *n - lastOnes) || ((*n - lastOnes) == 0)) {

                        if ((index % 3) == 0) {

                            if (aux == '1') {
                                *fout << '0';
                            } else {
                                *fout << '1';
                                actualOnes++;
                            }
                        } else {

                            if (aux == '1')
                                actualOnes++;

                            *fout << aux;
                        }

                    } else {

                        if ((index % 2) == 0) {

                            if (aux == '1') {
                                *fout << '0';
                            } else {
                                *fout << '1';
                                actualOnes++;
                            }
                        } else {

                            if (aux == '1')
                                actualOnes++;

                            *fout << aux;
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
        int index = 0;
        char aux[*n + 1], temp[*n + 1];

        while (fin->good()) {
            aux[index] = fin->get();

            if (fin->good()) {

                if (++index > (*n - 1)) {

                    temp[*n - 1] = aux[0];
                    for (short i = 1; i < *n; i++) {
                        temp[i - 1] = aux[i];
                    }

                    *fout << temp;
                    index = 0;
                }
            }
        }
    }
}

void binaryToText (ifstream *fin, ofstream *fout) {
    short aux1 = 128;
    short aux2 = 0;
    char temp;

    fin->seekg(0);

    while (fin->good()) {
        temp = fin->get();

        if (fin->good()) {

            aux2 += aux1 * (temp - 48);
            aux1 /= 2;

            if (aux1 < 1) {
                aux1 = 128;
                *fout << char(aux2);
                aux2 = 0;
            }
        }
    }
}
