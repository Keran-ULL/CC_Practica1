/**
 * @author Keran Miranda González
 * @file main.cpp
 * @brief Punto de entrada del simulador de un automata con pila (AP).
 *
 * Orquesta el flujo completo del programa: lee las opciones de linea de
 * comandos, construye el automata a partir del fichero de configuracion,
 * prepara el simulador (con su traza opcional), y por ultimo procesa
 * cada cadena de entrada indicando si pertenece o no al lenguaje
 * reconocido por el automata.
 *
 * Uso:
 *   pda_simulator -config <f> -trace <y|n> [-in <f>] [-out <f>]
 *
 * Asignatura: Complejidad Computacional. Curso 2026/27.
 */

#include <iostream>
#include <memory>
#include <string>

#include "argParser.h"
#include "automata.h"
#include "errores.h"
#include "lectorAutomata.h"
#include "lectorCadenas.h"
#include "simulador.h"
#include "traza.h"

/**
 * @brief Ejecuta el simulador sobre una unica cadena de entrada y
 *        muestra el resultado por pantalla.
 *
 * @param simulador Simulador ya inicializado con el automata a utilizar.
 * @param cadena Cadena de entrada a comprobar (puede ser vacia, es
 *        decir, epsilon).
 */
void procesarCadena(const Simulador& simulador, const std::string& cadena) {
  const bool aceptada = simulador.acepta(cadena);
  const std::string etiquetaCadena = cadena.empty() ? "." : cadena;

  std::cout << "\"" << etiquetaCadena << "\" -> "
            << (aceptada ? "ACEPTADA" : "NO ACEPTADA") << '\n';
}

/**
 * @brief Funcion principal del programa.
 * Se limita a coordinar las distintas clases del proyecto. 
 * @param argc Numero de argumentos recibidos por linea de comandos.
 * @param argv Vector de argumentos recibidos por linea de comandos.
 * @return 0 si el programa termina correctamente, 1 si se produce algun
 *         error controlado, 2 ante un error no anticipado.
 */
int main(int argc, char* argv[]) {
  try {
    const Opciones opciones = ArgParser::parsear(argc, argv);
    Automata automata = LectorAutomata::leer(opciones.ficheroConfig);
    std::unique_ptr<Traza> traza;
    if (opciones.trace) {
      traza = opciones.ficheroSalida.empty()
                  ? std::make_unique<Traza>()
                  : std::make_unique<Traza>(opciones.ficheroSalida);
      traza->listarTransiciones(automata.getTransiciones().todas());
    }
    Simulador simulador(automata, traza.get());
    const std::unique_ptr<LectorCadenas> lector =
        opciones.ficheroEntrada.empty()
            ? std::make_unique<LectorCadenas>()
            : std::make_unique<LectorCadenas>(opciones.ficheroEntrada);

    std::string cadena;
    while (lector->siguienteCadena(cadena)) {
      procesarCadena(simulador, cadena);
    }
  } 
  catch (const ErrorAutomata& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return 1;
  } catch (const std::exception& error) {
    std::cerr << "Error inesperado: " << error.what() << '\n';
    return 2;
  }

  return 0;
}