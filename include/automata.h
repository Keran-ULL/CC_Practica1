/**
 * @author Keran Miranda González
 * @file Automata.h
 * @brief Declaracion de la clase Automata.
 *
 * Modela la definicion formal de un automata con pila: conjunto de
 * estados, alfabeto de entrada, alfabeto de pila, estado inicial,
 * simbolo inicial de pila y conjunto de estados finales (este ultimo
 * solo se usa si el automata finaliza por estado final, APf).
 *
 * La funcion de transicion (clase Transiciones) se incorporara en una
 * iteracion posterior; por ahora Automata valida y expone el resto de
 * elementos de la definicion formal.
 */

#ifndef AUTOMATA_H
#define AUTOMATA_H

#include <string>
#include <vector>

#include "alfabeto.h"
#include "transiciones.h"

/**
 * @class Automata
 * @brief Definicion formal de un automata con pila.
 *
 * El constructor valida que los elementos recibidos cumplen las
 * restricciones de la definicion formal (p. ej. que el estado inicial
 * pertenezca a Q, o que cada transicion use estados y simbolos
 * declarados). Si alguna no se cumple, lanza una excepcion y el objeto
 * no llega a construirse: un Automata valido nunca queda en un estado
 * inconsistente.
 */
class Automata {
public:
  /**
   * @brief Construye y valida un automata con pila.
   *
   * @param estados Conjunto de estados, Q.
   * @param alfabetoEntrada Alfabeto de entrada, Sigma.
   * @param alfabetoPila Alfabeto de pila, Gamma.
   * @param estadoInicial Estado inicial, q0. Debe pertenecer a estados.
   * @param simboloInicialPila Simbolo inicial de pila, Z0. Debe
   *        pertenecer a alfabetoPila.
   * @param estadosFinales Conjunto de estados finales, F. Debe ser un
   *        subconjunto de estados (puede estar vacio si se va a
   *        implementar el automata por vaciado de pila).
   * @param transiciones Funcion de transicion completa. Cada regla debe
   *        usar unicamente estados de estados y simbolos de
   *        alfabetoEntrada (o epsilon) y de alfabetoPila.
   * @throws ErrorValidacion Si no se cumple alguna restriccion de la
   *         definicion formal.
   */
  Automata(std::vector<std::string> estados,
           Alfabeto alfabetoEntrada,
           Alfabeto alfabetoPila,
           std::string estadoInicial,
           char simboloInicialPila,
           std::vector<std::string> estadosFinales,
           Transiciones transiciones);

  /**
   * @brief Consulta si un estado pertenece al automata.
   * @param estado Nombre del estado a comprobar.
   * @return true si estado pertenece a Q.
   */
  bool esEstado(const std::string& estado) const;

  /**
   * @brief Consulta si un estado es final.
   * @param estado Nombre del estado a comprobar.
   * @return true si estado pertenece a F.
   */
  bool esFinal(const std::string& estado) const;

  /**
   * @brief Conjunto de estados del automata, Q.
   * @return Referencia constante al vector de estados.
   */
  const std::vector<std::string>& getEstados() const;

  /**
   * @brief Alfabeto de entrada del automata, Sigma.
   * @return Referencia constante al alfabeto de entrada.
   */
  const Alfabeto& getAlfabetoEntrada() const;

  /**
   * @brief Alfabeto de pila del automata, Gamma.
   * @return Referencia constante al alfabeto de pila.
   */
  const Alfabeto& getAlfabetoPila() const;

  /**
   * @brief Estado inicial del automata, q0.
   * @return Nombre del estado inicial.
   */
  const std::string& getEstadoInicial() const;

  /**
   * @brief Simbolo inicial de la pila, Z0.
   * @return Simbolo inicial de pila.
   */
  char getSimboloInicialPila() const;

  /**
   * @brief Conjunto de estados finales del automata, F.
   * @return Referencia constante al vector de estados finales.
   */
  const std::vector<std::string>& getEstadosFinales() const;

  /**
   * @brief Funcion de transicion del automata.
   * @return Referencia constante a la coleccion de transiciones.
   */
  const Transiciones& getTransiciones() const;

private:
  /**
   * @brief Comprueba que los elementos recibidos cumplen las
   *        restricciones de la definicion formal de un automata con
   *        pila, lanzando una excepcion si no es asi. Incluye
   *        comprobar que cada transicion usa unicamente estados y
   *        simbolos declarados.
   * @throws ErrorValidacion Si alguna restriccion no se cumple.
   */
  void validar() const;

  std::vector<std::string> estados;         ///< Q
  Alfabeto alfabetoEntrada;                  ///< Sigma
  Alfabeto alfabetoPila;                     ///< Gamma
  std::string estadoInicial;                 ///< q0
  char simboloInicialPila;                   ///< Z0
  std::vector<std::string> estadosFinales;   ///< F (solo se usa en APf)
  Transiciones transiciones;                 ///< delta
};

#endif  // AUTOMATA_H
