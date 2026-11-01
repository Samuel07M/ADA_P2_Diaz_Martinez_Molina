#include "operaciones.h"

using namespace std;

// Invierte una cadena (funcion interna, no esta en el .h).
// Las sumas y restas construyen el resultado de derecha a izquierda,
// asi que al final hay que darlo vuelta.
static string invertir(const string& s) {
    string r = s;
    int i = 0;
    int j = (int)r.length() - 1;
    while (i < j) {
        char temp = r[i];
        r[i] = r[j];
        r[j] = temp;
        i++;
        j--;
    }
    return r;
}

string sumar(const string& A, const string& B) {
    int i = (int)A.length() - 1;   // indice sobre A (de derecha a izquierda)
    int j = (int)B.length() - 1;   // indice sobre B
    int carry = 0;                 // acarreo
    string resultado = "";

    // seguimos mientras queden digitos en A o en B, o un acarreo pendiente
    while (i >= 0 || j >= 0 || carry > 0) {
        int suma = carry;

        if (i >= 0) {
            suma = suma + (A[i] - '0');
            i--;
        }
        if (j >= 0) {
            suma = suma + (B[j] - '0');
            j--;
        }

        resultado += (char)('0' + suma % 10);   // digito que se escribe
        carry = suma / 10;                      // manejamos el acarreo (carry)
    }

    if (resultado.length() == 0) {   // caso A y B vacias
        return "0";
    }
    return invertir(resultado);
}

string restar(const string& A, const string& B) {
    int i = (int)A.length() - 1;
    int j = (int)B.length() - 1;
    int borrow = 0;                // prestamo
    string resultado = "";

    // B puede traer ceros a la izquierda y ser mas larga que A en texto,
    // por eso el ciclo se mantiene mientras quede algo en A o en B
    while (i >= 0 || j >= 0) {
        int digA = 0;
        int digB = 0;

        if (i >= 0) {
            digA = A[i] - '0';
            i--;
        }
        if (j >= 0) {
            digB = B[j] - '0';
            j--;
        }

        int dif = digA - digB - borrow;

        if (dif < 0) {
            dif = dif + 10;   // pedimos prestado a la columna de la izquierda
            borrow = 1;
        } else {
            borrow = 0;
        }

        resultado += (char)('0' + dif);
    }

    // quitamos los ceros a la izquierda
    return normalizar(invertir(resultado));
}

string desplazar(const string& A, int k) {
    string resultado = A;
    // agregamos k ceros a la derecha (multiplicar por 10^k)
    for (int i = 0; i < k; i++) {
        resultado += '0';
    }
    return resultado;
}

void partir(const string& A, int m, string& alta, string& baja) {
    int n = (int)A.length();

    if (m <= 0) {
        // la mitad baja no tiene digitos
        alta = A;
        baja = "0";
    } else if (m >= n) {
        // A es mas corta que la mitad baja: no hay parte alta
        alta = "0";
        baja = A;
    } else {
        alta = A.substr(0, n - m);   // todo menos los ultimos m digitos
        baja = A.substr(n - m);      // los ultimos m digitos
    }
}

string normalizar(const string& A) {
    int i = 0;
    int n = (int)A.length();

    // buscamos el primer digito distinto de '0'
    while (i < n && A[i] == '0') {
        i++;
    }

    if (i == n) {      // todo eran ceros (o la cadena estaba vacia)
        return "0";
    }
    return A.substr(i);
}

string rellenar(const string& A, int n) {
    int largo = (int)A.length();
    if (largo >= n) {
        return A;
    }

    string resultado = "";
    // agregamos los ceros que faltan a la izquierda
    for (int i = 0; i < n - largo; i++) {
        resultado += '0';
    }
    resultado += A;
    return resultado;
}

int potenciaDe2(int n) {
    int p = 1;
    while (p < n) {
        p = p * 2;
    }
    return p;
}