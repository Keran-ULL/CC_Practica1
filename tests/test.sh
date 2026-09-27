#!/usr/bin/env bash
#
# test.sh - Bateria de pruebas automaticas para el simulador de AP.
#
# Compila el proyecto y comprueba: manejo de errores (argumentos,
# ficheros, formato y validacion del automata), comportamiento
# funcional con un automata de ejemplo, el modo traza, y la lectura de
# cadenas por fichero.
#
# Uso: ./tests/test.sh   

set -uo pipefail

# --- Localizar la raiz del proyecto y el ejecutable ------------------
PROYECTO_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$PROYECTO_DIR"

EJECUTABLE="build/main"

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

PASADOS=0
FALLADOS=0

# --- Funciones auxiliares --------------------------------------------

# Anota el resultado de un test y lo muestra por pantalla.
# $1: nombre del test.  $2: 0 si ha pasado, cualquier otro valor si ha fallado.
reportar() {
  local nombre="$1"
  local resultado="$2"
  if [ "$resultado" -eq 0 ]; then
    echo "  [OK]   $nombre"
    PASADOS=$((PASADOS + 1))
  else
    echo "  [FAIL] $nombre"
    FALLADOS=$((FALLADOS + 1))
  fi
}

# Ejecuta el simulador con los argumentos dados y comprueba que TERMINA
# CON ERROR (codigo de salida distinto de 0). Pensado para los tests de
# manejo de errores.
# $1: nombre del test. Resto: argumentos para el ejecutable.
esperar_error() {
  local nombre="$1"
  shift
  "$EJECUTABLE" "$@" >/dev/null 2>&1
  local codigo=$?
  if [ "$codigo" -ne 0 ]; then
    reportar "$nombre" 0
  else
    reportar "$nombre" 1
  fi
}

# Comprueba si el simulador acepta o rechaza una cadena concreta con el
# automata de ejemplo a^n b^n.
# $1: nombre del test. $2: cadena a comprobar. $3: "ACEPTADA" o "NO ACEPTADA".
comprobar_cadena() {
  local nombre="$1"
  local cadena="$2"
  local esperado="$3"
  local salida
  salida=$(printf '%s\n' "$cadena" | "$EJECUTABLE" -config "$TMP_DIR/anbn.txt" -trace n)
  if echo "$salida" | grep -q -- "$esperado"; then
    reportar "$nombre" 0
  else
    reportar "$nombre" 1
    echo "         esperado: $esperado / salida obtenida: $salida"
  fi
}

# --- Compilacion -------------------------------------------------------

echo "=== Compilando el proyecto ==="
if ! make >/tmp/test_make.log 2>&1; then
  echo "La compilacion ha fallado. Salida de 'make':"
  cat /tmp/test_make.log
  exit 1
fi
if [ ! -x "$EJECUTABLE" ]; then
  echo "No se encuentra el ejecutable en '$EJECUTABLE'."
  echo "Ajusta la variable EJECUTABLE al principio de este script."
  exit 1
fi
echo "OK: $EJECUTABLE"
echo

# --- Ficheros de configuracion usados en los tests ---------------------

# Automata valido: a^n b^n (n >= 0), por estado final.
# Solo se acepta si el numero de 'b' iguala exactamente al numero de 'a'
# (el estado qf solo es alcanzable cuando la pila vuelve al simbolo base).
cat > "$TMP_DIR/anbn.txt" << 'CFG'
# a^n b^n, n >= 0
q0 q1 qf
a b
A Z
q0
Z
qf
q0 a Z q0 AZ
q0 a A q0 AA
q0 b A q1 .
q1 b A q1 .
q1 . Z qf Z
q0 . Z qf Z
CFG

# Config con menos lineas de las necesarias (falta Gamma, q0, Z0, F...).
cat > "$TMP_DIR/pocas_lineas.txt" << 'CFG'
q0 q1
a b
CFG

# Estado inicial que no pertenece a Q.
cat > "$TMP_DIR/estado_inicial_invalido.txt" << 'CFG'
q0 q1
a b
A Z
q5
Z
q1
q0 a Z q0 AZ
CFG

# Transicion que usa un simbolo de entrada no declarado en Sigma.
cat > "$TMP_DIR/simbolo_no_declarado.txt" << 'CFG'
q0 q1
a b
A Z
q0
Z
q1
q0 c Z q0 AZ
CFG

# --- 1. Manejo de errores ----------------------------------------------

echo "=== 1. Manejo de errores ==="
esperar_error "Falta la opcion obligatoria -config"      -trace n
esperar_error "Falta la opcion obligatoria -trace"        -config "$TMP_DIR/anbn.txt"
esperar_error "Valor invalido para -trace"                -config "$TMP_DIR/anbn.txt" -trace x
esperar_error "Opcion no reconocida"                      -config "$TMP_DIR/anbn.txt" -trace n -foo bar
esperar_error "Fichero de configuracion inexistente"      -config "$TMP_DIR/no_existe.txt" -trace n
esperar_error "Config con lineas insuficientes"           -config "$TMP_DIR/pocas_lineas.txt" -trace n
esperar_error "Estado inicial que no pertenece a Q"       -config "$TMP_DIR/estado_inicial_invalido.txt" -trace n
esperar_error "Transicion con simbolo no declarado"       -config "$TMP_DIR/simbolo_no_declarado.txt" -trace n
echo

# --- 2. Comportamiento funcional (automata a^n b^n) ---------------------

echo "=== 2. Comportamiento funcional (automata a^n b^n) ==="
comprobar_cadena "Cadena vacia aceptada"   ""      "ACEPTADA"
comprobar_cadena "'ab' aceptada"           "ab"    "ACEPTADA"
comprobar_cadena "'aabb' aceptada"         "aabb"  "ACEPTADA"
comprobar_cadena "'aab' rechazada"         "aab"   "NO ACEPTADA"
comprobar_cadena "'ba' rechazada"          "ba"    "NO ACEPTADA"
comprobar_cadena "'abb' rechazada"         "abb"   "NO ACEPTADA"
echo

# --- 3. Modo traza -------------------------------------------------------

echo "=== 3. Modo traza ==="
salida_traza=$(printf 'ab\n' | "$EJECUTABLE" -config "$TMP_DIR/anbn.txt" -trace y)
if echo "$salida_traza" | grep -q "Transiciones del automata"; then
  reportar "La traza muestra el listado de transiciones" 0
else
  reportar "La traza muestra el listado de transiciones" 1
fi
if echo "$salida_traza" | grep -q "Resultado: ACEPTADA"; then
  reportar "La traza muestra el resultado final" 0
else
  reportar "La traza muestra el resultado final" 1
fi
echo

# --- 4. Lectura de cadenas por fichero (-in) ------------------------------

echo "=== 4. Lectura de cadenas por fichero (-in) ==="
printf 'ab\naabb\naab\n' > "$TMP_DIR/cadenas.txt"
salida_fichero=$("$EJECUTABLE" -config "$TMP_DIR/anbn.txt" -trace n -in "$TMP_DIR/cadenas.txt")
num_lineas=$(printf '%s\n' "$salida_fichero" | grep -c -- '->')
if [ "$num_lineas" -eq 3 ]; then
  reportar "Se procesan las 3 cadenas del fichero -in" 0
else
  reportar "Se procesan las 3 cadenas del fichero -in" 1
fi
echo

# --- Resumen ---------------------------------------------------------------

echo "=== Resumen ==="
echo "Pasados : $PASADOS"
echo "Fallados: $FALLADOS"

[ "$FALLADOS" -eq 0 ]