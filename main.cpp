#include <iostream>
#include <string.h>
#include <fstream>

using namespace std;

int processData (unsigned short *);
void encode (string *, string *, short *, short *);
void decode (string *, string *, short *, short *);
void turnData (ifstream *, ofstream *, short *, short *);
void encodeMethodOne (string, ofstream *, short *, short *, short *, short *, bool *);
void encodeMethodTwo (string, ofstream *, short *, short *,string);

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
//    else
//        decode(&input, &output, &n, &method);

    return 0;
}

void encode (string *input, string *output, short *n, short *method) {

    try {
        ifstream fin;
        fin.open(*input + ".txt");
        if (!fin.is_open())
            throw '1';

        ofstream fout;
        fout.open(*output + ".txt");
        if (!fout.is_open())
            throw '2';

        turnData (&fin, &fout, n, method);

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

void turnData (ifstream *fin, ofstream *fout, short *n, short *method) {
    //variables para convertir los datos a binarios
    unsigned short aux;
    string binary;

    //variables para los metodos de codificacion depende del metodo se destruyen algunas
    short index = 1;
    string temp;
    bool *flag = new bool(1);
    short *lastOnes = new short(0);
    short *actualOnes = new short(0);

    if (*method != 1) {
        delete flag;
        delete lastOnes;
        delete actualOnes;
    }


    // Mientras se convierten los datos a binario al mismo tiempo se codifican y se guardan en el archivo de salida
    for (string line; getline(*fin, line); ) {

        for (unsigned int i = 0; i <= line.length(); i++) {

            binary = "00000000";

            if (i < line.length()) {
                aux = line[i];
            } else {
                // Salto de linea
                aux = 10;
            }

            for (short j = 7; j >= 0; j--) {
                binary[j] = (aux % 2) + 48;
                aux /= 2;
            }

            if (*method == 1)
                encodeMethodOne(binary, fout, n, &index, lastOnes, actualOnes, flag);
            else
                encodeMethodTwo(binary, fout, n, &index, temp);
        }
    }
}

void encodeMethodOne (string binary, ofstream *fout, short *n, short *index, short *lastOnes, short *actualOnes, bool *flag) {

    for (short i = 0; i < 8; i++) {

        if (*flag) {
            if (binary[i] == '1') {
                *fout << '0';
                (*lastOnes)++;
            } else {
                *fout << '1';
            }


            if (++(*index) > *n) {
                *flag = 0;
                *index = 1;
            }
        } else {
            if (*lastOnes == (*n - *lastOnes)) {

                if (binary[i] == '1') {
                    *fout << '0';
                    (*actualOnes)++;
                } else {
                    *fout << '1';
                }

            } else if ((*lastOnes > (*n - *lastOnes)) || ((*n - *lastOnes) == 0)) {

                if (binary[i] == '1')
                    (*actualOnes)++;

                if ((*index % 3) == 0) {

                    if (binary[i] == '1') {
                        *fout << '0';
                    } else {
                        *fout << '1';
                    }
                } else {
                    *fout << binary[i];
                }

            } else {

                if (binary[i] == '1')
                    (*actualOnes)++;

                if ((*index % 2) == 0) {

                    if (binary[i] == '1') {
                        *fout << '0';
                    } else {
                        *fout << '1';
                    }
                } else {
                    *fout << binary[i];
                }

            }

            if (++(*index) > *n) {
                *index = 1;
                *lastOnes = *actualOnes;
                *actualOnes = 0;
            }
        }
    }
}

void encodeMethodTwo (string binary, ofstream *fout, short *n, short *index, string temp) {

    char aux[*n + 1];

    for (short i = 0; i < 8; i++) {

        temp += binary[i];

        if (++(*index) > *n) {

            aux[0] = temp[*n - 1];
            for (short j = 1; j < *n; j++) {
                aux[j] = temp[j - 1];
            }

            *fout << aux;
            *index = 1;
            temp.clear();
        }
    }
}


//void decode (string *input, string *output, short *n, short *method) {

//    try {
//        ifstream fin;
//        fin.open(*input + ".txt");
//        if (!fin.is_open())
//            throw '1';

//        ofstream fout;
//        fout.open("temp2.txt");
//        if (!fout.is_open())
//            throw '2';

//        decodeData (&fin, &fout, n, method);

//        fin.close();
//        fout.close();


//        fin.open("temp2.txt");
//        if (!fin.is_open())
//            throw '1';

//        fout.open(*output + ".txt");
//        if (!fout.is_open())
//            throw '2';

//        binaryToText(&fin, &fout);

//        fin.close();
//        fout.close();


//    }  catch (char c) {
//        if (c == '1')
//            cout << "e: Error archivo de lectura" << endl;
//        else if (c == '2') {
//            cout << "e: Error archivo de escritura" << endl;
//        }
//        else
//            cout << "e: Error inesperado" << endl;
//    }
//}

//void decodeData (ifstream *fin, ofstream *fout, short *n, short *method) {

//    if (*method == 1) {

//        unsigned short index = 1;
//        bool flag = 1;
//        unsigned short lastOnes = 0;
//        unsigned short actualOnes = 0;
//        char aux;

//        fin->seekg(0);

//        while (fin->good()) {
//            aux = fin->get();

//            if (fin->good()) {

//                if (flag) {
//                    if (aux == '1') {
//                        *fout << '0';
//                    } else {
//                        *fout << '1';
//                        lastOnes++;
//                    }


//                    if (++index > *n) {
//                        flag = 0;
//                        index = 1;
//                    }
//                } else {
//                    if (lastOnes == (*n - lastOnes)) {

//                        if (aux == '1') {
//                            *fout << '0';
//                        } else {
//                            *fout << '1';
//                            actualOnes++;
//                        }

//                    } else if ((lastOnes > *n - lastOnes) || ((*n - lastOnes) == 0)) {

//                        if ((index % 3) == 0) {

//                            if (aux == '1') {
//                                *fout << '0';
//                            } else {
//                                *fout << '1';
//                                actualOnes++;
//                            }
//                        } else {

//                            if (aux == '1')
//                                actualOnes++;

//                            *fout << aux;
//                        }

//                    } else {

//                        if ((index % 2) == 0) {

//                            if (aux == '1') {
//                                *fout << '0';
//                            } else {
//                                *fout << '1';
//                                actualOnes++;
//                            }
//                        } else {

//                            if (aux == '1')
//                                actualOnes++;

//                            *fout << aux;
//                        }

//                    }

//                    if (++index > *n) {
//                        index = 1;
//                        lastOnes = actualOnes;
//                        actualOnes = 0;
//                    }
//                }
//            }
//        }

//    } else if (*method == 2) {
//        int index = 0;
//        char aux[*n + 1], temp[*n + 1];

//        while (fin->good()) {
//            aux[index] = fin->get();

//            if (fin->good()) {

//                if (++index > (*n - 1)) {

//                    temp[*n - 1] = aux[0];
//                    for (short i = 1; i < *n; i++) {
//                        temp[i - 1] = aux[i];
//                    }

//                    *fout << temp;
//                    index = 0;
//                }
//            }
//        }
//    }
//}

//void binaryToText (ifstream *fin, ofstream *fout) {
//    short aux1 = 128;
//    short aux2 = 0;
//    char temp;

//    fin->seekg(0);

//    while (fin->good()) {
//        temp = fin->get();

//        if (fin->good()) {

//            aux2 += aux1 * (temp - 48);
//            aux1 /= 2;

//            if (aux1 < 1) {
//                aux1 = 128;
//                *fout << char(aux2);
//                aux2 = 0;
//            }
//        }
//    }
//}
