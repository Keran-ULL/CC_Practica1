/**
 * @author Keran Miranda González
 * @file Traza.h
 * @brief Declaracion de la clase Traza.
 *
 * Muestra la ejecucion de la simulacion en tres partes:
 *  1) listarTransiciones(): una vez, al principio, el listado completo
 *     de la funcion de transicion (ID + regla), a modo de leyenda.
 *  2) registrar(): una vez por cada intento de aplicar una transicion
 *     (incluidos los caminos fallidos que se descartan al retroceder),
 *     mostrando el estado actual, la cadena restante, la pila, los IDs
 *     de las transiciones posibles y cual de ellas se elige.
 *  3) mostrarResultado(): una vez, al final, si la cadena se acepta y,
 *     en ese caso, el camino de transiciones (por ID) que lleva a la
 *     aceptacion.
 *
 * Puede escribir a pantalla o a un fichero, segun con que constructor se
 * cree. El simbolo epsilon se representa con el mismo caracter que en
 * los ficheros de entrada, el punto (.).
 */

#ifndef TRAZA_H
#define TRAZA_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "transicion.h"
#include "transiciones.h"

/**
 * @class Traza
 * @brief Registro, a pantalla o a fichero, de la ejecucion de la
 *        simulacion.
 */
class Traza {
public:
  /**
   * @brief Construye una traza que escribe por pantalla (std::cout).
   */
  Traza();

  /**
   * @brief Construye una traza que escribe a un fichero de texto.
   * @param rutaFichero Ruta del fichero donde se escribira la traza.
   * @throws ErrorFichero Si el fichero no se puede abrir para escritura.
   */
  explicit Traza(const std::string& rutaFichero);

  /**
   * @brief Muestra, una unica vez, el listado completo de la funcion de
   *        transicion, para poder consultar despues cada ID citado en
   *        el resto de la traza.
   * @param transiciones Todas las transiciones del automata, en el
   *        orden en que aparecen en el fichero de configuracion.
   */
  void listarTransiciones(const std::vector<Transicion>& transiciones);

  /**
   * @brief Registra un intento de aplicar una transicion.
   *
   * @param estado Estado actual.
   * @param cadenaRestante Parte de la cadena de entrada que aun queda
   *        por consumir.
   * @param pila Contenido actual de la pila, de cima a base (ver
   *        Pila::aTexto()).
   * @param idsPosibles IDs de todas las transiciones aplicables desde
   *        la configuracion actual (puede estar vacio si no hay
   *        ninguna, es decir, un callejon sin salida).
   * @param idTransicionElegida ID de la transicion que se va a intentar
   *        a continuacion, o 0 si ninguna de las candidatas se elige.
   */
  void registrar(const std::string& estado,
                 const std::string& cadenaRestante,
                 const std::string& pila,
                 const std::vector<int>& idsPosibles,
                 int idTransicionElegida);

  /**
   * @brief Muestra el resultado final de la simulacion.
   *
   * @param aceptada true si la cadena pertenece al lenguaje reconocido.
   * @param camino IDs de las transiciones aplicadas, en orden, a lo
   *        largo del camino que lleva a la aceptacion. Se ignora si
   *        aceptada es false.
   */
  void mostrarResultado(bool aceptada, const std::vector<int>& camino);

private:
  /**
   * @brief Convierte una cadena a su representacion en la traza,
   *        mostrando "." cuando esta vacia (equivale a epsilon).
   * @param texto Cadena a representar.
   * @return texto si no esta vacia, o "." si lo esta.
   */
  static std::string representar(const std::string& texto);

  /**
   * @brief Convierte un simbolo de entrada a su representacion en la
   *        traza, mostrando "." cuando es Transiciones::EPSILON.
   * @param simbolo Simbolo a representar.
   * @return El simbolo como cadena de un caracter, o "." si es epsilon.
   */
  static std::string representarSimboloEntrada(char simbolo);

  /**
   * @brief Construye la lista "T2, T5, T7" a partir de unos IDs.
   * @param ids IDs de transicion a listar, en orden.
   * @return Los IDs formateados y separados por comas, o "(ninguna)" si
   *         el vector esta vacio.
   */
  static std::string listarIds(const std::vector<int>& ids);

  std::ofstream ficheroSalida;  ///< Fichero de salida, solo si se traza a fichero.
  std::ostream* salida;         ///< Apunta a std::cout o a ficheroSalida, segun el constructor usado.
  int iteracion;                ///< Numero de iteracion actual, para numerar la tabla.
};

#endif  // TRAZA_H