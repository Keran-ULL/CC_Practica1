/**
 * @file Automata.cc
 * @brief Implementacion de la clase Automata.
 */

#include "automata.h"

#include <algorithm>
#include <string>
#include <utility>

#include "errores.h"

/**
 * @brief Construye y valida un automata con pila.
 *
 * Se guardan todos los elementos recibidos y, a continuacion, se llama a
 * validar() para comprobar que forman una definicion de automata
 * coherente. Si la validacion falla, la excepcion se propaga y el
 * objeto Automata no llega a existir.
 */
Automata::Automata(std::vector<std::string> estados,
                    Alfabeto alfabetoEntrada,
                    Alfabeto alfabetoPila,
                    std::string estadoInicial,
                    char simboloInicialPila,
                    std::vector<std::string> estadosFinales,
                    Transiciones transiciones)
    : estados(std::move(estados)),
      alfabetoEntrada(std::move(alfabetoEntrada)),
      alfabetoPila(std::move(alfabetoPila)),
      estadoInicial(std::move(estadoInicial)),
      simboloInicialPila(simboloInicialPila),
      estadosFinales(std::move(estadosFinales)),
      transiciones(std::move(transiciones)) {
  validar();
}

/**
 * @brief Comprueba las restricciones de la definicion formal.
 *
 * Se valida: que Q no este vacio, que el estado inicial pertenezca a Q,
 * que el simbolo inicial de pila pertenezca a Gamma, que F sea
 * subconjunto de Q, y que cada transicion use unicamente estados de Q y
 * simbolos declarados en Sigma/Gamma (incluidos los simbolos que apila).
 */
void Automata::validar() const {
  if (estados.empty()) {
    throw ErrorValidacion(
        "El conjunto de estados Q no puede estar vacio");
  }

  if (!esEstado(estadoInicial)) {
    throw ErrorValidacion(
        "El estado inicial '" + estadoInicial + "' no pertenece a Q");
  }

  if (!alfabetoPila.pertenece(simboloInicialPila)) {
    throw ErrorValidacion(
        std::string("El simbolo inicial de pila '") +
        simboloInicialPila + "' no pertenece a Gamma");
  }

  for (const std::string& final : estadosFinales) {
    if (!esEstado(final)) {
      throw ErrorValidacion(
          "El estado final '" + final + "' no pertenece a Q");
    }
  }

  for (const Transicion& transicion : transiciones.todas()) {
    if (!esEstado(transicion.origen)) {
      throw ErrorValidacion(
          "La transicion T" + std::to_string(transicion.id) +
          " usa un estado origen ('" + transicion.origen +
          "') que no pertenece a Q");
    }
    if (!esEstado(transicion.destino)) {
      throw ErrorValidacion(
          "La transicion T" + std::to_string(transicion.id) +
          " usa un estado destino ('" + transicion.destino +
          "') que no pertenece a Q");
    }
    if (transicion.simboloEntrada != Transiciones::EPSILON &&
        !alfabetoEntrada.pertenece(transicion.simboloEntrada)) {
      throw ErrorValidacion(
          "La transicion T" + std::to_string(transicion.id) +
          " usa un simbolo de entrada que no pertenece a Sigma");
    }
    if (!alfabetoPila.pertenece(transicion.simboloPila)) {
      throw ErrorValidacion(
          "La transicion T" + std::to_string(transicion.id) +
          " usa un simbolo de pila que no pertenece a Gamma");
    }
    for (char simboloApilado : transicion.pila) {
      if (!alfabetoPila.pertenece(simboloApilado)) {
        throw ErrorValidacion(
            "La transicion T" + std::to_string(transicion.id) +
            " apila un simbolo que no pertenece a Gamma");
      }
    }
  }
}

/**
 * @brief Consulta si un estado pertenece al automata.
 */
bool Automata::esEstado(const std::string& estado) const {
  return std::find(estados.begin(), estados.end(), estado) != estados.end();
}

/**
 * @brief Consulta si un estado es final.
 */
bool Automata::esFinal(const std::string& estado) const {
  return std::find(estadosFinales.begin(), estadosFinales.end(), estado) != estadosFinales.end();
}

/**
 * @brief Conjunto de estados del automata, Q.
 */
const std::vector<std::string>& Automata::getEstados() const {
  return estados;
}

/**
 * @brief Alfabeto de entrada del automata, Sigma.
 */
const Alfabeto& Automata::getAlfabetoEntrada() const {
  return alfabetoEntrada;
}

/**
 * @brief Alfabeto de pila del automata, Gamma.
 */
const Alfabeto& Automata::getAlfabetoPila() const {
  return alfabetoPila;
}

/**
 * @brief Estado inicial del automata, q0.
 */
const std::string& Automata::getEstadoInicial() const {
  return estadoInicial;
}

/**
 * @brief Simbolo inicial de la pila, Z0.
 */
char Automata::getSimboloInicialPila() const {
  return simboloInicialPila;
}

/**
 * @brief Conjunto de estados finales del automata, F.
 */
const std::vector<std::string>& Automata::getEstadosFinales() const {
  return estadosFinales;
}

/**
 * @brief Funcion de transicion del automata.
 */
const Transiciones& Automata::getTransiciones() const {
  return transiciones;
}