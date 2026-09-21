/**
 * @file Transiciones.cc
 * @brief Implementacion de la clase Transiciones.
 */

#include "transiciones.h"

/**
 * @brief Anade una nueva transicion a la coleccion.
 *
 * El identificador es simplemente la posicion (1-indexada) que ocupara
 * la transicion en el vector interno, por lo que coincide con el orden
 * de llegada.
 */
int Transiciones::agregar(const std::string& origen,
                           char simboloEntrada,
                           char simboloPila,
                           const std::string& destino,
                           const std::string& pila) {
  const int id = static_cast<int>(lista.size()) + 1;
  lista.push_back(Transicion{id, origen, simboloEntrada, simboloPila, destino, pila});
  return id;
}

/**
 * @brief Obtiene las transiciones aplicables que consumen un simbolo de
 *        entrada concreto.
 *
 * Busqueda lineal sobre todas las transiciones: para el numero de
 * transiciones habitual en estos automatas es suficiente y mantiene la
 * clase simple.
 */
std::vector<Transicion> Transiciones::aplicables(const std::string& estado,
                                                   char simboloEntrada,
                                                   char simboloPila) const {
  std::vector<Transicion> resultado;
  for (const Transicion& transicion : lista) {
    if (transicion.origen == estado &&
      transicion.simboloEntrada == simboloEntrada &&
      transicion.simboloPila == simboloPila) {
      resultado.push_back(transicion);
    }
  }
  return resultado;
}

/**
 * @brief Obtiene las transiciones-epsilon aplicables.
 */
std::vector<Transicion> Transiciones::aplicablesEpsilon(const std::string& estado, char simboloPila) const {
  std::vector<Transicion> resultado;
  for (const Transicion& transicion : lista) {
    if (transicion.origen == estado &&
      transicion.simboloEntrada == EPSILON &&
      transicion.simboloPila == simboloPila) {
      resultado.push_back(transicion);
    }
  }
  return resultado;
}

/**
 * @brief Numero total de transiciones almacenadas.
 */
std::size_t Transiciones::tamano() const {
  return lista.size();
}

/**
 * @brief Acceso de solo lectura a todas las transiciones.
 */
const std::vector<Transicion>& Transiciones::todas() const {
  return lista;
}