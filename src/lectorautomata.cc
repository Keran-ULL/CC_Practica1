/**
 * @file LectorAutomata.cc
 * @brief Implementacion de la clase LectorAutomata.
 */

#include "lectorautomata.h"

#include <sstream>

#include "errores.h"
#include "transiciones.h"

/**
 * @brief Lee y parsea el fichero de configuracion indicado.
 *
 * El fichero debe seguir el orden fijo: Q, Sigma, Gamma, estado
 * inicial, simbolo inicial de pila, F, y a continuacion una transicion
 * por linea hasta el final del fichero.
 */
Automata LectorAutomata::leer(const std::string& ruta) {
  std::ifstream fichero(ruta);
  if (!fichero.is_open()) {
    throw ErrorFichero("No se ha podido abrir el fichero de configuracion: " + ruta);
  }
  std::string linea;
  if (!leerLineaSignificativa(fichero, linea)) {
    throw ErrorFormato("Falta la linea con el conjunto de estados Q");
  }
  std::vector<std::string> estados = dividirEnPalabras(linea);
  if (!leerLineaSignificativa(fichero, linea)) {
    throw ErrorFormato("Falta la linea con el alfabeto de entrada Sigma");
  }
  Alfabeto alfabetoEntrada;
  for (const std::string& palabra : dividirEnPalabras(linea)) {
    alfabetoEntrada.agregar(aSimbolo(palabra, "el alfabeto de entrada Sigma"));
  }
  if (!leerLineaSignificativa(fichero, linea)) {
    throw ErrorFormato("Falta la linea con el alfabeto de pila Gamma");
  }
  Alfabeto alfabetoPila;
  for (const std::string& palabra : dividirEnPalabras(linea)) {
    alfabetoPila.agregar(aSimbolo(palabra, "el alfabeto de pila Gamma"));
  }
  if (!leerLineaSignificativa(fichero, linea)) {
    throw ErrorFormato("Falta la linea con el estado inicial");
  }
  std::vector<std::string> palabrasEstadoInicial = dividirEnPalabras(linea);
  if (palabrasEstadoInicial.size() != 1) {
    throw ErrorFormato("La linea del estado inicial debe contener un unico estado");
  }
  std::string estadoInicial = palabrasEstadoInicial[0];
  if (!leerLineaSignificativa(fichero, linea)) {
    throw ErrorFormato("Falta la linea con el simbolo inicial de pila");
  }
  std::vector<std::string> palabrasSimboloInicial = dividirEnPalabras(linea);
  if (palabrasSimboloInicial.size() != 1) {
    throw ErrorFormato("La linea del simbolo inicial de pila debe contener un unico simbolo");
  }
  char simboloInicialPila = aSimbolo(palabrasSimboloInicial[0], "el simbolo inicial de pila");
  if (!leerLineaSignificativa(fichero, linea)) {
    throw ErrorFormato("Falta la linea con el conjunto de estados finales F");
  }
  std::vector<std::string> estadosFinales = dividirEnPalabras(linea);
  Transiciones transiciones;
  while (leerLineaSignificativa(fichero, linea)) {
    std::vector<std::string> palabras = dividirEnPalabras(linea);
    if (palabras.size() != 5) {
      throw ErrorFormato(
          "Una transicion debe tener 5 campos: origen simboloEntrada simboloPila destino pila");
    }

    const std::string& origen = palabras[0];
    char simboloEntrada = (palabras[1] == ".")
                               ? Transiciones::EPSILON
                               : aSimbolo(palabras[1], "el simbolo de entrada de una transicion");
    char simboloPila = aSimbolo(palabras[2], "el simbolo de pila de una transicion");
    const std::string& destino = palabras[3];
    std::string pila = (palabras[4] == ".") ? std::string() : palabras[4];

    transiciones.agregar(origen, simboloEntrada, simboloPila, destino, pila);
  }

  return Automata(estados, alfabetoEntrada, alfabetoPila, estadoInicial,
                  simboloInicialPila, estadosFinales, transiciones);
}

/**
 * @brief Lee la siguiente linea significativa del fichero.
 *
 * Descarta todo lo que aparezca a partir de un '#' (comentario), recorta
 * espacios en blanco al principio y al final, y salta las lineas que
 * queden vacias tras ese proceso.
 */
bool LectorAutomata::leerLineaSignificativa(std::ifstream& fichero, std::string& linea) {
  std::string bruta;
  while (std::getline(fichero, bruta)) {
    const std::size_t posComentario = bruta.find('#');
    std::string sinComentario = (posComentario == std::string::npos)
                                     ? bruta
                                     : bruta.substr(0, posComentario);

    const std::size_t inicio = sinComentario.find_first_not_of(" \t\r");
    if (inicio == std::string::npos) {
      continue;
    }
    const std::size_t fin = sinComentario.find_last_not_of(" \t\r");
    linea = sinComentario.substr(inicio, fin - inicio + 1);
    return true;
  }
  return false;
}

/**
 * @brief Divide una linea en palabras separadas por espacios en blanco.
 */
std::vector<std::string> LectorAutomata::dividirEnPalabras(const std::string& linea) {
  std::vector<std::string> palabras;
  std::istringstream flujo(linea);
  std::string palabra;
  while (flujo >> palabra) {
    palabras.push_back(palabra);
  }
  return palabras;
}

/**
 * @brief Comprueba que una palabra representa un unico simbolo valido.
 *
 * El punto '.' esta reservado para representar epsilon en el fichero,
 * por lo que nunca es un simbolo valido en si mismo.
 */
char LectorAutomata::aSimbolo(const std::string& palabra, const std::string& contexto) {
  if (palabra.size() != 1 || palabra == ".") {
    throw ErrorFormato("'" + palabra + "' no es un simbolo valido de un unico caracter en " + contexto);
  }
  return palabra[0];
}