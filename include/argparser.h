#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <string>

/**
 * @file ArgParser.h
 * @brief Declaracion de ArgParser y del struct Opciones.
 *
 * Recoge y valida las opciones de linea de comandos del programa:
 *
 *   -config <f>       Fichero de texto con la configuracion del AP (obligatoria).
 *   -trace <y|n>       Si se muestra o no la traza durante la ejecucion (obligatoria).
 *   [-in <f>]          Fichero con las cadenas de entrada. Opcional (por defecto, teclado).
 *   [-out <f>]         Fichero donde escribir la traza. Opcional (por defecto, pantalla).
 */

/**
 * @struct Opciones
 * @brief Opciones de linea de comandos ya interpretadas.
 *
 * Es un simple contenedor de datos, sin logica propia, por lo que sus
 * campos son publicos y no llevan sufijo "_".
 */
struct Opciones {
  std::string ficheroConfig;   ///< Ruta del fichero de configuracion del automata.
  bool trace;                  ///< true si se debe mostrar la traza durante la ejecucion.
  std::string ficheroEntrada;  ///< Ruta del fichero de cadenas de entrada; vacio = teclado.
  std::string ficheroSalida;   ///< Ruta del fichero de traza; vacio = pantalla.
};

/**
 * @class ArgParser
 * @brief Analizador de las opciones de linea de comandos.
 *
 * No mantiene estado propio: su unico metodo publico es estatico.
 */
class ArgParser {
public:
  /**
   * @brief Analiza los argumentos de linea de comandos.
   *
   * @param argc Numero de argumentos, tal y como los recibe main().
   * @param argv Vector de argumentos, tal y como los recibe main().
   * @return Las opciones ya interpretadas.
   * @throws ErrorArgumentos Si falta alguna opcion obligatoria, si una
   *         opcion no reconocida aparece, si a una opcion le falta su
   *         valor, o si el valor de -trace no es 'y' ni 'n'.
   */
  static Opciones parsear(int argc, char* argv[]);

private:
  /**
   * @brief Obtiene el valor asociado a una opcion (el argumento
   *        siguiente) y avanza el indice hasta el.
   *
   * @param argc Numero de argumentos.
   * @param argv Vector de argumentos.
   * @param indice Indice de la opcion; se incrementa en 1 para que pase
   *        a apuntar al valor leido.
   * @param opcion Nombre de la opcion, para el mensaje de error.
   * @return El valor asociado a la opcion.
   * @throws ErrorArgumentos Si no queda ningun argumento despues de la
   *         opcion.
   */
  static std::string obtenerValor(int argc, char* argv[], int& indice, const std::string& opcion);

  /**
   * @brief Interpreta el valor de -trace como booleano.
   * @param valor Valor recibido tras -trace.
   * @return true si valor es "y"/"Y", false si es "n"/"N".
   * @throws ErrorArgumentos Si valor no es ninguno de los anteriores.
   */
  static bool aBooleano(const std::string& valor);
};

#endif  // ARG_PARSER_H