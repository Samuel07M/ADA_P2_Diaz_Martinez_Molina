#ifndef OPERACIONES_H
#define OPERACIONES_H

#include <string>

// Los numeros son cadenas de digitos '0'-'9', con el digito mas
// significativo a la izquierda. Ejemplo: "1234" representa 1234.

// Entra: A y B (cualquier longitud). Sale: A + B (puede tener un digito mas).
std::string sumar(const std::string& A, const std::string& B);

// Entra: A y B. Sale: A - B, sin ceros a la izquierda.
// Precondicion: A >= B.
std::string restar(const std::string& A, const std::string& B);

// Entra: A y k >= 0. Sale: A * 10^k (agrega k ceros a la derecha).
std::string desplazar(const std::string& A, int k);

// Entra: A y la posicion m (cantidad de digitos de la mitad baja).
// Sale: alta = A sin sus ultimos m digitos, baja = ultimos m digitos.
// Si A tiene m digitos o menos, alta queda en "0".
void partir(const std::string& A, int m, std::string& alta, std::string& baja);

// Entra: A. Sale: A sin ceros a la izquierda ("000" queda "0").
std::string normalizar(const std::string& A);

// Entra: A y n. Sale: A con ceros a la izquierda hasta tener n digitos.
// Si A ya tiene n digitos o mas, se devuelve igual.
std::string rellenar(const std::string& A, int n);

// Entra: n >= 1. Sale: la menor potencia de 2 que es >= n.
int potenciaDe2(int n);

#endif