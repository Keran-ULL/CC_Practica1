/**
 * @author Keran Miranda González
 * @file LectorAutomata.h
 * @brief Declaracion de la clase LectorAutomata.
 *
 * Se encarga de leer un fichero de texto con la definicion de un
 * automata con pila (formato descrito en el guion de la practica) y
 * construir el objeto Automata correspondiente. Toda la logica de
 * parseo vive aqui; la validacion semantica de la definicion formal la
 * realiza el propio Automata en su constructor.
 *
 * De momento se asume que el automata implementado finaliza por estado
 * final (APf), por lo que siempre se espera la linea con el conjunto de
 * estados finales F.
 */

#ifndef LECTOR_AUTOMATA_H
#define LECTOR_AUTOMATA_H

#include <fstream>
#include <string>
#include <vector>

#include "automata.h"

/**
 * @class LectorAutomata
 * @brief Lector del fichero de configuracion de un automata con pila.
 */
class LectorAutomata {
public:
  /**
   * @brief Lee y parsea el fichero de configuracion indicado.
   * @param ruta Ruta del fichero de texto con la definicion del
   *        automata.
   * @return El Automata construido y validado a partir del fichero.
   * @throws ErrorFichero Si el fichero no se puede abrir.
   * @throws ErrorFormato Si alguna linea no respeta el formato
   *         esperado (numero de campos, simbolos de un solo caracter,
   *         etc.).
   * @throws ErrorValidacion Si el automata resultante no cumple las
   *         restricciones de su definicion formal.
   */
  static Automata leer(const std::string& ruta);

private:
  /**
   * @brief Lee la siguiente linea "significativa" del fichero,
   *        ignorando lineas en blanco y descartando los comentarios
   *        (todo lo que sigue a un '#').
   *
   * @param fichero Flujo de entrada del que se lee.
   * @param linea Donde se guarda la linea significativa leida, ya sin
   *        comentario y sin espacios sobrantes al principio o al final.
   * @return true si se ha leido una linea, false si se ha llegado al
   *         final del fichero sin encontrar ninguna.
   */
  static bool leerLineaSignificativa(std::ifstream& fichero, std::string& linea);

  /**
   * @brief Divide una linea en las palabras separadas por espacios en
   *        blanco.
   *
   * @param linea Linea a dividir.
   * @return Vector con las palabras encontradas, en el orden en que
   *         aparecen.
   */
  static std::vector<std::string> dividirEnPalabras(const std::string& linea);

  /**
   * @brief Comprueba que una palabra representa un unico simbolo valido
   *        (un solo caracter, distinto del punto reservado para
   *        epsilon) y lo devuelve.
   *
   * @param palabra Palabra a comprobar.
   * @param contexto Descripcion de donde aparece la palabra, para poder
   *        dar un mensaje de error claro.
   * @return El caracter que representa la palabra.
   * @throws ErrorFormato Si la palabra no es un simbolo valido de un
   *         unico caracter.
   */
  static char aSimbolo(const std::string& palabra, const std::string& contexto);
};

#endif  