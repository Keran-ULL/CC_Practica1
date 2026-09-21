/**
 * @author Keran Miranda González
 * @file Transicion.h
 * @brief Declaracion de la estructura Transicion.
 *
 * Representa una unica regla de la funcion de transicion de un automata
 * con pila: (destino, pila) pertenece a delta(origen, simboloEntrada,
 * simboloPila). Es un simple contenedor de datos, sin logica propia, por
 * lo que sus campos son publicos y no llevan sufijo "_" como los
 * atributos de clases con estado interno (p. ej. Automata).
 */

#ifndef TRANSICION_H
#define TRANSICION_H

#include <string>


/**
 * @struct Transicion
 * @brief Una regla de transicion, identificada por el orden en que
 *        aparece en el fichero de configuracion.
 */
struct Transicion {
  int id;                ///< Identificador (orden de aparicion en el fichero), empieza en 1.
  std::string origen;    ///< Estado origen.
  char simboloEntrada;   ///< Simbolo leido, o Transiciones::EPSILON si no consume entrada.
  char simboloPila;      ///< Simbolo de la cima de la pila que se consume.
  std::string destino;   ///< Estado destino.
  std::string pila;      ///< Cadena a apilar en sustitucion de simboloPila (vacia = epsilon).
};

#endif  // TRANSICION_H