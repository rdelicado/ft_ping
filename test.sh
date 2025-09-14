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
            # Extraer mensaje después de "ft_ping:" para comparar
            if echo "$output" | grep -q "ft_ping:.*:"; then
                # Caso: "ft_ping: destino: mensaje"
                msg=$(echo "$output" | sed 's/ft_ping:.*: //' | head -1)
            else
                # Caso: "ft_ping: mensaje"
                msg=$(echo "$output" | sed 's/ft_ping: //' | head -1)
            fi
            if [[ "$msg" == "$expected" ]] || [[ "$msg" == *"$expected"* ]]; then
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
    if [ -z "$args" ]; then
        PING_COMPARISON_STATUS="${GREEN}[PING:✓]${NC}"
        PING_MATCHES=$((PING_MATCHES + 1))
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
    else
        ping_output=$(timeout 3s ping -c 1 -W 2 $args 2>&1)
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
                ping_msg=$(echo "$normalized_ping" | sed 's/.*ft_ping: //' | head -1)
                ft_msg=$(echo "$ft_output" | sed 's/.*ft_ping: //' | head -1)
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
# PRUEBAS DE ARGUMENTOS BÁSICOS
# ============================================================================
echo -e "${CYAN}=== Pruebas de Argumentos Básicos ===${NC}"

# 1. Sin argumentos
run_test "Sin argumentos" "./ft_ping" "usage error: Destination address required" "true" "true"

# 2. Solo ayuda (-?) - Subject requiere -? en lugar de -h
run_help_test "Solo ayuda (-?)" "./ft_ping \"-?\""

# 3. Solo verbose sin destino (debe fallar)
run_test "Solo verbose (-v)" "./ft_ping -v" "usage error: Destination address required" "true"

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
run_test "Solo hostname (google.com)" "./ft_ping google.com" ""

# 8. Solo IP válida
run_test "Solo IP (8.8.8.8)" "./ft_ping 8.8.8.8" ""

# 9. Solo localhost
run_test "Solo localhost" "./ft_ping localhost" ""

# 10. IP local (127.0.0.1)
run_test "IP local (127.0.0.1)" "./ft_ping 127.0.0.1" ""

# ============================================================================
# PRUEBAS DE IPs VÁLIDAS
# ============================================================================
echo -e "${CYAN}=== Pruebas de IPs Válidas ===${NC}"

# IPs válidas
run_test "IP mínima (0.0.0.0)" "./ft_ping 0.0.0.0" ""
run_test "IP máxima (255.255.255.255)" "./ft_ping 255.255.255.255" "Do you want to ping broadcast" "true"
run_test "IP pública (1.2.3.4)" "./ft_ping 1.2.3.4" ""
run_test "IP loopback (127.0.0.1)" "./ft_ping 127.0.0.1" ""
run_test "IP Google DNS (8.8.8.8)" "./ft_ping 8.8.8.8" ""

# ============================================================================
# PRUEBAS DE DESTINOS INVÁLIDOS
# ============================================================================
echo -e "${CYAN}=== Pruebas de Destinos Inválidos ===${NC}"

# 20. Hostname completamente inválido
run_test "Hostname inválido" "./ft_ping thisdomaindoesnotexist12345.invalid" "Name or service not known" "true"

# 21. IP malformada
run_test "IP malformada (999.999.999.999)" "./ft_ping 999.999.999.999" "Name or service not known" "true"

# 22. IP incompleta (ahora se convierte correctamente)
run_test "IP incompleta (192.168.1)" "./ft_ping 192.168.1" ""

# 23. String vacío como destino (si es posible)
run_test "String vacío" "./ft_ping ''" "No address associated with hostname" "true" "true"

# ============================================================================
# PRUEBAS DE IPs INVÁLIDAS
# ============================================================================
echo -e "${CYAN}=== Pruebas de IPs Inválidas ===${NC}"

# Formatos que ping convierte (como ping original)
run_test "Solo un segmento (192)" "./ft_ping 192" ""
run_test "Dos segmentos (192.168)" "./ft_ping 192.168" ""
run_test "Tres segmentos (192.168.1)" "./ft_ping 192.168.1" ""
run_test "Cinco segmentos (1.2.3.4.5)" "./ft_ping 1.2.3.4.5" "Name or service not known" "true"
run_test "Punto inicial (.1.2.3.4)" "./ft_ping .1.2.3.4" "Name or service not known" "true"
run_test "Punto final (1.2.3.4.)" "./ft_ping 1.2.3.4." "Name or service not known" "true"
run_test "Doble punto (1..2.3)" "./ft_ping 1..2.3" "Name or service not known" "true"

# Valores fuera de rango
run_test "Segmento >255 (256.1.1.1)" "./ft_ping 256.1.1.1" "Name or service not known" "true"
run_test "Segmento negativo (1.2.3.-1)" "./ft_ping 1.2.3.-1" "Name or service not known" "true"
run_test "Segmento >255 (1.2.3.999)" "./ft_ping 1.2.3.999" "Name or service not known" "true"
run_test "Segmento vacío (1.2..4)" "./ft_ping 1.2..4" "Name or service not known" "true"

# ============================================================================
echo -e "${CYAN}=== Pruebas de IPs Decimales Válidas ===${NC}"

# IPs decimales válidas (formato decimal de IPs conocidas)
run_test "IP decimal 0 (0.0.0.0)" "./ft_ping 0" ""
run_test "IP decimal localhost (2130706433)" "./ft_ping 2130706433" ""
run_test "IP decimal Google DNS (134744072)" "./ft_ping 134744072" ""
run_test "IP decimal 192.168.1.1 (3232235777)" "./ft_ping 3232235777" ""
run_test "IP decimal máxima (4294967295)" "./ft_ping 4294967295" "Do you want to ping broadcast" "true"

# ============================================================================
echo -e "${CYAN}=== Pruebas de IPs Decimales Inválidas ===${NC}"

# Overflow y casos inválidos
run_test "IP decimal overflow (4294967296)" "./ft_ping 4294967296" "Temporary failure in name resolution" "true"
run_test "IP decimal muy grande (99999999999)" "./ft_ping 99999999999" "Temporary failure in name resolution" "true"
run_test "IP decimal negativa (-1)" "./ft_ping -1" "invalid option" "true"
run_test "IP decimal con signo negativo (-123)" "./ft_ping -123" "invalid option" "true"

# ============================================================================
echo -e "${CYAN}=== Pruebas de Casos Edge y Raros ===${NC}"

# Combinaciones extrañas de caracteres
run_test "Números con letras (123abc)" "./ft_ping 123abc" "Temporary failure in name resolution" "true"
run_test "Hexadecimal (0x12345678)" "./ft_ping 0x12345678" ""
run_test "Octal (0123456)" "./ft_ping 0123456" ""
run_test "Decimal con espacios ( 123456 )" "./ft_ping ' 123456 '" "Name or service not known" "true"
run_test "Decimal con ceros a la izquierda (0000123456)" "./ft_ping 0000123456" ""

# Casos con símbolos especiales
run_test "IP con símbolo + (+192.168.1.1)" "./ft_ping +192.168.1.1" "Name or service not known" "true"
run_test "Número con coma decimal (192,168)" "./ft_ping 192,168" "Name or service not known" "true"
run_test "Número científico (1e6)" "./ft_ping 1e6" "Temporary failure in name resolution" "true"

# Casos muy largos
run_test "String muy largo (100 caracteres)" "./ft_ping $(printf '%0101s' | tr ' ' '9')" "Name or service not known" "true"
run_test "IP con muchos puntos (...)" "./ft_ping ..." "Name or service not known" "true"
run_test "Solo puntos (.....)" "./ft_ping ....." "Name or service not known" "true"

# Casos con caracteres Unicode/especiales  
run_test "Caracteres especiales (cafe.com)" "./ft_ping cafe.com" ""
run_test "Caracteres especiales (emoji.com)" "./ft_ping emoji.com" ""

# ============================================================================
# PRUEBAS DE COMPATIBILIDAD CON PING ORIGINAL
# ============================================================================
echo ""
echo -e "${CYAN}=== Pruebas de Compatibilidad con Ping Original ===${NC}"

# Casos específicos probados para coincidir mensajes de error
run_test "Sin argumentos" "./ft_ping" "usage error: Destination address required"
run_test "IP inválida (192.168.1.999)" "./ft_ping 192.168.1.999" "Name or service not known"
run_test "Hex inválido (0x)" "./ft_ping 0x" "Temporary failure in name resolution"
run_test "Hostname inválido con caracteres válidos (0xxxx)" "./ft_ping 0xxxx" "Temporary failure in name resolution"
run_test "Destino con caracteres inválidos (////)" "./ft_ping ////" "Name or service not known"
run_test "Destino con caracteres inválidos (abc!)" "./ft_ping abc!" "Name or service not known" "true"
run_test "IP válida (8.8.8.8)" "./ft_ping 8.8.8.8" ""  # No debe tener error
run_test "Hostname válido (google.com)" "./ft_ping google.com" ""  # No debe tener error
run_test "Hex válido (0x3)" "./ft_ping 0x3" ""  # No debe tener error

# ============================================================================
# PRUEBAS DE MEMORY LEAKS
# ============================================================================
echo ""
echo -e "${CYAN}=== Pruebas de Memory Leaks ===${NC}"

# Pruebas de memory leaks con diferentes tipos de destinos
run_memory_test "Hostname válido (google.com)" "timeout 3 ./ft_ping google.com"
run_memory_test "IP válida (8.8.8.8)" "timeout 3 ./ft_ping 8.8.8.8"
run_memory_test "Localhost" "timeout 3 ./ft_ping localhost"
run_memory_test "IP local (127.0.0.1)" "timeout 3 ./ft_ping 127.0.0.1"
run_memory_test "Con verbose (-v)" "timeout 3 ./ft_ping -v google.com"
run_memory_test "Decimal IP (2130706433)" "timeout 3 ./ft_ping 2130706433"
run_memory_test "Decimal IP con verbose" "timeout 3 ./ft_ping -v 2130706433"

# Pruebas de memory leaks con casos de error (deben fallar pero sin leaks)
run_memory_test "Sin argumentos (error)" "./ft_ping"
run_memory_test "Destino inválido (error)" "./ft_ping invalid.host.name"
run_memory_test "IP inválida (error)" "./ft_ping 999.999.999.999"

# ============================================================================
# RESUMEN Y ESTADÍSTICAS
# ============================================================================
echo ""
show_stats