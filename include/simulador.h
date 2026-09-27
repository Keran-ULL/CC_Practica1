/**
 * @author Keran Miranda González
 * @file Simulador.h
 * @brief Declaracion de la clase Simulador.
 *
 * Motor de busqueda en profundidad (DFS) que determina si una cadena de
 * entrada es aceptada por un automata con pila. El automata puede ser no
 * determinista (varias transiciones aplicables desde una misma
 * configuracion, incluidas transiciones-epsilon), asi que el simulador
 * prueba las alternativas con retroceso hasta encontrar un camino de
 * aceptacion o agotarlas todas.
 *
 * El criterio de aceptacion (por estado final, APf) esta integrado
 * directamente en esta clase por ser una unica comprobacion muy
 * sencilla; si en el futuro se quisiera soportar tambien el automata por
 * vaciado de pila (APv), se puede extraer a una clase/estrategia aparte
 * sin cambiar el resto del motor de busqueda.
 *
 * Si se le proporciona una Traza (opcional), se registra cada intento
 * de aplicar una transicion (incluidos los caminos fallidos que se
 * descartan al retroceder) y, al terminar, el resultado final con el
 * camino de transiciones que lleva a la aceptacion, si existe.
 */

#ifndef SIMULADOR_H
#define SIMULADOR_H

#include <string>
#include <vector>

#include "automata.h"
#include "pila.h"
#include "transicion.h"
#include "traza.h"

/**
 * @class Simulador
 * @brief Ejecuta la busqueda en profundidad sobre un Automata para
 *        comprobar si acepta una cadena de entrada.
 */
class Simulador {
public:
  /**
   * @brief Construye un simulador para un automata concreto.
   * @param automata Automata sobre el que se va a simular. Debe seguir
   *        siendo valido durante toda la vida del Simulador (no se
   *        copia, se guarda una referencia).
   * @param traza Traza donde registrar la ejecucion, o nullptr (por
   *        defecto) si no se quiere trazar. No se posee: quien la creo
   *        sigue siendo responsable de su ciclo de vida.
   */
  explicit Simulador(const Automata& automata, Traza* traza = nullptr);

  /**
   * @brief Comprueba si una cadena pertenece al lenguaje reconocido por
   *        el automata.
   * @param cadena Cadena de entrada a comprobar (puede ser vacia, es
   *        decir, epsilon).
   * @return true si existe al menos un camino de computo que termina en
   *         un estado final tras consumir toda la cadena.
   */
  bool acepta(const std::string& cadena) const;

private:
  /**
   * @brief Obtiene todas las transiciones aplicables desde una
   *        configuracion, combinando las que consumen el siguiente
   *        simbolo de entrada (si queda alguno) con las
   *        transiciones-epsilon.
   *
   * @param estado Estado actual.
   * @param entrada Parte de la cadena que aun queda por consumir.
   * @param tope Simbolo en la cima de la pila.
   * @return Todas las transiciones aplicables, en el orden en que se
   *         deben probar.
   */
  std::vector<Transicion> candidatas(const std::string& estado,
                                     const std::string& entrada,
                                     char tope) const;

  /**
   * @brief Explora recursivamente las transiciones aplicables desde una
   *        configuracion concreta (estado, entrada restante y pila).
   *
   * @param estado Estado actual.
   * @param entrada Parte de la cadena que aun queda por consumir.
   * @param pila Contenido actual de la pila 
   * @param visitados Firmas de las configuraciones visitadas en el
   *        camino actual de la busqueda. 
   * @param camino IDs de las transiciones aplicadas en el camino actual. 
   * @return true si desde esta configuracion existe algun camino de
   *         aceptacion.
   */
  bool buscar(const std::string& estado,
             const std::string& entrada,
             Pila pila,
             std::vector<std::string>& visitados,
             std::vector<int>& camino) const;

  const Automata& automata;
  Traza* traza;
};

#endif  