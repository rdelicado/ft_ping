#!/bin/bash

# Script para comparar comportamiento de validación entre ft_ping y ping original
# Se enfoca solo en si acepta o rechaza argumentos, no en el texto exacto

GREEN='\033[0;32m'
RED='\033[0;31m'
BLUE='\033[0;34m'
YELLOW='\033[1;33m'
NC='\033[0m'

echo -e "${GREEN}=== Comparación de Validación ft_ping vs ping ===${NC}"
echo ""

TOTAL_TESTS=0
MATCHING_TESTS=0

compare_validation() {
    local desc="$1"
    local args="$2"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Prueba $TOTAL_TESTS: $desc${NC}"
    echo -e "${YELLOW}Args: $args${NC}"
    
    # ft_ping (capturar solo exit code y si muestra "destination:")
    ft_output=$(timeout 2s ./ft_ping $args 2>&1)
    ft_exit=$?
    ft_accepts=false
    if [ $ft_exit -eq 0 ] && echo "$ft_output" | grep -q "destination:"; then
        ft_accepts=true
    fi
    
    # ping original (capturar exit code)
    ping_output=$(timeout 2s ping -c 1 -W 1 $args 2>&1)
    ping_exit=$?
    ping_accepts=false
    # ping acepta si exit code es 0 o 1 (1 = timeout pero args válidos)
    if [ $ping_exit -eq 0 ] || [ $ping_exit -eq 1 ]; then
        ping_accepts=true
    fi
    
    # Mostrar resultados
    echo "ft_ping acepta: $ft_accepts (exit: $ft_exit)"
    echo "ping acepta:    $ping_accepts (exit: $ping_exit)"
    
    # Comparar
    if [ "$ft_accepts" = "$ping_accepts" ]; then
        echo -e "${GREEN}✓ MATCH${NC}"
        MATCHING_TESTS=$((MATCHING_TESTS + 1))
    else
        echo -e "${RED}✗ DIFF${NC}"
        echo "ft_ping output: $ft_output"
        echo "ping output: $ping_output"
    fi
    echo "---"
}

# Compilar
echo "Compilando..."
make
if [ $? -ne 0 ]; then
    echo "Error al compilar"
    exit 1
fi

echo -e "${YELLOW}Casos especiales de números:${NC}"
compare_validation "Solo 192" "192"
compare_validation "Solo 256" "256"
compare_validation "Solo 0" "0"
compare_validation "Solo 1" "1"
compare_validation "Decimal localhost" "2130706433"
compare_validation "Decimal Google" "134744072"
compare_validation "Decimal muy grande" "4294967295"
compare_validation "Decimal overflow" "4294967296"

echo -e "${YELLOW}IPs válidas:${NC}"
compare_validation "IP normal" "8.8.8.8"
compare_validation "IP localhost" "127.0.0.1"
compare_validation "IP broadcast" "255.255.255.255"
compare_validation "IP cero" "0.0.0.0"

echo -e "${YELLOW}Hostnames:${NC}"
compare_validation "Google" "google.com"
compare_validation "Localhost" "localhost"
compare_validation "Inexistente" "noexiste.invalid"

echo -e "${YELLOW}Casos inválidos:${NC}"
compare_validation "IP inválida" "999.999.999.999"
compare_validation "Solo letras" "abc"
compare_validation "Hexadecimal" "0x12345"
compare_validation "IP incompleta 3 partes" "192.168.1"
compare_validation "IP incompleta 2 partes" "192.168"
compare_validation "String vacío" "''"

echo ""
echo -e "${GREEN}=== Resultados Finales ===${NC}"
echo "Total pruebas: $TOTAL_TESTS"
echo "Coincidencias: $MATCHING_TESTS"
echo "Diferencias: $((TOTAL_TESTS - MATCHING_TESTS))"

PERCENTAGE=$((MATCHING_TESTS * 100 / TOTAL_TESTS))
echo "Compatibilidad: ${PERCENTAGE}%"

if [ $MATCHING_TESTS -eq $TOTAL_TESTS ]; then
    echo -e "${GREEN}¡Perfecto! Comportamiento 100% compatible${NC}"
elif [ $PERCENTAGE -ge 90 ]; then
    echo -e "${YELLOW}Muy buena compatibilidad (${PERCENTAGE}%)${NC}"
else
    echo -e "${RED}Necesita mejoras (${PERCENTAGE}%)${NC}"
fi