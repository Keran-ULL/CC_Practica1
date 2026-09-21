/**
 * @file Traza.cc
 * @brief Implementacion de la clase Traza.
 */

#include "traza.h"

#include "errores.h"

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
 * @brief Construye la lista "T2, T5, T7" a partir de unos IDs.
 */
std::string Traza::listarIds(const std::vector<int>& ids) {
  if (ids.empty()) {
    return "(ninguna)";
  }

  std::string resultado;
  for (std::size_t i = 0; i < ids.size(); ++i) {
    if (i > 0) {
      resultado += ", ";
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
 * @brief Registra un intento de aplicar una transicion.
 *
 * La transicion elegida se resalta con una flecha delante de la linea y
 * con la etiqueta explicita del ID elegido, para que se distinga a
 * simple vista incluso en un fichero de texto plano.
 */
void Traza::registrar(const std::string& estado,
                       const std::string& cadenaRestante,
                       const std::string& pila,
                       const std::vector<int>& idsPosibles,
                       int idTransicionElegida) {
  ++iteracion;

  const std::string elegida = (idTransicionElegida > 0)
                                   ? ("T" + std::to_string(idTransicionElegida))
                                   : "(ninguna, callejon sin salida)";

  *salida << "----- Iteracion " << iteracion << " -----\n";
  *salida << "Estado actual        : " << estado << '\n';
  *salida << "Cadena restante      : " << representar(cadenaRestante) << '\n';
  *salida << "Pila (cima->base)    : " << representar(pila) << '\n';
  *salida << "Transiciones posibles: " << listarIds(idsPosibles) << '\n';
  *salida << "  -> Transicion elegida: " << elegida << "\n\n";
}

/**
 * @brief Muestra el resultado final de la simulacion.
 */
void Traza::mostrarResultado(bool aceptada, const std::vector<int>& camino) {
  *salida << "===== Resultado =====\n";
  if (aceptada) {
    *salida << "La cadena ES ACEPTADA.\n";
    *salida << "Camino de transiciones: " << listarIds(camino) << '\n';
  } else {
    *salida << "La cadena NO es aceptada (no existe ningun camino de aceptacion).\n";
  }
}