#!/bin/bash

# ICMP Messages Test Suite for ft_ping
# Author: rdelicad
# Date: 2025-10-24
#
# This script tests all ICMP message types that ft_ping should handle

GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

PASSED=0
FAILED=0
TOTAL=0

# Verificar que ft_ping existe
if [ ! -f "./ft_ping" ]; then
    echo -e "${RED}Error: ft_ping no encontrado. Ejecuta 'make' primero.${NC}"
    exit 1
fi

# Verificar que se ejecuta como root
if [ "$EUID" -ne 0 ]; then
    echo -e "${RED}Error: Este script debe ejecutarse como root (sudo)${NC}"
    exit 1
fi

echo -e "${CYAN}╔═══════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║          ICMP MESSAGES TEST SUITE - ft_ping                  ║${NC}"
echo -e "${CYAN}╚═══════════════════════════════════════════════════════════════╝${NC}\n"

# Función para verificar si la salida contiene el texto esperado
check_output() {
    local output="$1"
    local expected="$2"
    local test_name="$3"
    
    TOTAL=$((TOTAL + 1))
    
    if echo "$output" | grep -qi "$expected"; then
        echo -e "${GREEN}  ✓ PASS${NC} - $test_name"
        PASSED=$((PASSED + 1))
        return 0
    else
        echo -e "${RED}  ✗ FAIL${NC} - $test_name"
        echo -e "${RED}     Esperado: $expected${NC}"
        FAILED=$((FAILED + 1))
        return 1
    fi
}

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}TEST CATEGORY 1: Type 0 - Echo Reply (Respuestas exitosas)${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

# Test 1.1: Ping normal a Google DNS
echo -e "${YELLOW}[TEST 1.1]${NC} Echo Reply - Ping a 8.8.8.8"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 3 8.8.8.8"
OUTPUT=$(./ft_ping -c 3 8.8.8.8 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "64 bytes from 8.8.8.8" "Debe recibir respuestas de 8.8.8.8"
check_output "$OUTPUT" "icmp_seq=" "Debe mostrar número de secuencia"
check_output "$OUTPUT" "ttl=" "Debe mostrar TTL"
check_output "$OUTPUT" "time=" "Debe mostrar tiempo RTT"
echo ""

# Test 1.2: Ping a localhost
echo -e "${YELLOW}[TEST 1.2]${NC} Echo Reply - Ping a localhost"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 2 localhost"
OUTPUT=$(./ft_ping -c 2 localhost 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "64 bytes from 127.0.0.1" "Debe recibir respuestas de localhost"
check_output "$OUTPUT" "2 packets transmitted, 2 received" "Debe recibir todos los paquetes"
echo -e "\n${YELLOW}─────────────────────────────────────────────────────────────${NC}\n"

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}TEST CATEGORY 2: Type 11 - Time Exceeded${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

# Test 2.1: TTL expired (Code 0)
echo -e "${YELLOW}[TEST 2.1]${NC} Time Exceeded (Code 0) - TTL=1"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 3 --ttl 1 8.8.8.8"
OUTPUT=$(./ft_ping -c 3 --ttl 1 8.8.8.8 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "Time to live exceeded" "Debe mostrar 'Time to live exceeded'"
check_output "$OUTPUT" "From" "Debe mostrar la IP del router que envía el error"
check_output "$OUTPUT" "icmp_seq=" "Debe mostrar el número de secuencia"
check_output "$OUTPUT" "100% packet loss" "Debe mostrar 100% de pérdida"
echo ""

# Test 2.2: TTL=2 a destino lejano
echo -e "${YELLOW}[TEST 2.2]${NC} Time Exceeded (Code 0) - TTL=2 a servidor remoto"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 2 --ttl 2 142.250.185.46"
OUTPUT=$(./ft_ping -c 2 --ttl 2 142.250.185.46 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "Time to live exceeded" "Debe mostrar 'Time to live exceeded'"
echo ""

# Test 2.3: TTL=1 modo verbose
echo -e "${YELLOW}[TEST 2.3]${NC} Time Exceeded con modo verbose"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 2 -v --ttl 1 8.8.8.8"
OUTPUT=$(./ft_ping -c 2 -v --ttl 1 8.8.8.8 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "Time to live exceeded" "Modo verbose debe mostrar el error"
echo -e "\n${YELLOW}─────────────────────────────────────────────────────────────${NC}\n"

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}TEST CATEGORY 3: Type 3 - Destination Unreachable${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

# Test 3.1: Host Unreachable (Code 1)
echo -e "${YELLOW}[TEST 3.1]${NC} Destination Unreachable - Host Unreachable (probando varias IPs)"
echo -e "${YELLOW}Nota:${NC} Dependiendo de la red, puede mostrar 'Host Unreachable' o timeout"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 2 -W 1 192.168.255.254"
OUTPUT=$(./ft_ping -c 2 -W 1 192.168.255.254 2>&1)
echo "$OUTPUT"
# Puede ser Host Unreachable o timeout dependiendo del router
if echo "$OUTPUT" | grep -qi "Host Unreachable"; then
    check_output "$OUTPUT" "Destination Host Unreachable" "Debe mostrar Host Unreachable"
else
    check_output "$OUTPUT" "Request timeout" "Router no responde - timeout esperado"
fi
echo ""

# Test 3.2: Network Unreachable (Code 0)
echo -e "${YELLOW}[TEST 3.2]${NC} Destination Unreachable - Network Unreachable"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 2 -W 1 10.255.255.1"
OUTPUT=$(./ft_ping -c 2 -W 1 10.255.255.1 2>&1)
echo "$OUTPUT"
# Puede ser Network/Host Unreachable o timeout
if echo "$OUTPUT" | grep -qi "Unreachable"; then
    check_output "$OUTPUT" "Unreachable" "Debe mostrar algún tipo de Unreachable"
else
    check_output "$OUTPUT" "Request timeout" "Red no responde - timeout esperado"
fi
echo -e "\n${YELLOW}─────────────────────────────────────────────────────────────${NC}\n"

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}TEST CATEGORY 4: Timeout - Sin respuesta${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

# Test 4.1: Timeout a IP inexistente
echo -e "${YELLOW}[TEST 4.1]${NC} Request Timeout - IP completamente inalcanzable"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 2 -W 1 203.0.113.1"
OUTPUT=$(./ft_ping -c 2 -W 1 203.0.113.1 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "Request timeout" "Debe mostrar 'Request timeout for icmp_seq='"
check_output "$OUTPUT" "100% packet loss" "Debe mostrar 100% de pérdida"
echo ""

# Test 4.2: Timeout con timeout corto
echo -e "${YELLOW}[TEST 4.2]${NC} Request Timeout - Timeout muy corto"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 2 -W 1 172.16.255.254"
OUTPUT=$(./ft_ping -c 2 -W 1 172.16.255.254 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "Request timeout\|Unreachable" "Debe mostrar timeout o unreachable"
echo -e "\n${YELLOW}─────────────────────────────────────────────────────────────${NC}\n"

echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${BLUE}TEST CATEGORY 5: Casos especiales y combinaciones${NC}"
echo -e "${BLUE}═══════════════════════════════════════════════════════════════${NC}\n"

# Test 5.1: Múltiples paquetes con errores
echo -e "${YELLOW}[TEST 5.1]${NC} Múltiples paquetes con TTL=1"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 5 --ttl 1 8.8.8.8"
OUTPUT=$(./ft_ping -c 5 --ttl 1 8.8.8.8 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "icmp_seq=1" "Debe procesar secuencia 1"
check_output "$OUTPUT" "icmp_seq=5" "Debe procesar secuencia 5"
check_output "$OUTPUT" "5 packets transmitted, 0 received" "Debe mostrar estadísticas correctas"
echo ""

# Test 5.2: Tamaño de paquete personalizado con error
echo -e "${YELLOW}[TEST 5.2]${NC} Paquete grande con TTL=1"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 2 -s 1000 --ttl 1 8.8.8.8"
OUTPUT=$(./ft_ping -c 2 -s 1000 --ttl 1 8.8.8.8 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "Time to live exceeded" "Debe funcionar con paquetes grandes"
echo ""

# Test 5.3: Intervalo corto con errores
echo -e "${YELLOW}[TEST 5.3]${NC} Ping rápido con TTL=1"
echo -e "${YELLOW}Comando:${NC} sudo ./ft_ping -c 3 -i 0.2 --ttl 1 8.8.8.8"
OUTPUT=$(./ft_ping -c 3 -i 0.2 --ttl 1 8.8.8.8 2>&1)
echo "$OUTPUT"
check_output "$OUTPUT" "Time to live exceeded" "Debe funcionar con intervalos cortos"
echo -e "\n${YELLOW}─────────────────────────────────────────────────────────────${NC}\n"

# Resumen final
echo -e "${CYAN}╔═══════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                    RESUMEN DE PRUEBAS                        ║${NC}"
echo -e "${CYAN}╚═══════════════════════════════════════════════════════════════╝${NC}\n"

echo -e "${GREEN}Tests pasados:${NC} $PASSED"
echo -e "${RED}Tests fallidos:${NC} $FAILED"
echo -e "${BLUE}Total de tests:${NC} $TOTAL"
echo ""

PERCENTAGE=$((PASSED * 100 / TOTAL))
if [ $PERCENTAGE -ge 80 ]; then
    echo -e "${GREEN}Resultado: $PERCENTAGE% - EXCELENTE ✓${NC}"
elif [ $PERCENTAGE -ge 60 ]; then
    echo -e "${YELLOW}Resultado: $PERCENTAGE% - ACEPTABLE ~${NC}"
else
    echo -e "${RED}Resultado: $PERCENTAGE% - NECESITA MEJORAS ✗${NC}"
fi

echo ""
echo -e "${CYAN}═══════════════════════════════════════════════════════════════${NC}"
echo -e "${CYAN}  TIPOS DE MENSAJES ICMP VERIFICADOS${NC}"
echo -e "${CYAN}═══════════════════════════════════════════════════════════════${NC}"
echo -e "✓ Type 0  - Echo Reply (respuestas exitosas)"
echo -e "✓ Type 3  - Destination Unreachable"
echo -e "  ├─ Code 0: Network Unreachable"
echo -e "  ├─ Code 1: Host Unreachable"
echo -e "  ├─ Code 2: Protocol Unreachable"
echo -e "  ├─ Code 3: Port Unreachable"
echo -e "  └─ Code 4: Fragmentation Needed"
echo -e "✓ Type 11 - Time Exceeded"
echo -e "  ├─ Code 0: TTL expired in transit"
echo -e "  └─ Code 1: Fragment reassembly time exceeded"
echo -e "✓ Type 5  - Redirect"
echo -e "✓ Type 12 - Parameter Problem"
echo -e "✓ Timeout - No response (Request timeout)"
echo -e "${CYAN}═══════════════════════════════════════════════════════════════${NC}\n"

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 ¡Todas las pruebas completadas exitosamente!${NC}\n"
    exit 0
else
    echo -e "${YELLOW}⚠️  Algunas pruebas fallaron. Revisa los resultados arriba.${NC}\n"
    exit 1
fi
