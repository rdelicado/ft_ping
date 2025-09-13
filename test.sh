#!/bin/bash

# Script de pruebas completas para ft_ping
# Prueba todos los casos posibles de argumentos, flags y validación de destinos

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Pruebas Completas de ft_ping ===${NC}"
echo ""

# Contadores
TOTAL_TESTS=0
PASSED_TESTS=0

# Función para ejecutar y mostrar resultado con PASS/FAIL
run_test() {
    local desc="$1"
    local cmd="$2"
    local expected="$3"
    local should_fail="$4" # opcional: si debe fallar (exit code != 0)

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Prueba $TOTAL_TESTS: $desc${NC}"
    echo -e "${YELLOW}Comando: $cmd${NC}"

    # Ejecutar comando y capturar salida
    output=$(eval "$cmd" 2>&1)
    exit_code=$?

    # Evaluar si pasa o falla
    local test_passed=false
    
    if [ "$should_fail" = "true" ]; then
        # Esperamos que falle (exit code != 0) Y contenga el mensaje esperado
        if [ $exit_code -ne 0 ] && [[ "$output" == *"$expected"* ]]; then
            test_passed=true
        fi
    else
        # Esperamos que funcione (exit code = 0) Y contenga el mensaje esperado
        if [ $exit_code -eq 0 ] && [[ "$output" == *"$expected"* ]]; then
            test_passed=true
        fi
    fi
    
    if [ "$test_passed" = true ]; then
        echo -e "${GREEN}✓ PASS${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo -e "${RED}Salida real: $output${NC}"
        echo -e "${RED}Exit code: $exit_code${NC}"
        echo -e "${RED}Esperado: $expected${NC}"
    fi
    echo "---"
}

# Función para mostrar estadísticas finales
show_stats() {
    echo -e "${CYAN}=== Estadísticas Finales ===${NC}"
    echo -e "Total de pruebas: $TOTAL_TESTS"
    echo -e "${GREEN}Pasadas: $PASSED_TESTS${NC}"
    echo -e "${RED}Falladas: $((TOTAL_TESTS - PASSED_TESTS))${NC}"
    
    if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
        echo -e "${GREEN}¡Todas las pruebas pasaron! 🎉${NC}"
    else
        echo -e "${RED}Algunas pruebas fallaron. Revisar código.${NC}"
    fi
}

# Compilar si no existe el ejecutable
if [ ! -f ft_ping ]; then
    echo "Compilando ft_ping..."
    make
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error al compilar. Abortando pruebas.${NC}"
        exit 1
    fi
fi

echo -e "${GREEN}Ejecutable encontrado. Iniciando pruebas...${NC}"
echo ""

# ============================================================================
# PRUEBAS DE ARGUMENTOS BÁSICOS
# ============================================================================
echo -e "${CYAN}=== Pruebas de Argumentos Básicos ===${NC}"

# 1. Sin argumentos
run_test "Sin argumentos" "./ft_ping" "Error: falta destino o opción"

# 2. Solo ayuda (-?)
run_test "Solo ayuda (-?)" "./ft_ping \"-?\"" "Usage:"

# 3. Solo verbose sin destino (debe fallar)
run_test "Solo verbose (-v)" "./ft_ping -v" "Error: falta destino"

# ============================================================================
# PRUEBAS DE FLAGS VÁLIDOS
# ============================================================================
echo -e "${CYAN}=== Pruebas de Flags Válidos ===${NC}"

# 4. Flag verbose con hostname válido
run_test "Verbose con hostname" "./ft_ping -v google.com" "Modo verbose activado"

# 5. Flag verbose con IP válida
run_test "Verbose con IP" "./ft_ping -v 8.8.8.8" "Modo verbose activado"

# 6. Flag verbose con localhost
run_test "Verbose con localhost" "./ft_ping -v localhost" "Modo verbose activado"

# ============================================================================
# PRUEBAS DE DESTINOS VÁLIDOS
# ============================================================================
echo -e "${CYAN}=== Pruebas de Destinos Válidos ===${NC}"

# 7. Solo hostname válido
run_test "Solo hostname (google.com)" "./ft_ping google.com" "destination: google.com"

# 8. Solo IP válida
run_test "Solo IP (8.8.8.8)" "./ft_ping 8.8.8.8" "destination: 8.8.8.8"

# 9. Solo localhost
run_test "Solo localhost" "./ft_ping localhost" "destination: localhost"

# 10. IP local (127.0.0.1)
run_test "IP local (127.0.0.1)" "./ft_ping 127.0.0.1" "destination: 127.0.0.1"

# ============================================================================
# PRUEBAS DE FLAGS INVÁLIDOS
# ============================================================================
echo -e "${CYAN}=== Pruebas de Flags Inválidos ===${NC}"

# 11. Flag inválido (-x)
run_test "Flag inválido (-x)" "./ft_ping -x google.com" "Error: flag inválido"

# 12. Flag inválido (-h)
run_test "Flag inválido (-h)" "./ft_ping -h google.com" "Error: flag inválido"

# 13. Flag inválido (--help)
run_test "Flag inválido (--help)" "./ft_ping --help google.com" "Error: flag inválido"

# ============================================================================
# PRUEBAS DE MÚLTIPLES ARGUMENTOS INVÁLIDOS
# ============================================================================
echo -e "${CYAN}=== Pruebas de Múltiples Argumentos ===${NC}"

# 14. Múltiples destinos
run_test "Múltiples destinos" "./ft_ping google.com yahoo.com" "Error: demasiados destinos"

# 15. Múltiples destinos con verbose
run_test "Múltiples destinos con -v" "./ft_ping -v google.com yahoo.com" "Error: demasiados destinos"

# 16. Tres destinos
run_test "Tres destinos" "./ft_ping google.com yahoo.com microsoft.com" "Error: demasiados destinos"

# ============================================================================
# PRUEBAS DE COMBINACIONES DE FLAGS
# ============================================================================
echo -e "${CYAN}=== Pruebas de Combinaciones de Flags ===${NC}"

# 17. -v y -? (debe mostrar ayuda e ignorar verbose)
run_test "Combinación -v -?" "./ft_ping -v \"-?\"" "Usage:"

# 18. -? con destino (debe ignorar destino y mostrar ayuda)
run_test "-? con destino" "./ft_ping \"-?\" google.com" "Usage:"

# 19. -? al final
run_test "-? al final con destino" "./ft_ping google.com \"-?\"" "Usage:"

# ============================================================================
# PRUEBAS DE DESTINOS INVÁLIDOS
# ============================================================================
echo -e "${CYAN}=== Pruebas de Destinos Inválidos ===${NC}"

# 20. Hostname completamente inválido
run_test "Hostname inválido" "./ft_ping thisdomaindoesnotexist12345.invalid" "No es una destino valido"

# 21. IP malformada
run_test "IP malformada (999.999.999.999)" "./ft_ping 999.999.999.999" "No es una destino valido"

# 22. IP incompleta
run_test "IP incompleta (192.168.1)" "./ft_ping 192.168.1" "No es una destino valido"

# 23. String vacío como destino (si es posible)
run_test "String vacío" "./ft_ping ''" "Error: falta destino"

# ============================================================================
# PRUEBAS DE CASOS EDGE
# ============================================================================
echo -e "${CYAN}=== Pruebas de Casos Edge ===${NC}"

# 24. Solo espacios como destino
run_test "Solo espacios" "./ft_ping ' '" "No es una destino valido"

# 25. Caracteres especiales
run_test "Caracteres especiales" "./ft_ping '@#\$%'" "No es una destino valido"

# 26. Números como hostname (pero no IP válida)
run_test "Números inválidos como IP" "./ft_ping 123456" "No es una destino valido"

# ============================================================================
# RESUMEN Y ESTADÍSTICAS
# ============================================================================
echo ""
show_stats