/**
 * @author Keran Miranda González
 * @file Pila.h
 * @brief Declaracion de la clase Pila.
 *
 * Implementacion propia de una pila de simbolos (no se usa std::stack)
 * para poder copiarla e inspeccionar su contenido con facilidad durante
 * la busqueda en profundidad del simulador y al generar la traza.
 */

#ifndef PILA_H
#define PILA_H

#include <string>
#include <vector>



/**
 * @class Pila
 * @brief Pila de simbolos de un unico caracter.
 */
class Pila {
public:
    /**
     * @brief Construye una pila vacia.
     */
    Pila() = default;

    /**
     * @brief Construye una pila con un unico simbolo inicial.
     * @param simboloInicial Simbolo que se apila como primer elemento.
     */
    explicit Pila(char simboloInicial);

    /**
     * @brief Apila una cadena de simbolos sobre la cima actual.
     *
     * Si la cadena esta vacia (equivalente a epsilon) la pila no cambia.
     *
     * @param simbolos Cadena de simbolos a apilar, de izquierda a
     *        derecha; simbolos[0] quedara como nueva cima.
     */
    void push(const std::string& cadena);

    /**
     * @brief Elimina el simbolo situado en la cima de la pila.
     * @throws std::out_of_range Si la pila esta vacia.
     */
    void pop();

    /**
     * @brief Consulta el simbolo situado en la cima de la pila.
     * @return El simbolo de la cima.
     * @throws std::out_of_range Si la pila esta vacia.
     */
    char cima() const;

    /**
     * @brief Comprueba si la pila no contiene ningun simbolo.
     * @return true si la pila esta vacia.
     */
    bool vacia() const;

    /**
     * @brief Numero de simbolos actualmente en la pila.
     * @return Tamano de la pila.
     */
    std::size_t tamano() const;

    /**
     * @brief Representacion textual de la pila, de cima a base, pensada
     *        para mostrarse en la traza de ejecucion.
     * @return Cadena con los simbolos de la pila, empezando por la cima.
     */
    std::string aTexto() const;

private:
    std::vector<char> simbolos;  
};

#endif  // PILA_H