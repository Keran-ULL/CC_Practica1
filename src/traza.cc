/**
 * @file Traza.cc
 * @brief Implementacion de la clase Traza.
 */

#include "traza.h"

#include <iomanip>

#include "errores.h"

namespace {
constexpr int kAnchoIteracion = 6;
constexpr int kAnchoEstado = 10;
constexpr int kAnchoCadena = 18;
constexpr int kAnchoPila = 18;
constexpr int kAnchoPosibles = 22;
constexpr int kAnchoElegida = 10;
}  

/**
 * @brief Construye una traza que escribe por pantalla.
 */
Traza::Traza() : salida(&std::cout), iteracion(0) {}

/**
 * @brief Construye una traza que escribe a un fichero de texto.
 */
Traza::Traza(const std::string& rutaFichero) : salida(nullptr), iteracion(0) {
  ficheroSalida.open(rutaFichero);
  if (!ficheroSalida.is_open()) {
    throw ErrorFichero("No se ha podido abrir el fichero de traza para escritura: " + rutaFichero);
  }
  salida = &ficheroSalida;
}

/**
 * @brief Convierte una cadena a su representacion en la traza.
 */
std::string Traza::representar(const std::string& texto) {
  return texto.empty() ? "." : texto;
}

/**
 * @brief Convierte un simbolo de entrada a su representacion en la
 *        traza.
 */
std::string Traza::representarSimboloEntrada(char simbolo) {
  return (simbolo == Transiciones::EPSILON) ? "." : std::string(1, simbolo);
}

/**
 * @brief Construye la lista "T2,T5,T7" a partir de unos IDs.
 */
std::string Traza::listarIds(const std::vector<int>& ids) {
  if (ids.empty()) {
    return "-";
  }
  std::string resultado;
  for (std::size_t i = 0; i < ids.size(); ++i) {
    if (i > 0) {
      resultado += ",";
    }
    resultado += "T" + std::to_string(ids[i]);
  }
  return resultado;
}

/**
 * @brief Muestra, una unica vez, el listado completo de la funcion de
 *        transicion.
 */
void Traza::listarTransiciones(const std::vector<Transicion>& transiciones) {
  *salida << "===== Transiciones del automata =====\n";
  for (const Transicion& transicion : transiciones) {
    *salida << "  T" << transicion.id << ": (" << transicion.origen << ", "
            << representarSimboloEntrada(transicion.simboloEntrada) << ", "
            << transicion.simboloPila << ") -> (" << transicion.destino << ", "
            << representar(transicion.pila) << ")\n";
  }
  *salida << '\n';
}

/**
 * @brief Abre una tabla nueva para trazar una cadena.
 */
void Traza::comenzarCadena(const std::string& cadena) {
  iteracion = 0;
  *salida << "\n===== Comprobando cadena: \"" << representar(cadena) << "\" =====\n";
  *salida << std::left
          << std::setw(kAnchoIteracion) << "Iter"
          << std::setw(kAnchoEstado) << "Estado"
          << std::setw(kAnchoCadena) << "Cadena rest."
          << std::setw(kAnchoPila) << "Pila (cima-base)"
          << std::setw(kAnchoPosibles) << "Posibles"
          << std::setw(kAnchoElegida) << "Elegida"
          << '\n';
  *salida << std::string(kAnchoIteracion + kAnchoEstado + kAnchoCadena +
                              kAnchoPila + kAnchoPosibles + kAnchoElegida,
                          '-')
          << '\n';
}

/**
 * @brief Registra, como una fila de la tabla, un intento de aplicar una
 *        transicion.
 */
void Traza::registrar(const std::string& estado,
                       const std::string& cadenaRestante,
                       const std::string& pila,
                       const std::vector<int>& idsPosibles,
                       int idTransicionElegida) {
  ++iteracion;
  const std::string elegida =
      (idTransicionElegida > 0) ? ("T" + std::to_string(idTransicionElegida)) : "-";
  *salida << std::left
          << std::setw(kAnchoIteracion) << iteracion
          << std::setw(kAnchoEstado) << estado
          << std::setw(kAnchoCadena) << representar(cadenaRestante)
          << std::setw(kAnchoPila) << representar(pila)
          << std::setw(kAnchoPosibles) << listarIds(idsPosibles)
          << std::setw(kAnchoElegida) << elegida
          << '\n';
}

/**
 * @brief Muestra el resultado de comprobar la cadena actual.
 */
void Traza::mostrarResultado(bool aceptada, const std::vector<int>& camino) {
  if (aceptada) {
    *salida << "Resultado: ACEPTADA. Camino: " << listarIds(camino) << '\n';
  } 
  else {
    *salida << "Resultado: NO ACEPTADA (no existe ningun camino de aceptacion).\n";
  }
}