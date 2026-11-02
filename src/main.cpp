#include <iostream>
#include <string>
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "algoritmos.h"

using namespace std;

// Entra: n >= 1. Sale: cadena aleatoria de n digitos, primer digito != '0'
string generarAleatorio(int n) {
    string s = "";
    s += (char)('1' + rand() % 9); // primer digito: 1 a 9
    for (int i = 1; i < n; i++) {
        s += (char)('0' + rand() % 10); // demas digitos: 0 a 9
    }
    return s;
}

// Opcion 1: validacion

// Compara karatsuba contra la clasica e imprime OK o FALLO
// Devuelve true si coinciden
bool comparar(const string& nombre, const string& A, const string& B) {
    string r1 = multiplicacionClasica(A, B);
    string r2 = karatsuba(A, B);

    if (r1 == r2) {
        cout << "OK     " << nombre << endl;
        return true;
    }
    cout << "FALLO  " << nombre << endl;
    if (A.length() <= 30 && B.length() <= 30) {
        cout << "  A = " << A << ", B = " << B << endl;
        cout << "  clasica   = " << r1 << endl;
        cout << "  karatsuba = " << r2 << endl;
    }
    return false;
}

// Comprueba que la clasica da el resultado esperado (valor conocido)
// La clasica es la referencia, por eso se verifica por separado
bool verificarClasica(const string& nombre, const string& A, const string& B,
                      const string& esperado) {
    if (multiplicacionClasica(A, B) == esperado) {
        cout << "OK     " << nombre << endl;
        return true;
    }
    cout << "FALLO  " << nombre << endl;
    return false;
}

// Casos de karatsuba vs clasica. Devuelve la cantidad de fallos
int casosKaratsuba() {
    int fallos = 0;
    srand(42); // mismos numeros aleatorios con cualquier UMBRAL

    // numeros pequenos
    if (!comparar("1 digito (7 x 8)", "7", "8")) fallos++;
    if (!comparar("1 digito (9 x 9)", "9", "9")) fallos++;
    if (!comparar("2 digitos (12 x 34)", "12", "34")) fallos++;
    if (!comparar("acarreos (99 x 99)", "99", "99")) fallos++;
    if (!comparar("cero (0 x 12345)", "0", "12345")) fallos++;

    // ceros a la izquierda
    if (!comparar("ceros a la izquierda", "000123", "0045")) fallos++;
    if (!comparar("solo ceros", "0000", "000")) fallos++;
    string ceros(200, '0'); // 200 ceros
    if (!comparar("200 ceros a la izquierda", ceros + "12345", ceros + "678")) fallos++;

    // ceros intercalados
    if (!comparar("ceros intercalados", "10203004", "50006007")) fallos++;
    if (!comparar("potencias de 10", "1000000", "1000")) fallos++;

    // longitudes impares y distintas entre si
    if (!comparar("longitud impar (5 x 5)", "98765", "12345")) fallos++;
    if (!comparar("longitudes distintas (3 x 9)", "123", "987654321")) fallos++;

    // numeros grandes aleatorios
    string A = generarAleatorio(300);
    string B = generarAleatorio(300);
    if (!comparar("aleatorios de 300 digitos", A, B)) fallos++;

    A = generarAleatorio(517);
    B = generarAleatorio(333);
    if (!comparar("aleatorios de 517 x 333 digitos", A, B)) fallos++;

    return fallos;
}

void validar() {
    int fallos = 0;
    int umbralOriginal = UMBRAL;

    cout << "\n--- Validacion de la clasica (valores conocidos) ---" << endl;
    if (!verificarClasica("clasica 99 x 99 = 9801", "99", "99", "9801")) fallos++;
    if (!verificarClasica("clasica 123456789 x 987654321",
                          "123456789", "987654321", "121932631112635269")) fallos++;

    // Con UMBRAL = 3 hasta los casos pequenos (4 u 8 digitos tras el
    // relleno) se parten en mitades y pasan por P1, P2, P3, la resta
    // (P3 - P1) - P2 y la recomposicion. Con UMBRAL = 64 se valida la
    // configuracion usada en la medicion
    const int umbrales[2] = {3, 64};
    for (int u = 0; u < 2; u++) {
        UMBRAL = umbrales[u];
        cout << "\n--- Validacion: karatsuba vs clasica (UMBRAL = " << UMBRAL
             << ") ---" << endl;
        fallos += casosKaratsuba();
    }
    UMBRAL = umbralOriginal;

    cout << endl;
    if (fallos == 0) {
        cout << "Todos los casos pasaron." << endl;
    } else {
        cout << "Casos con FALLO: " << fallos << endl;
    }
}

// Opcion 2: medicion de tiempos

// Mide el tiempo promedio (en microsegundos) de una multiplicacion. Repite
// la operacion k veces; k se duplica hasta que el tiempo total supere
// 10 ms, para que sea mucho mayor que la resolucion del reloj,
// 'fantasma' acumula el tamano de los resultados para que el compilador
// no elimine las llamadas
double medirUs(string (*multiplicar)(const string&, const string&),
               const string& A, const string& B, long long& fantasma) {
    // llamada de calentamiento (no se cronometra)
    fantasma += (long long)multiplicar(A, B).length();

    int k = 1;
    while (true) {
        chrono::steady_clock::time_point inicio = chrono::steady_clock::now();

        for (int r = 0; r < k; r++) {
            string resultado = multiplicar(A, B);
            fantasma += (long long)resultado.length();
        }

        chrono::steady_clock::time_point fin = chrono::steady_clock::now();
        double totalUs = chrono::duration<double, micro>(fin - inicio).count();

        if (totalUs >= 10000.0 || k >= 1000000) { // >= 10 ms
            return totalUs / k; // promedio por multiplicacion
        }
        k = k * 2;
    }
}

// Mide una tabla completa (n = 2 ... 4096) con el UMBRAL actual y la
// guarda en resultados/tabla_umbralU_corridaN.txt
void medirCorrida(int corrida) {
    long long fantasma = 0;

    // la tabla se arma aqui y al final se muestra y se guarda en archivo
    ostringstream tabla;
    tabla << "n\tclasica_us\tkaratsuba_us" << endl;

    cout << "\n--- UMBRAL = " << UMBRAL << ", corrida " << corrida
         << " (microsegundos) ---" << endl;
    cout << "n\tclasica_us\tkaratsuba_us" << endl;

    for (int n = 2; n <= 4096; n = n * 2) {
        // las entradas se generan fuera del cronometro
        string A = generarAleatorio(n);
        string B = generarAleatorio(n);

        // comprobacion de que ambos dan lo mismo (fuera del cronometro)
        if (multiplicacionClasica(A, B) != karatsuba(A, B)) {
            cout << "ERROR: resultados distintos para n = " << n << endl;
        }

        double tClasica = medirUs(multiplicacionClasica, A, B, fantasma);
        double tKaratsuba = medirUs(karatsuba, A, B, fantasma);

        cout << n << "\t" << tClasica << "\t" << tKaratsuba << endl;
        tabla << n << "\t" << tClasica << "\t" << tKaratsuba << endl;
    }

    // se imprime para que el compilador no pueda descartar los calculos
    cout << "(checksum: " << fantasma << ")" << endl;

    // guardado de la tabla (solo la tabla, sin el menu)
    string ruta = "resultados/tabla_umbral" + to_string(UMBRAL) +
                  "_corrida" + to_string(corrida) + ".txt";
    ofstream archivo(ruta);
    if (archivo) {
        archivo << tabla.str();
        archivo.close();
        cout << "Tabla guardada en: " << ruta << endl;
    } else {
        cout << "ERROR: no se pudo crear " << ruta << endl;
    }
}

// Opcion 2 completa: 3 umbrales x 3 corridas = 9 archivos en una sola
// ejecucion. La corrida k usa la misma semilla en los tres umbrales,
// asi los tres umbrales se comparan con los mismos operandos
void medir() {
    const int umbrales[3] = {3, 32, 64};
    const int CORRIDAS = 3;
    int umbralOriginal = UMBRAL;

    filesystem::create_directories("resultados");

    for (int u = 0; u < 3; u++) {
        UMBRAL = umbrales[u];
        for (int c = 1; c <= CORRIDAS; c++) {
            srand(42 + c); // misma entrada para la corrida c de cada umbral
            medirCorrida(c);
        }
    }

    UMBRAL = umbralOriginal;
    cout << "\nListo: 9 tablas guardadas en la carpeta resultados/" << endl;
}

// Menu principal
int main() {
    srand(42); // semilla fija, los experimentos son reproducibles

    int opcion = -1;
    while (opcion != 0) {
        cout << "\n=== MENU PRINCIPAL ===" << endl;
        cout << "0. Salir" << endl;
        cout << "1. Validar correctitud" << endl;
        cout << "2. Medir tiempos" << endl;
        cout << "> ";

        if (!(cin >> opcion)) { // entrada no numerica o fin de entrada
            break;
        }

        if (opcion == 1) {
            validar();
        } else if (opcion == 2) {
            medir();
        } else if (opcion != 0) {
            cout << "Opcion no valida." << endl;
        }
    }
    return 0;
}