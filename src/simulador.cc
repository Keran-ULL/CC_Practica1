/**
 * @file Simulador.cc
 * @brief Implementacion de la clase Simulador.
 */

#include "simulador.h"

#include <algorithm>

/**
 * @brief Construye un simulador para un automata concreto.
 */
Simulador::Simulador(const Automata& automata, Traza* traza)
    : automata(automata), traza(traza) {}

/**
 * @brief Comprueba si una cadena pertenece al lenguaje reconocido.
 *
 * Se arranca la busqueda desde el estado inicial, con la pila
 * conteniendo unicamente el simbolo inicial, y la cadena completa por
 * consumir. Si hay traza, al terminar se muestra el resultado final.
 */
bool Simulador::acepta(const std::string& cadena) const {
  if (traza != nullptr) {
    traza->comenzarCadena(cadena);
  }

  Pila pila(automata.getSimboloInicialPila());
  std::vector<std::string> visitados;
  std::vector<int> camino;

  const bool aceptada = buscar(automata.getEstadoInicial(), cadena, pila, visitados, camino);

  if (traza != nullptr) {
    traza->mostrarResultado(aceptada, camino);
  }

  return aceptada;
}

/**
 * @brief Obtiene todas las transiciones aplicables desde una
 *        configuracion.
 *
 * Primero las que consumen el siguiente simbolo de entrada (si queda
 * alguno) y despues las transiciones-epsilon.
 */
std::vector<Transicion> Simulador::candidatas(const std::string& estado,
                                               const std::string& entrada,
                                               char tope) const {
  std::vector<Transicion> resultado;
  if (!entrada.empty()) {
    resultado = automata.getTransiciones().aplicables(estado, entrada.front(), tope);
  }

  const std::vector<Transicion> epsilon = automata.getTransiciones().aplicablesEpsilon(estado, tope);
  resultado.insert(resultado.end(), epsilon.begin(), epsilon.end());
  return resultado;
}

/**
 * @brief Explora recursivamente las transiciones aplicables desde una
 *        configuracion concreta.
 */
bool Simulador::buscar(const std::string& estado,
                        const std::string& entrada,
                        Pila pila,
                        std::vector<std::string>& visitados,
                        std::vector<int>& camino) const {
  const std::string firma = estado + "|" + entrada + "|" + pila.aTexto();
  if (std::find(visitados.begin(), visitados.end(), firma) != visitados.end()) {
    return false;
  }

  if (entrada.empty() && automata.esFinal(estado)) {
    return true;
  }

  std::vector<Transicion> candidatasActuales;
  if (!pila.vacia()) {
    candidatasActuales = candidatas(estado, entrada, pila.cima());
  }

  if (candidatasActuales.empty()) {
    if (traza != nullptr) {
      traza->registrar(estado, entrada, pila.aTexto(), {}, 0);
    }
    return false;
  }

  std::vector<int> idsPosibles;
  idsPosibles.reserve(candidatasActuales.size());
  for (const Transicion& transicion : candidatasActuales) {
    idsPosibles.push_back(transicion.id);
  }

  visitados.push_back(firma);
  bool aceptado = false;

  for (const Transicion& transicion : candidatasActuales) {
    if (traza != nullptr) {
      traza->registrar(estado, entrada, pila.aTexto(), idsPosibles, transicion.id);
    }

    Pila siguientePila = pila;
    siguientePila.pop();
    siguientePila.push(transicion.pila);

    const std::string siguienteEntrada = (transicion.simboloEntrada == Transiciones::EPSILON)
                                              ? entrada
                                              : entrada.substr(1);

    camino.push_back(transicion.id);
    if (buscar(transicion.destino, siguienteEntrada, siguientePila, visitados, camino)) {
      aceptado = true;
      break;
    }
    camino.pop_back();
  }

  visitados.pop_back();
  return aceptado;
}