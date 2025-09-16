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
PING_COMPARISONS=0
PING_MATCHES=0

# Función auxiliar para ping tests que aceptan timeout como válido
run_ping_test() {
    local desc="$1"
    local cmd="$2" 
    local expected="$3"
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Prueba $TOTAL_TESTS: $desc${NC}"
    echo -e "${YELLOW}Comando: $cmd${NC}"

    # Ejecutar comando con timeout de 3 segundos
    output=$(eval "timeout 3s $cmd" 2>&1)
    exit_code=$?

    # Para ping tests, aceptamos exit code 124 (timeout) como válido si contiene output esperado
    local test_passed=false
    
    if [[ "$output" == *"$expected"* ]]; then
        if [ $exit_code -eq 0 ] || [ $exit_code -eq 124 ]; then
            test_passed=true
        fi
    fi
    
    if $test_passed; then
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

# Función especial para el test de ayuda con warning explicativo
run_help_test() {
    local desc="$1"
    local cmd="$2"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Prueba $TOTAL_TESTS: $desc${NC}"
    echo -e "${YELLOW}Comando: $cmd${NC}"

    # Ejecutar comando y capturar salida
    output=$(eval "$cmd" 2>&1)
    exit_code=$?

    # Para el test de ayuda, siempre consideramos PASS si muestra Usage
    if [[ "$output" == *"Usage"* ]]; then
        echo -e "${GREEN}✓ PASS${NC} ${CYAN}(Subject requirement: -? instead of -h)${NC}"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo -e "${RED}Salida real: $output${NC}"
        echo -e "${RED}Exit code: $exit_code${NC}"
    fi
    echo "---"
}

# Función para ejecutar pruebas de memory leaks con Valgrind
run_memory_test() {
    local desc="$1"
    local cmd="$2"

    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    echo -e "${BLUE}Prueba $TOTAL_TESTS: Memory Leak - $desc${NC}"
    echo -e "${YELLOW}Comando: valgrind $cmd${NC}"

    # Ejecutar con Valgrind y capturar salida
    valgrind_output=$(valgrind --leak-check=summary --error-exitcode=1 $cmd 2>&1)
    valgrind_exit=$?

    # Analizar resultados - verificar que no haya leaks Y que el programa termine correctamente
    if echo "$valgrind_output" | grep -q "All heap blocks were freed -- no leaks are possible"; then
        echo -e "${GREEN}✓ PASS${NC} - Sin memory leaks detectados"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAIL${NC} - Memory leak detectado o error en análisis"
        echo -e "${RED}Valgrind output:${NC}"
        echo "$valgrind_output" | head -10
        if [ $valgrind_exit -ne 0 ]; then
            echo -e "${RED}Exit code: $valgrind_exit${NC}"
        fi
    fi
    echo "---"
}

# Función para ejecutar y mostrar resultado con PASS/FAIL + comparación con ping original
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
        if [ $exit_code -ne 0 ]; then
            # Verificar si la salida completa contiene el mensaje esperado
            if [[ "$output" == *"$expected"* ]]; then
                test_passed=true
            fi
        fi
    else
        # Para casos exitosos, verificar exit code 0 Y mensaje esperado
        if [ $exit_code -eq 0 ] && [[ "$output" == *"$expected"* ]]; then
            test_passed=true
        fi
    fi
    
    # NUEVA FUNCIONALIDAD: Comparar con ping original si es posible
    local ping_comparison_result=""
    if [[ "$cmd" == *"./ft_ping"* ]] && [[ "$cmd" != *'"-?"'* ]] && [[ "$desc" != *"ayuda"* ]]; then
        # Extraer argumentos del comando (quitar "./ft_ping " del inicio)
        local args=$(echo "$cmd" | sed 's/.*\.\/ft_ping //')
        # Solo comparar si hay argumentos (no casos vacíos o solo flags)
        if [[ "$args" != "-v" ]] && [[ "$args" != "" ]]; then
            compare_with_ping "$args" "$exit_code" "$output"
            ping_comparison_result=" $PING_COMPARISON_STATUS"
        fi
    fi
    
    if [ "$test_passed" = true ]; then
        echo -e "${GREEN}✓ PASS${NC}$ping_comparison_result"
        PASSED_TESTS=$((PASSED_TESTS + 1))
    else
        echo -e "${RED}✗ FAIL${NC}$ping_comparison_result"
        echo -e "${RED}Salida real: $output${NC}"
        echo -e "${RED}Exit code: $exit_code${NC}"
        echo -e "${RED}Esperado: $expected${NC}"
    fi
    echo "---"
}

# Función para comparar con ping original
compare_with_ping() {
    local args="$1"
    local ft_exit="$2"
    local ft_output="$3"
    
    PING_COMPARISONS=$((PING_COMPARISONS + 1))
    
    # No comparar para comandos sin argumentos
    if [ -z "$args" ] || [ "$args" = "./ft_ping" ]; then
        # Para comandos sin argumentos, comparar directamente los mensajes
        ping_msg="usage error: Destination address required"
        ft_msg=$(echo "$ft_output" | sed 's/.*ft_ping: //' | head -1)
        if [ "$ping_msg" = "$ft_msg" ]; then
            PING_COMPARISON_STATUS="${GREEN}[PING:✓ MSG:✓]${NC}"
            PING_MATCHES=$((PING_MATCHES + 1))
        else
            PING_COMPARISON_STATUS="${RED}[PING:✓ MSG:✗ '$ping_msg' vs '$ft_msg']${NC}"
        fi
        return
    fi
    
    # Determinar si ft_ping acepta los argumentos
    ft_accepts=false
    if [ $ft_exit -eq 0 ] && ! echo "$ft_output" | grep -q "ft_ping:"; then
        ft_accepts=true
    fi
    
    # Probar con ping original (timeout más largo para evitar false positives)
    if [ -z "$args" ]; then
        ping_output="ping: usage error: Destination address required"
        ping_exit=1
    elif [ "$args" = "''" ]; then
        # Caso especial para string vacío
        ping_output=$(timeout 3s ping '' 2>&1)
        ping_exit=$?
    else
        ping_output=$(eval "timeout 3s ping -c 1 -W 2 $args 2>&1")
        ping_exit=$?
    fi
    
    # Determinar si ping original acepta los argumentos
    ping_accepts=false
    # Exit codes: 0=success, 1=timeout pero args válidos, 2=error args, 124=timeout del comando timeout
    if [ $ping_exit -eq 0 ] || [ $ping_exit -eq 1 ]; then
        ping_accepts=true
    elif [ $ping_exit -eq 124 ]; then
        # Timeout del comando timeout - asumir que args son válidos si no hay error inmediato  
        if ! echo "$ping_output" | grep -q "ping:.*invalid\|ping:.*failure\|ping:.*error\|ping:.*unknown"; then
            ping_accepts=true
        fi
    fi
    
    # Comparar comportamientos
    if [ "$ft_accepts" = "$ping_accepts" ]; then
        # Si ambos rechazan, comparar mensajes de error
        if [ "$ft_accepts" = "false" ]; then
            # Normalizar mensajes: reemplazar "ping:" por "ft_ping:" en ping_output
            normalized_ping=$(echo "$ping_output" | sed 's/ping:/ft_ping:/g')
            if echo "$normalized_ping" | grep -q "ft_ping:" && echo "$ft_output" | grep -q "ft_ping:"; then
                # Extraer mensaje después de "ft_ping:"
                # Caso especial para string vacío: "ft_ping: : mensaje"
                if echo "$normalized_ping" | grep -q "ft_ping: :"; then
                    ping_msg=$(echo "$normalized_ping" | sed 's/ft_ping: : //' | head -1)
                    ft_msg=$(echo "$ft_output" | sed 's/ft_ping: : //' | head -1)
                else
                    ping_msg=$(echo "$normalized_ping" | sed 's/.*ft_ping: //' | head -1)
                    ft_msg=$(echo "$ft_output" | sed 's/.*ft_ping: //' | head -1)
                fi
                if [ "$ping_msg" = "$ft_msg" ]; then
                    PING_COMPARISON_STATUS="${GREEN}[PING:✓ MSG:✓]${NC}"
                    PING_MATCHES=$((PING_MATCHES + 1))
                else
                    PING_COMPARISON_STATUS="${RED}[PING:✓ MSG:✗ '$ping_msg' vs '$ft_msg']${NC}"
                fi
            else
                PING_COMPARISON_STATUS="${GREEN}[PING:✓]${NC}"
                PING_MATCHES=$((PING_MATCHES + 1))
            fi
        else
            PING_COMPARISON_STATUS="${GREEN}[PING:✓]${NC}"
            PING_MATCHES=$((PING_MATCHES + 1))
        fi
    else
        PING_COMPARISON_STATUS="${RED}[PING:✗ ft=$ft_accepts ping=$ping_accepts]${NC}"
        # Si hay discrepancia, mostrar detalles
        if [ "$ft_accepts" != "$ping_accepts" ]; then
            echo -e "${YELLOW}⚠️  PING COMPARISON MISMATCH:${NC}"
            echo -e "   ft_ping acepta: $ft_accepts (exit: $ft_exit)"
            echo -e "   ping acepta:    $ping_accepts (exit: $ping_exit)"
            echo -e "   ping output: ${ping_output:0:80}..."
        fi
    fi
}

# Función para mostrar estadísticas finales
show_stats() {
    echo -e "${CYAN}=== Estadísticas Finales ===${NC}"
    echo -e "Total de pruebas: $TOTAL_TESTS"
    echo -e "${GREEN}Pasadas: $PASSED_TESTS${NC}"
    echo -e "${RED}Falladas: $((TOTAL_TESTS - PASSED_TESTS))${NC}"
    
    # Estadísticas de comparación con ping original
    if [ $PING_COMPARISONS -gt 0 ]; then
        echo ""
        echo -e "${CYAN}=== Comparación con ping original ===${NC}"
        echo -e "Comparaciones realizadas: $PING_COMPARISONS"
        echo -e "${GREEN}Comportamiento idéntico: $PING_MATCHES${NC}"
        echo -e "${RED}Comportamiento diferente: $((PING_COMPARISONS - PING_MATCHES))${NC}"
        
        PING_PERCENTAGE=$((PING_MATCHES * 100 / PING_COMPARISONS))
        echo -e "Compatibilidad con ping: ${PING_PERCENTAGE}%"
        
        if [ $PING_MATCHES -eq $PING_COMPARISONS ]; then
            echo -e "${GREEN}🎯 Comportamiento 100% compatible con ping original${NC}"
        elif [ $PING_PERCENTAGE -ge 90 ]; then
            echo -e "${YELLOW}⚠️  Muy buena compatibilidad (${PING_PERCENTAGE}%), revisar casos diferentes${NC}"
        else
            echo -e "${RED}❌ Compatibilidad baja (${PING_PERCENTAGE}%), necesita mejoras${NC}"
        fi
    fi
    
    echo ""
    if [ $PASSED_TESTS -eq $TOTAL_TESTS ]; then
        echo -e "${GREEN}¡Todas las pruebas pasaron! 🎉${NC}"
        echo -e "${CYAN}📋 Nota: Test de ayuda (-?) cumple con requisitos del subject${NC}"
        echo -e "${CYAN}🛡️  Nota: Todas las pruebas de memory leaks pasaron - sin fugas detectadas${NC}"
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
# GRUPO 1: TESTS DE VALIDACIÓN (Argumentos, parámetros, ayuda)
# ============================================================================
echo -e "${CYAN}╔══════════════════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                           GRUPO 1: TESTS DE VALIDACIÓN                       ║${NC}"
echo -e "${CYAN}║                     (Argumentos, parámetros, ayuda)                         ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════════════════════════════════════════════╝${NC}"
echo ""

echo -e "${BLUE}--- Validación de Argumentos Básicos ---${NC}"
# 1. Sin argumentos
run_test "Sin argumentos" "./ft_ping" "usage error: Destination address required" "true" "true"

# 2. Solo ayuda (-?) - Subject requiere -? en lugar de -h
run_help_test "Solo ayuda (-?)" "./ft_ping \"-?\""

# 3. Solo verbose sin destino (debe fallar)
run_test "Solo verbose (-v)" "./ft_ping -v" "usage error: Destination address required" "true"

echo -e "${BLUE}--- Validación de Flags ---${NC}"

# 4. Flag verbose con hostname válido
run_ping_test "Verbose con hostname" "sudo ./ft_ping -v google.com" "Modo verbose activado"

# 5. Flag verbose con IP válida
run_test "Verbose con IP" "./ft_ping -v 8.8.8.8" "Modo verbose activado"

# 6. Flag verbose con localhost
run_test "Verbose con localhost" "./ft_ping -v localhost" "Modo verbose activado"

echo -e "${BLUE}--- Validación de Destinos Básicos ---${NC}"
# Casos básicos de destinos válidos (solo verificar que no fallen)
run_ping_test "Hostname válido (google.com)" "sudo ./ft_ping google.com" "64 bytes from"
run_ping_test "IP válida (8.8.8.8)" "sudo ./ft_ping 8.8.8.8" "64 bytes from"
run_ping_test "Localhost" "sudo ./ft_ping localhost" "64 bytes from"
run_ping_test "IP local (127.0.0.1)" "sudo ./ft_ping 127.0.0.1" "64 bytes from"

# ============================================================================
# GRUPO 2: TESTS DE CASOS DE ERROR (Argumentos raros, mensajes de advertencia)
# ============================================================================
echo ""
echo -e "${CYAN}╔══════════════════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                         GRUPO 2: TESTS DE CASOS DE ERROR                    ║${NC}"
echo -e "${CYAN}║                  (Argumentos raros, mensajes de advertencia)                ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════════════════════════════════════════════╝${NC}"
echo ""

echo -e "${BLUE}--- Argumentos Inválidos ---${NC}"
# Tests que deberían fallar con mensajes específicos de error
run_test "Flag inválido (-x)" "./ft_ping -x google.com" "invalid option" "true"
run_test "Flag inválido (-h)" "./ft_ping -h google.com" "invalid option" "true"
run_test "Múltiples flags inválidos (-xyz)" "./ft_ping -xyz google.com" "invalid option" "true"
run_test "Destino como flag (-google.com)" "./ft_ping -google.com" "invalid option" "true"

echo -e "${BLUE}--- IPs con Casos Especiales ---${NC}"
# Tests que pueden generar advertencias o comportamientos especiales
run_ping_test "IP broadcast (255.255.255.255)" "sudo ./ft_ping 255.255.255.255" "Do you want to ping broadcast"
run_ping_test "IP de red (0.0.0.0)" "sudo ./ft_ping 0.0.0.0" "64 bytes from"
run_ping_test "IP multicast (224.0.0.1)" "sudo ./ft_ping 224.0.0.1" "64 bytes from"

echo -e "${BLUE}--- Formatos de IP Inválidos ---${NC}"
# IPs malformadas que deberían fallar
run_test "IP con octeto > 255 (256.1.1.1)" "./ft_ping 256.1.1.1" "Name or service not known" "true"
run_test "IP con octetos negativos (-1.1.1.1)" "./ft_ping -1.1.1.1" "invalid option" "true"
run_test "IP incompleta (192.168.1)" "./ft_ping 192.168.1" "Name or service not known" "true"
run_test "IP con demasiados octetos (1.2.3.4.5)" "./ft_ping 1.2.3.4.5" "Name or service not known" "true"
run_test "IP con caracteres no numéricos (1.2.a.4)" "./ft_ping 1.2.a.4" "Name or service not known" "true"

echo -e "${BLUE}--- Hostnames Inválidos ---${NC}"
run_test "Hostname inexistente" "./ft_ping thisdomaindoesnotexist12345.invalid" "Name or service not known" "true"
run_test "Hostname con caracteres inválidos" "./ft_ping abc!" "Name or service not known" "true"
run_test "Hostname vacío con caracteres especiales" "./ft_ping ////" "Name or service not known" "true"

echo -e "${BLUE}--- Formatos Hexadecimales y Decimales Inválidos ---${NC}"
run_test "Hex inválido (0x)" "./ft_ping 0x" "Temporary failure in name resolution" "true"
run_test "Hex incompleto (0xxxx)" "./ft_ping 0xxxx" "Temporary failure in name resolution" "true"

# ============================================================================
# GRUPO 3: TESTS DE PING REALES (Diferentes formatos de IP y estadísticas)
# ============================================================================
echo ""
echo -e "${CYAN}╔══════════════════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                        GRUPO 3: TESTS DE PING REALES                        ║${NC}"
echo -e "${CYAN}║                  (Diferentes formatos de IP y estadísticas)                 ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════════════════════════════════════════════╝${NC}"
echo ""

# Función especializada para pruebas de ping reales con estadísticas
run_ping_stats_test() {
    local desc="$1"
    local target="$2"
    local timeout_sec="$3"
    local expect_success="$4" # true/false si esperamos que funcione
    
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    PING_COMPARISONS=$((PING_COMPARISONS + 1))
    
    echo -e "${BLUE}Prueba $TOTAL_TESTS: $desc${NC}"
    echo -e "${YELLOW}Target: $target (timeout: ${timeout_sec}s)${NC}"
    
    # Ejecutar ft_ping con timeout y capturar salida
    echo -e "${CYAN}Ejecutando ft_ping...${NC}"
    ft_output=$(timeout ${timeout_sec}s sudo ./ft_ping $target 2>&1 || true)
    ft_exit=$?
    
    # Ejecutar ping original para comparar (solo si esperamos éxito)
    if [ "$expect_success" = "true" ]; then
        echo -e "${CYAN}Ejecutando ping original...${NC}"
        ping_output=$(timeout ${timeout_sec}s ping -c 3 $target 2>&1 || true)
        ping_exit=$?
    fi
    
    # Mostrar resultados de ft_ping
    echo -e "${GREEN}--- ft_ping output ---${NC}"
    echo "$ft_output"
    
    if [ "$expect_success" = "true" ]; then
        echo -e "${GREEN}--- ping original output ---${NC}"
        echo "$ping_output"
        
        # Analizar estadísticas de ambos
        ft_stats=$(echo "$ft_output" | grep -E "(packets transmitted|rtt min)" || echo "No stats found")
        ping_stats=$(echo "$ping_output" | grep -E "(packets transmitted|rtt min)" || echo "No stats found")
        
        echo -e "${YELLOW}📊 Comparación de estadísticas:${NC}"
        echo -e "${BLUE}ft_ping:${NC} $ft_stats"
        echo -e "${BLUE}ping:   ${NC} $ping_stats"
        
        # Verificar que ambos tengan estadísticas similares
        if [[ "$ft_output" == *"packets transmitted"* && "$ping_output" == *"packets transmitted"* ]]; then
            echo -e "${GREEN}✓ PASS${NC} - Ambos muestran estadísticas"
            PASSED_TESTS=$((PASSED_TESTS + 1))
            PING_MATCHES=$((PING_MATCHES + 1))
        else
            echo -e "${RED}✗ FAIL${NC} - Estadísticas no coinciden o faltan"
        fi
    else
        # Para casos que esperamos que fallen
        if [ $ft_exit -ne 0 ]; then
            echo -e "${GREEN}✓ PASS${NC} - Falló como se esperaba"
            PASSED_TESTS=$((PASSED_TESTS + 1))
        else
            echo -e "${RED}✗ FAIL${NC} - Debería haber fallado"
        fi
    fi
    echo "---"
}

echo -e "${BLUE}--- Formatos de IP Estándar (Dotted Decimal) ---${NC}"
# Pruebas con diferentes IPs en formato decimal con puntos
run_ping_stats_test "Localhost (127.0.0.1)" "127.0.0.1" "5" "true"
run_ping_stats_test "Google DNS (8.8.8.8)" "8.8.8.8" "5" "true"
run_ping_stats_test "Cloudflare DNS (1.1.1.1)" "1.1.1.1" "5" "true"
run_ping_stats_test "IP privada común (192.168.1.1)" "192.168.1.1" "5" "true"

echo -e "${BLUE}--- Formatos de Hostname ---${NC}"
# Pruebas con hostnames que resuelven a IP
run_ping_stats_test "Hostname (localhost)" "localhost" "5" "true"
run_ping_stats_test "Hostname (google.com)" "google.com" "6" "true"

echo -e "${BLUE}--- Formatos de IP Especiales ---${NC}"
# IPs con comportamientos especiales que aún funcionan
run_ping_stats_test "IP de loopback extendida (127.0.0.2)" "127.0.0.2" "5" "true"
run_ping_stats_test "IP pública aleatoria (1.2.3.4)" "1.2.3.4" "8" "true"

echo -e "${BLUE}--- Formatos Decimales (según ping original) ---${NC}"
# El ping original soporta algunos formatos decimales
run_ping_stats_test "Formato decimal (2130706433)" "2130706433" "5" "true"  # 127.0.0.1 en decimal
run_ping_stats_test "Formato octal (0177.0.0.1)" "0177.0.0.1" "5" "true"   # 127.0.0.1 en octal

echo -e "${BLUE}--- Formatos Hexadecimales (según ping original) ---${NC}"
# El ping original soporta algunos formatos hex
run_ping_stats_test "Formato hex (0x7f000001)" "0x7f000001" "5" "true"     # 127.0.0.1 en hex
run_ping_stats_test "Formato hex mixto (0x7f.0.0.1)" "0x7f.0.0.1" "5" "true" # hex/decimal mixto

# Pruebas con diferentes formatos de IP
echo -e "${YELLOW}Formatos decimales de IP:${NC}"
run_ping_stats_test "IP en formato decimal (127.0.0.1)" "127.0.0.1" "3"
run_ping_stats_test "IP decimal completa (2130706433 = 127.0.0.1)" "2130706433" "3"
run_ping_stats_test "IP decimal Google DNS (134744072 = 8.8.8.8)" "134744072" "4"

echo -e "${YELLOW}Formatos hexadecimales de IP:${NC}"
run_ping_stats_test "IP hexadecimal (0x7f000001 = 127.0.0.1)" "0x7f000001" "3"
run_ping_stats_test "IP hexadecimal Google DNS (0x08080808 = 8.8.8.8)" "0x08080808" "4"
run_ping_stats_test "IP hexadecimal simple (0x1 = 0.0.0.1)" "0x1" "3"

echo -e "${YELLOW}Formatos octales de IP:${NC}"
run_ping_stats_test "IP octal (0177000001 = 127.0.0.1)" "0177000001" "3"
run_ping_stats_test "IP octal simple (01 = 0.0.0.1)" "01" "3"

echo -e "${YELLOW}Formatos de IP compactos:${NC}"
run_ping_stats_test "IP compacta (192.168.1 = 192.168.0.1)" "192.168.1" "3"
run_ping_stats_test "IP muy compacta (10.1 = 10.0.0.1)" "10.1" "3"
run_ping_stats_test "IP mínima (1 = 0.0.0.1)" "1" "3"

echo -e "${YELLOW}Hostnames comunes:${NC}"
run_ping_stats_test "localhost" "localhost" "3"
run_ping_stats_test "google.com" "google.com" "4"

# ============================================================================
# PRUEBAS ADICIONALES DE MEMORY LEAKS Y RENDIMIENTO
# ============================================================================
echo ""
echo -e "${CYAN}╔══════════════════════════════════════════════════════════════════════════════╗${NC}"
echo -e "${CYAN}║                         PRUEBAS ADICIONALES                                  ║${NC}"
echo -e "${CYAN}║                    (Memory leaks y rendimiento)                             ║${NC}"
echo -e "${CYAN}╚══════════════════════════════════════════════════════════════════════════════╝${NC}"
echo ""

echo -e "${BLUE}--- Memory Leaks (Casos exitosos) ---${NC}"
# Pruebas de memory leaks con diferentes tipos de destinos válidos
run_memory_test "Hostname válido (google.com)" "timeout 3 ./ft_ping google.com"
run_memory_test "IP válida (8.8.8.8)" "timeout 3 ./ft_ping 8.8.8.8"
run_memory_test "Localhost" "timeout 3 ./ft_ping localhost"
run_memory_test "Con verbose (-v)" "timeout 3 ./ft_ping -v google.com"

echo -e "${BLUE}--- Memory Leaks (Casos de error) ---${NC}"
# Pruebas de memory leaks con casos de error (deben fallar pero sin leaks)
run_memory_test "Sin argumentos (error)" "./ft_ping"
run_memory_test "Destino inválido (error)" "./ft_ping invalid.host.name"
run_memory_test "IP inválida (error)" "./ft_ping 999.999.999.999"

# ============================================================================
# RESUMEN Y ESTADÍSTICAS
# ============================================================================
echo ""
show_stats