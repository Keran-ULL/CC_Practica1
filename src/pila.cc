/**
 * @file Pila.cc
 * @brief Implementacion de la clase Pila.
 */

#include "pila.h"
#include <stdexcept>

/**
 * @brief Construye una pila con un unico simbolo inicial.
 */
Pila::Pila(char simboloInicial) {
  simbolos.push_back(simboloInicial);
}

/**
 * @brief Apila una cadena de simbolos sobre la cima actual.
 *
 * Se recorre la cadena de derecha a izquierda para que el primer
 * simbolo (simbolos[0]) sea el ultimo en apilarse y, por tanto, quede
 * como nueva cima de la pila.
 */
void Pila::push(const std::string& simbolos) {
  for (auto it = simbolos.rbegin(); it != simbolos.rend(); ++it) {
    simbolos.push_back(*it);
  }
}

/**
 * @brief Elimina el simbolo situado en la cima de la pila.
 */
void Pila::pop() {
  if (vacia()) {
    throw std::out_of_range("Pila::pop: la pila esta vacia");
  }
  simbolos.pop_back();
}

/**
 * @brief Consulta el simbolo situado en la cima de la pila.
 */
char Pila::cima() const {
  if (vacia()) {
    throw std::out_of_range("Pila::cima: la pila esta vacia");
  }
  return simbolos.back();
}

/**
 * @brief Comprueba si la pila no contiene ningun simbolo.
 */
bool Pila::vacia() const {
  return simbolos.empty();
}

/**
 * @brief Numero de simbolos actualmente en la pila.
 */
std::size_t Pila::tamano() const {
  return simbolos.size();
}

/**
 * @brief Representacion textual de la pila, de cima a base.
 *
 * Se recorre el vector interno desde el final (la cima) hacia el
 * principio (la base), que es el orden mas util para leer la traza.
 */
std::string Pila::aTexto() const {
  std::string texto;
  texto.reserve(simbolos.size());
  for (auto it = simbolos.rbegin(); it != simbolos.rend(); ++it) {
    texto.push_back(*it);
  }
  return texto;
}