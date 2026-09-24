/**
 * @file ArgParser.cc
 * @brief Implementacion de la clase ArgParser.
 */

#include "argParser.h"

#include "errores.h"

/**
 * @brief Analiza los argumentos de linea de comandos.
 *
 * Recorre argv de izquierda a derecha reconociendo las cuatro opciones
 * soportadas; cualquier otra cosa se considera un error. Al final se
 * comprueba que las dos opciones obligatorias (-config y -trace) hayan
 * aparecido.
 */
Opciones ArgParser::parsear(int argc, char* argv[]) {
  Opciones opciones;
  opciones.trace = false;
  bool traceEspecificado = false;

  int i = 1;
  while (i < argc) {
    const std::string argumento = argv[i];

    if (argumento == "-config") {
      opciones.ficheroConfig = obtenerValor(argc, argv, i, "-config");
    } else if (argumento == "-trace") {
      opciones.trace = aBooleano(obtenerValor(argc, argv, i, "-trace"));
      traceEspecificado = true;
    } else if (argumento == "-in") {
      opciones.ficheroEntrada = obtenerValor(argc, argv, i, "-in");
    } else if (argumento == "-out") {
      opciones.ficheroSalida = obtenerValor(argc, argv, i, "-out");
    } else {
      throw ErrorArgumentos("Opcion no reconocida: '" + argumento + "'");
    }

    ++i;
  }

  if (opciones.ficheroConfig.empty()) {
    throw ErrorArgumentos("Falta la opcion obligatoria -config <f>");
  }
  if (!traceEspecificado) {
    throw ErrorArgumentos("Falta la opcion obligatoria -trace <y|n>");
  }

  return opciones;
}

/**
 * @brief Obtiene el valor asociado a una opcion y avanza el indice.
 */
std::string ArgParser::obtenerValor(int argc, char* argv[], int& indice, const std::string& opcion) {
  if (indice + 1 >= argc) {
    throw ErrorArgumentos("A la opcion '" + opcion + "' le falta el valor");
  }
  ++indice;
  return argv[indice];
}

/**
 * @brief Interpreta el valor de -trace como booleano.
 */
bool ArgParser::aBooleano(const std::string& valor) {
  if (valor == "y" || valor == "Y") {
    return true;
  }
  if (valor == "n" || valor == "N") {
    return false;
  }
  throw ErrorArgumentos("El valor de -trace debe ser 'y' o 'n', se ha recibido: '" + valor + "'");
}