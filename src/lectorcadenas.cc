/**
 * @file LectorCadenas.cc
 * @brief Implementacion de la clase LectorCadenas.
 */

#include "LectorCadenas.h"

#include "Errores.h"

/**
 * @brief Construye un lector que lee por teclado.
 */
LectorCadenas::LectorCadenas() : entrada(&std::cin), desdeTeclado(true) {}

/**
 * @brief Construye un lector que lee de un fichero de texto.
 */
LectorCadenas::LectorCadenas(const std::string& rutaFichero) : entrada(nullptr), desdeTeclado(false) {
  ficheroEntrada.open(rutaFichero);
  if (!ficheroEntrada.is_open()) {
    throw ErrorFichero("No se ha podido abrir el fichero de cadenas de entrada: " + rutaFichero);
  }
  entrada = &ficheroEntrada;
}

/**
 * @brief Obtiene la siguiente cadena de entrada disponible.
 *
 * Se recorta un posible '\r' final (ficheros con final de linea de
 * Windows), pero no se recorta ningun otro espacio: la cadena se pasa
 * tal cual al Simulador.
 */
bool LectorCadenas::siguienteCadena(std::string& cadena) {
  if (desdeTeclado) {
    std::cout << "Introduzca una cadena (Ctrl+D para terminar): ";
  }

  if (!std::getline(*entrada, cadena)) {
    return false;
  }

  if (!cadena.empty() && cadena.back() == '\r') {
    cadena.pop_back();
  }

  return true;
}