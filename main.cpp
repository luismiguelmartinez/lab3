#include <iostream>
//#include <string.h>
#include <fstream>

using namespace std;

int processData (unsigned short *, bool *);
void encode (string *, string *, short *, short *);
void decode (string *, string *, short *, short *);
void encodeData (ifstream *, ofstream *, short *, short *);
void decodeData (ifstream *, ofstream *, short *, short *);
void binaryToText (string, ofstream *);
void textToBinary(char *, char);
void encodeMethodOne (string, ofstream *, short *, short *, short *, short *, bool *);
void encodeMethodTwo (string, ofstream *, short *, string *);

int main()
{
    unsigned short process;
    bool exit = false;

    while (!exit) {

        cout<<endl;
        cout<<"|||||||||||||||||||||||||||||||||||||||||||"<<endl;
        cout<<"|||                                     |||"<<endl;
        cout<<"|||     CODIFICADOR Y DECODIFICADOR     |||"<<endl;
        cout<<"|||  1. Si desea codificar un archivo   |||"<<endl;
        cout<<"|||  2. Si desea decodificar un archivo |||"<<endl;
        cout<<"|||  * Salir presione otra tecla        |||"<<endl;
        cout<<"|||                                     |||"<<endl;
        cout<<"|||||||||||||||||||||||||||||||||||||||||||"<<endl;
        cout<<endl;
        cout<<"Opcion: ";

        cin >> process;

        processData(&process, &exit);
    }

    return 0;
}

int processData (unsigned short *process, bool *exit) {

    if ((*process != 1) && (*process != 2)) {
        *exit = true;
        return 0;
    }

    string input, output;
    short n, method;

    cout << endl <<"Ingrese nombre archivo de entrada" << endl;
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
        fin.open(*input + ".txt");
        if (!fin.is_open())
            throw '1';

        ofstream fout;
        fout.open(*output + ".txt");
        if (!fout.is_open())
            throw '2';

        encodeData (&fin, &fout, n, method);

        fin.close();
        fout.close();

    }  catch (char c) {
        if (c == '1')
            cout << "Error archivo de lectura" << endl;
        else if (c == '2') {
            cout << "Error archivo de escritura" << endl;
        }
        else
            cout << "Error inesperado" << endl;
    }
}

void encodeData (ifstream *fin, ofstream *fout, short *n, short *method) {
    //variable para convertir los datos a binarios
    char binary[9];

    //variables para los metodos de codificacion depende del metodo se destruyen algunas de ellas
    string *buffer = new string;

    bool *flag = new bool(1);
    short *index = new short(1);
    short *lastOnes = new short(0);
    short *actualOnes = new short(0);

    if (*method != 1) {
        delete flag;
        delete index;
        delete lastOnes;
        delete actualOnes;
    }

    if (*method != 2) {
        delete buffer;
    }


    // Mientras se convierten los datos a binario al mismo tiempo se codifican y se guardan en el archivo de salida
    for (string line; getline(*fin, line); ) {

        for (unsigned int i = 0; i <= line.length(); i++) {


            if (i < line.length()) {
                textToBinary(binary, line[i]);
            } else {
                textToBinary(binary, '\n');
            }


            if (*method == 1)
                encodeMethodOne(binary, fout, n, index, lastOnes, actualOnes, flag);
            else
                encodeMethodTwo(binary, fout, n, buffer);
        }
    }

    // se verifica que en el metodo 2 no hayan queado bit restantes sin procesar
    if ((buffer->length() > 0) && (*method == 2)) {

        string aux;
        aux = buffer->substr(buffer->length() - 1);
        aux += buffer->substr(0, buffer->length() - 1);

        *fout << aux;
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

void encodeMethodTwo (string binary, ofstream *fout, short *n, string *buffer) {

    string aux;

    *buffer += binary;

    while (buffer->length() >= unsigned(*n)) {

        aux = buffer->substr(*n - 1, 1);
        aux += buffer->substr(0, *n - 1);

        *fout << aux;
        aux.clear();
        *buffer = buffer->substr(*n);
    }
}


void decode (string *input, string *output, short *n, short *method) {

    try {
        ifstream fin;
        fin.open(*input + ".txt");
        if (!fin.is_open())
            throw '1';

        ofstream fout;
        fout.open(*output + ".txt");
        if (!fout.is_open())
            throw '2';

        decodeData (&fin, &fout, n, method);

        fin.close();
        fout.close();


    }  catch (char c) {
        if (c == '1')
            cout << "Error archivo de lectura" << endl;
        else if (c == '2') {
            cout << "Error archivo de escritura" << endl;
        }
        else
            cout << "Error inesperado" << endl;
    }
}

void decodeData (ifstream *fin, ofstream *fout, short *n, short *method) {

    if (*method == 1) {

        unsigned short index = 1;
        bool flag = 1;
        unsigned short lastOnes = 0;
        unsigned short actualOnes = 0;
        char temp;
        string byte;

        fin->seekg(0);

        while (fin->good()) {
            temp = fin->get();

            if (fin->good()) {

                if (flag) {
                    if (temp == '1') {
                        byte += '0';
                    } else {
                        byte += '1';
                        lastOnes++;
                    }


                    if (++index > *n) {
                        flag = 0;
                        index = 1;
                    }
                } else {
                    if (lastOnes == (*n - lastOnes)) {

                        if (temp == '1') {
                            byte += '0';
                        } else {
                            byte += '1';
                            actualOnes++;
                        }

                    } else if ((lastOnes > *n - lastOnes) || ((*n - lastOnes) == 0)) {

                        if ((index % 3) == 0) {

                            if (temp == '1') {
                                byte += '0';
                            } else {
                                byte += '1';
                                actualOnes++;
                            }
                        } else {

                            if (temp == '1')
                                actualOnes++;

                            byte += temp;
                        }

                    } else {

                        if ((index % 2) == 0) {

                            if (temp == '1') {
                                byte += '0';
                            } else {
                                byte += '1';
                                actualOnes++;
                            }
                        } else {

                            if (temp == '1')
                                actualOnes++;

                            byte += temp;
                        }

                    }

                    if (byte.length() == 8) {
                        binaryToText(byte, fout);
                        byte.clear();
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

        string aux, buffer;

        while (fin->good()) {
            aux += fin->get();

            if (fin->good()) {

                if (aux.length() == unsigned(*n)) {

                    buffer += aux.substr(1);
                    buffer += aux.substr(0, 1);

                    aux.clear();

                    while (buffer.length() >= 8) {
                        binaryToText(buffer, fout);
                        buffer = buffer.substr(8);
                    }
                }
            }
        }

        if (aux.length() > 1) {

            buffer += aux.substr(1, aux.length() - 2);
            buffer += aux.substr(0, 1);
            binaryToText(buffer, fout);
        }
    }
}

void binaryToText (string byte, ofstream *fout) {
    short aux1 = 128;
    short aux2 = 0;

    for (short i = 0; i < 8; i++) {
        aux2 += aux1 * (byte[i] - 48);
        aux1 /= 2;
    }

    *fout << char(aux2);
}

void textToBinary (char *binary, char character) {

    for (short j = 7; j >= 0; j--) {
        *(binary + j) = (character % 2) + 48;
        character /= 2;
    }
}
