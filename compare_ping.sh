#!/bin/bash

# Script de comparación entre ft_ping y ping original
# Ejecuta las mismas pruebas en ambos programas y compara resultados

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Comparación ft_ping vs ping original ===${NC}"
echo ""

# Contadores
TOTAL_TESTS=0
MATCHING_TESTS=0
DIFFERENT_BEHAVIOR=0

# Función para comparar comportamiento
compare_ping() {
    local desc="$1"
    local args="$2"
    local expect_success="$3"  # true/false - si esperamos que funcione

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Prueba $TOTAL_TESTS: $desc${NC}"
    echo -e "${YELLOW}Argumentos: $args${NC}"

    # Ejecutar ft_ping (timeout de 2 segundos para evitar colgarse)
    ft_output=$(timeout 2s ./ft_ping $args 2>&1)
    ft_exit=$?
    
    # Ejecutar ping original (timeout de 2 segundos, -W 1 = timeout 1 segundo por paquete)
    ping_output=$(timeout 2s ping -c 1 -W 1 $args 2>&1)
    ping_exit=$?
    
    # Analizar resultados
    local match=false
    
    if [ "$expect_success" = "true" ]; then
        # Esperamos que ambos funcionen
        if [ $ft_exit -eq 0 ] && [ $ping_exit -eq 0 ]; then
            match=true
        elif [ $ft_exit -ne 0 ] && [ $ping_exit -ne 0 ]; then
            match=true  # Ambos fallan, es correcto
        fi
    else
        # Esperamos que ambos fallen
        if [ $ft_exit -ne 0 ] && [ $ping_exit -ne 0 ]; then
            match=true
        elif [ $ft_exit -eq 0 ] && [ $ping_exit -eq 0 ]; then
            match=true  # Ambos funcionan, inesperado pero consistente
        fi
    fi
    
    if [ "$match" = true ]; then
        echo -e "${GREEN}✓ MATCH${NC} - Comportamiento idéntico"
        MATCHING_TESTS=$((MATCHING_TESTS + 1))
    else
        echo -e "${RED}✗ DIFF${NC} - Comportamiento diferente"
        echo -e "${RED}ft_ping (exit $ft_exit): ${ft_output:0:100}${NC}"
        echo -e "${RED}ping    (exit $ping_exit): ${ping_output:0:100}${NC}"
        DIFFERENT_BEHAVIOR=$((DIFFERENT_BEHAVIOR + 1))
    fi
    echo "---"
}

# Compilar si es necesario
if [ ! -f ft_ping ]; then
    echo "Compilando ft_ping..."
    make
    if [ $? -ne 0 ]; then
        echo -e "${RED}Error al compilar. Abortando comparación.${NC}"
        exit 1
    fi
fi

echo -e "${GREEN}Iniciando comparación...${NC}"
echo ""

# ============================================================================
# PRUEBAS DE CASOS VÁLIDOS
# ============================================================================
echo -e "${CYAN}=== Casos que deben funcionar ===${NC}"

compare_ping "IP válida básica" "8.8.8.8" "true"
compare_ping "IP localhost" "127.0.0.1" "true"
compare_ping "IP broadcast" "255.255.255.255" "true"
compare_ping "IP cero" "0.0.0.0" "true"
compare_ping "Hostname google" "google.com" "true"
compare_ping "Hostname localhost" "localhost" "true"

# IPs decimales (si ping las soporta)
compare_ping "IP decimal localhost" "2130706433" "true"
compare_ping "IP decimal Google DNS" "134744072" "true"
compare_ping "IP decimal cero" "0" "true"

echo -e "${CYAN}=== Casos que deben fallar ===${NC}"

# ============================================================================
# PRUEBAS DE CASOS INVÁLIDOS
# ============================================================================

# IPs malformadas
compare_ping "IP fuera de rango" "999.999.999.999" "false"
compare_ping "IP incompleta 1 segmento" "192" "false"
compare_ping "IP incompleta 2 segmentos" "192.168" "false"
compare_ping "IP incompleta 3 segmentos" "192.168.1" "false"
compare_ping "IP con 5 segmentos" "1.2.3.4.5" "false"
compare_ping "IP con punto al inicio" ".1.2.3.4" "false"
compare_ping "IP con punto al final" "1.2.3.4." "false"
compare_ping "IP con doble punto" "1..2.3.4" "false"

# Hostnames inválidos  
compare_ping "Hostname inexistente" "thisdomaindoesnotexist12345.invalid" "false"
compare_ping "String vacío" "''" "false"
compare_ping "Solo espacios" "' '" "false"
compare_ping "Caracteres especiales" "'@#\$%'" "false"

# Casos edge
compare_ping "Hexadecimal" "0x12345678" "false"
compare_ping "Números con letras" "123abc" "false"
compare_ping "Notación científica" "1e6" "false"

echo -e "${CYAN}=== Casos de overflow ===${NC}"

# Overflow decimal
compare_ping "Decimal overflow" "4294967296" "false"
compare_ping "Decimal muy grande" "99999999999" "false"

# ============================================================================
# ESTADÍSTICAS FINALES
# ============================================================================
echo ""
echo -e "${CYAN}=== Estadísticas de Comparación ===${NC}"
echo -e "Total de pruebas: $TOTAL_TESTS"
echo -e "${GREEN}Comportamiento idéntico: $MATCHING_TESTS${NC}"
echo -e "${RED}Comportamiento diferente: $DIFFERENT_BEHAVIOR${NC}"

MATCH_PERCENTAGE=$((MATCHING_TESTS * 100 / TOTAL_TESTS))
echo -e "Compatibilidad: ${MATCH_PERCENTAGE}%"

if [ $DIFFERENT_BEHAVIOR -eq 0 ]; then
    echo -e "${GREEN}¡Perfect! Tu ft_ping se comporta igual que ping original! 🎉${NC}"
elif [ $MATCH_PERCENTAGE -ge 90 ]; then
    echo -e "${YELLOW}Muy bien! Compatibilidad alta (${MATCH_PERCENTAGE}%). Revisar casos diferentes.${NC}"
else
    echo -e "${RED}Hay diferencias significativas. Revisar implementación.${NC}"
fi