/**
 * @author Keran Miranda González
 * @file Transiciones.h
 * @brief Declaracion de la clase Transiciones.
 *
 * Agrupa todas las reglas de la funcion de transicion de un automata con
 * pila y ofrece consultas para obtener las transiciones aplicables desde
 * un estado y una cima de pila dados, distinguiendo entre las que
 * consumen un simbolo de entrada y las transiciones-epsilon.
 *
 * A cada transicion se le asigna automaticamente un identificador segun
 * el orden en que se anade (que debe coincidir con el orden en que
 * aparece en el fichero de configuracion), para poder referirse a ella
 * de forma compacta en la traza de ejecucion (p. ej. "se aplica T7").
 */

#ifndef TRANSICIONES_H
#define TRANSICIONES_H

#include <string>
#include <vector>

#include "transicion.h"

/**
 * @class Transiciones
 * @brief Coleccion de reglas de transicion con consultas por estado y
 *        simbolo de cima de pila.
 */
class Transiciones {
public:
    /// Simbolo reservado para representar epsilon en simboloEntrada.
    static constexpr char EPSILON = '\0';

    /**
     * @brief Construye una coleccion de transiciones vacia.
     */
    Transiciones() = default;

    /**
     * @brief Añade una nueva transicion a la coleccion.
     *
     * El identificador se asigna automaticamente como el siguiente
     * numero disponible (empezando en 1), segun el orden de llamadas a
     * este metodo.
     *
     * @param origen Estado origen.
     * @param simboloEntrada Simbolo leido, o Transiciones::EPSILON si la
     *        transicion no consume simbolo de entrada.
     * @param simboloPila Simbolo de la cima de la pila que se consume.
     * @param destino Estado destino.
     * @param pila Cadena a apilar en sustitucion de simboloPila (cadena
     *        vacia equivale a epsilon).
     * @return Identificador asignado a la transicion anadida.
     */
    int agregar(const std::string& origen,
                char simboloEntrada,
                char simboloPila,
                const std::string& destino,
                const std::string& pila);

    /**
     * @brief Obtiene las transiciones aplicables que consumen un simbolo
     *        de entrada concreto.
     *
     * @param estado Estado origen desde el que se busca.
     * @param simboloEntrada Simbolo de entrada a consumir.
     * @param simboloPila Simbolo que debe estar en la cima de la pila.
     * @return Transiciones cuyo origen, simbolo de entrada y simbolo de
     *         pila coinciden con los indicados.
     */
    std::vector<Transicion> aplicables(const std::string& estado,
                                        char simboloEntrada,
                                        char simboloPila) const;

    /**
     * @brief Obtiene las transiciones-epsilon aplicables (no consumen
     *        simbolo de entrada).
     *
     * @param estado Estado origen desde el que se busca.
     * @param simboloPila Simbolo que debe estar en la cima de la pila.
     * @return Transiciones cuyo origen y simbolo de pila coinciden con
     *         los indicados y cuyo simbolo de entrada es EPSILON.
     */
    std::vector<Transicion> aplicablesEpsilon(const std::string& estado,
                                               char simboloPila) const;

    /**
     * @brief Numero total de transiciones almacenadas.
     * @return Cantidad de transiciones.
     */
    std::size_t tamano() const;

    /**
     * @brief Acceso de solo lectura a todas las transiciones, en el
     *        orden en que se anadieron. Pensado para que Automata pueda
     *        validar que cada transicion usa simbolos y estados
     *        declarados.
     * @return Referencia constante al vector interno de transiciones.
     */
    const std::vector<Transicion>& todas() const;

private:
    std::vector<Transicion> lista;  ///< Transiciones almacenadas, en orden de llegada.
};

#endif  // TRANSICIONES_H