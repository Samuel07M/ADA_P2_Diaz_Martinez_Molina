#include "algoritmos.h"
#include "operaciones.h"
#include <vector>

using namespace std;

// valor por defecto del caso base (ver algoritmos.h)
int UMBRAL = 64;

// Multiplicacion clasica (metodo del colegio), Theta(n^2)
string multiplicacionClasica(const string& A, const string& B) {
    int la = (int)A.length();
    int lb = (int)B.length();

    if (la == 0 || lb == 0) {
        return "0";
    }

    // el producto tiene a lo sumo la + lb digitos
    vector<int> res(la + lb, 0);

    // cada digito de A se multiplica por cada digito de B
    for (int i = la - 1; i >= 0; i--) {
        int carry = 0;
        for (int j = lb - 1; j >= 0; j--) {
            int producto = (A[i] - '0') * (B[j] - '0');
            int suma = res[i + j + 1] + producto + carry;
            res[i + j + 1] = suma % 10;   // digito que se queda en la columna
            carry = suma / 10;            // acarreo hacia la columna de la izquierda
        }
        res[i] = res[i] + carry;          // acarreo final de esta fila
    }

    // pasamos los enteros a caracteres
    string resultado = "";
    for (int k = 0; k < la + lb; k++) {
        resultado += (char)('0' + res[k]);
    }

    // quitamos los ceros a la izquierda
    return normalizar(resultado);
}

// Karatsuba recursiva (privada a este archivo)
// Acepta longitudes distintas: las iguala entre si, sin redondear
// a potencia de 2
static string karatsubaRec(const string& A0, const string& B0) {
    // igualamos las longitudes de A y B
    int n = (int)A0.length();
    if ((int)B0.length() > n) {
        n = (int)B0.length();
    }
    string A = rellenar(A0, n);
    string B = rellenar(B0, n);

    // caso base: con n pequeno se multiplica directo
    if (n <= UMBRAL) {
        return multiplicacionClasica(A, B);
    }

    int m = (n + 1) / 2;   // cantidad de digitos de la mitad baja

    // partimos: x = x1*10^m + x0 y y = y1*10^m + y0
    string x1, x0, y1, y0;
    partir(A, m, x1, x0);
    partir(B, m, y1, y0);

    // llamadas recursivas
    string P1 = karatsubaRec(x1, y1); // llamada 1: x1*y1
    string P2 = karatsubaRec(x0, y0); // llamada 2: x0*y0
    string P3 = karatsubaRec(sumar(x1, x0), sumar(y1, y0)); // llamada 3: (x1+x0)*(y1+y0)

    // termino central, en el orden exigido: (P3 - P1) - P2
    string medio = restar(P3, P1);
    medio = restar(medio, P2);

    // recomposicion: P1*10^(2m) + medio*10^m + P2
    string resultado = sumar(desplazar(P1, 2 * m), desplazar(medio, m));
    resultado = sumar(resultado, P2);

    return resultado;
}

// Karatsuba publica (envoltura)
string karatsuba(const string& A, const string& B) {
    // quitamos los ceros a la izquierda
    string a = normalizar(A);
    string b = normalizar(B);

    // longitud comun: potencia de 2 >= la longitud del mayor
    int n = (int)a.length();
    if ((int)b.length() > n) {
        n = (int)b.length();
    }
    n = potenciaDe2(n);

    // rellenamos ambos a n digitos (solo aqui se redondea a potencia de 2)
    a = rellenar(a, n);
    b = rellenar(b, n);

    string resultado = karatsubaRec(a, b);

    // quitamos los ceros a la izquierda del resultado
    return normalizar(resultado);
}