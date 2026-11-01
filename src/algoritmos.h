#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include <string>

// Tamano (en digitos) a partir del cual karatsuba deja de dividir y
// usa la multiplicacion clasica. Minimo 3 (con 2 o 3 la recursion no
// achicaria el problema). Para el experimento probar 3, 32 y 64.
// Se usa tambien en main.cpp para nombrar el archivo de resultados.
const int UMBRAL = 64;

// Los numeros son cadenas de digitos '0'-'9', con el digito mas
// significativo a la izquierda (igual que en operaciones.h).

// Entra: A y B (cualquier longitud, pueden traer ceros a la izquierda).
// Sale: A * B sin ceros a la izquierda. Metodo del colegio, Theta(n^2).
std::string multiplicacionClasica(const std::string& A, const std::string& B);

// Entra: A y B (cualquier longitud, pueden traer ceros a la izquierda).
// Sale: A * B sin ceros a la izquierda. Metodo de Karatsuba, Theta(n^1.585).
std::string karatsuba(const std::string& A, const std::string& B);

#endif