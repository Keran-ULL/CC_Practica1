# Simulador de un Autómata con Pila (AP)

**Asignatura:** Complejidad Computacional — Curso 2026/27
**Práctica 1:** Programar un simulador de un autómata con pila
**Repositorio:** [github.com/Keran-ULL/CC_Practica1](https://github.com/Keran-ULL/CC_Practica1)

## Objetivo

Programa que simula un autómata con pila (AP) a partir de una definición
leída de un fichero de texto, y que permite comprobar si una o varias
cadenas de entrada pertenecen al lenguaje reconocido por dicho autómata.

## Tipo de autómata implementado

Se ha implementado el autómata con pila **por estado final (APf)**. La
aceptación de una cadena se determina así: existe algún camino de
cómputo que, tras consumir toda la cadena de entrada, termina en un
estado perteneciente al conjunto de estados finales `F` — el contenido
de la pila en ese momento no influye en la decisión.

El diseño deja preparado el punto de extensión para implementar también
el autómata por vaciado de pila (APv) en el futuro sin tener que tocar
el motor de búsqueda (`Simulador`): bastaría con cambiar la condición de
aceptación de `entrada.empty() && esFinal(estado)` a
`entrada.empty() && pila.vacia()`.

## Compilación

Requisitos: `g++` con soporte de C++17.

```bash
make          # compila el proyecto; genera el ejecutable en build/
make clean    # borra los ficheros generados (build/)
make run      # compila (si hace falta) y ejecuta el programa sin argumentos
```

## Uso

```
build/main -config <f> -trace <y|n> [-in <f>] [-out <f>]
```

| Opción      | Obligatoria | Descripción                                                                 |
|-------------|:-----------:|------------------------------------------------------------------------------|
| `-config f` | Sí          | Fichero de texto con la definición del autómata.                            |
| `-trace y\|n` | Sí        | Si se muestra (`y`) o no (`n`) la traza de ejecución.                       |
| `-in f`     | No          | Fichero con las cadenas a comprobar (una por línea). Si se omite, se leen por teclado, mostrando el mensaje "Introduzca una cadena". |
| `-out f`    | No          | Fichero donde escribir la traza. Si se omite, se muestra por pantalla.      |


## Formato del fichero de configuración

```
# Los comentarios empiezan por '#' y se ignoran, en línea propia o al
# final de una línea con contenido.

q1 q2 q3 ...     # conjunto de estados, Q
a1 a2 a3 ...     # alfabeto de entrada, Sigma (un caracter por símbolo)
A1 A2 A3 ...     # alfabeto de pila, Gamma (un caracter por símbolo)
q1               # estado inicial
A1               # símbolo inicial de la pila
q2 q3            # conjunto de estados finales, F
q1 a A1 q2 A     # una transición por línea: (q2, A) pertenece a delta(q1, a, A1)
...
```

- El símbolo epsilon (ε) se representa con un punto (`.`): tanto como
  símbolo de entrada de una transición, como cadena vacía a apilar.
- El símbolo de pila que se consume en cada transición (`A1` en el
  ejemplo) siempre debe ser un único carácter de `Gamma`; nunca puede
  ser epsilon.
- La cadena a apilar (`A` al final de la línea de transición) puede ser
  epsilon (`.`) o varios símbolos de `Gamma` concatenados sin espacios
  (p. ej. `A1A1A1`); el primer símbolo de esa cadena queda como nueva
  cima de la pila.

## Estructura del proyecto

```
.
├── Makefile
├── README.md
├── build/              # objetos y ejecutable generados por make (no versionar)
├── include/             # cabeceras (.h) de todas las clases
├── src/                 # implementación (.cc) de todas las clases, incluido main
└── tests/
    └── test.sh          # batería de pruebas automáticas (ver más abajo)
```

## Diseño: resumen de clases

| Clase              | Responsabilidad                                                                 |
|---------------------|-----------------------------------------------------------------------------------|
| `ArgParser`         | Interpreta las opciones de línea de comandos (`argc`/`argv`) y las valida.       |
| `LectorAutomata`    | Lee y parsea el fichero de configuración, construyendo un `Automata`.            |
| `Automata`          | Definición formal del AP (Q, Sigma, Gamma, q0, Z0, F, transiciones); se autovalida en el constructor. |
| `Alfabeto`          | Conjunto de símbolos de un único carácter, con comprobación de pertenencia. Se usa para Sigma y Gamma. |
| `Transicion`        | Una única regla de la función de transición, identificada por un ID (orden en el fichero). |
| `Transiciones`      | Colección de `Transicion`, con búsqueda de las aplicables desde una configuración dada. |
| `Pila`              | Pila de símbolos con operaciones `push`/`pop`/`cima`, implementación propia (sin `std::stack`). |
| `Simulador`         | Motor de búsqueda en profundidad (DFS) con retroceso que decide si una cadena se acepta. |
| `Traza`             | Muestra la ejecución paso a paso en formato tabla, a pantalla o a fichero.        |
| `LectorCadenas`     | Proporciona las cadenas a comprobar, por teclado o por fichero.                  |
| `Errores`           | Jerarquía de excepciones (`ErrorArgumentos`, `ErrorFichero`, `ErrorFormato`, `ErrorValidacion`), todas derivadas de `ErrorAutomata`. |

Todos los errores controlados (argumentos inválidos, ficheros
inaccesibles, formato incorrecto, autómata que no cumple su definición
formal) se capturan de forma centralizada en `main`, que los muestra por
`stderr` y termina con un código de salida distinto de 0.

## Algoritmo de simulación

Como el autómata puede ser no determinista (varias transiciones
aplicables desde una misma configuración, incluidas transiciones-ε), el
`Simulador` explora todas las alternativas con backtracking (DFS): desde
cada configuración `(estado, entrada restante, pila)` prueba, en orden,
las transiciones que consumen el siguiente símbolo de entrada y las
transiciones-ε, hasta encontrar un camino que termine en un estado final
con la entrada agotada, o hasta agotar todas las alternativas.

Para evitar quedarse colgado en un bucle de transiciones-ε que no
consumen entrada ni progresan, se lleva un registro de las
configuraciones ya visitadas en el camino actual de la búsqueda: si una
configuración se repite, esa rama se descarta (su futuro está
completamente determinado por la propia configuración, así que no puede
aportar nada nuevo).

## Traza de ejecución

Con `-trace y`, antes de comprobar ninguna cadena se muestra el listado
completo de transiciones (con su ID), y por cada cadena se abre una
tabla con su propia numeración de iteración:

```
===== Transiciones del automata =====
  T1: (q0, a, Z) -> (q0, AZ)
  T2: (q0, a, A) -> (q0, AA)
  T3: (q0, b, A) -> (q1, .)
  T4: (q1, b, A) -> (q1, .)
  T5: (q1, ., Z) -> (qf, Z)
  T6: (q0, ., Z) -> (qf, Z)

===== Comprobando cadena: "ab" =====
Iter  Estado    Cadena rest.      Pila (cima-base)  Posibles              Elegida
------------------------------------------------------------------------------------
1     q0        ab                Z                 T1,T6                 T1
2     q0        b                 AZ                T3                    T3
3     q1        .                 Z                 T5                    T5
Resultado: ACEPTADA. Camino: T1,T3,T5
```

Cada fila es un intento de aplicar una transición (incluidos los que
acaban en un callejón sin salida y se descartan al retroceder), con la
columna "Posibles" mostrando todos los IDs aplicables en ese momento y
"Elegida" cuál de ellos se prueba. Al final de la tabla de cada cadena
se indica si se acepta y, en caso afirmativo, el camino completo de IDs
que lo demuestra.

## Tests automáticos

`tests/test.sh` compila el proyecto y ejecuta una batería de
comprobaciones:

- **Manejo de errores**: falta de opciones obligatorias, valores
  inválidos, ficheros inexistentes, ficheros de configuración con
  formato incorrecto o que no cumplen la definición formal del
  autómata (estado inicial fuera de Q, transición con símbolo no
  declarado...).
- **Comportamiento funcional**: usando un autómata a·ⁿ·b·ⁿ de ejemplo,
  comprueba que se aceptan/rechazan correctamente varias cadenas.
- **Modo traza**: comprueba que la salida con `-trace y` incluye el
  listado de transiciones y el resultado final.
- **Lectura por fichero**: comprueba que se procesan todas las cadenas
  de un fichero pasado con `-in`.

Para ejecutarlo:

```bash
chmod +x tests/test.sh   # solo la primera vez
./tests/test.sh
```

Al final muestra un resumen de tests pasados/fallados, y termina con
código de salida 0 solo si todos han pasado.

## Limitaciones conocidas

- La poda de bucles en transiciones-ε compara la configuración completa
  (estado + entrada restante + contenido de la pila) mediante búsqueda
  lineal en un `std::vector`; es más que suficiente para los tamaños de
  autómata y de cadena de esta práctica, pero no está pensado para
  entradas muy largas.
- Solo se ha implementado el autómata por estado final (APf), tal y
  como se indicó como decisión de diseño; el código está preparado para
  añadir el autómata por vaciado de pila (APv) más adelante.