# ft_ping - Pruebas Automatizadas

Este documento describe los scripts de pruebas automáticas para el proyecto ft_ping.

## Archivos de Pruebas

### 1. `test_argument_parsing.sh`
**Propósito:** Valida el parseo de argumentos y el manejo de flags en la línea de comandos.

**Cobertura:**
- Argumentos básicos (sin argumentos, ayuda, verbose)
- Flags individuales (-v, -c, -s, -i, -W, --ttl, -f)
- Valores inválidos para flags (negativos, cero, fuera de rango)
- Detección de flags desconocidas
- Combinaciones de flags
- Formatos de destino (IP, hostname, decimal, hexadecimal, octal)
- Destinos inválidos
- Casos especiales (valores faltantes, múltiples destinos)

**Estadísticas:**
- Total de tests: 43
- Tiempo de ejecución: ~30 segundos
- Porcentaje de éxito: 100%

**Uso:**
```bash
sudo ./test_argument_parsing.sh
```

### 2. `test_icmp_messages.sh`
**Propósito:** Verifica el manejo de mensajes ICMP y la presentación de errores.

**Cobertura:**
- Type 0 - Echo Reply (respuestas exitosas)
- Type 3 - Destination Unreachable (códigos 0-4)
- Type 11 - Time Exceeded (TTL expirado)
- Type 5 - Redirect
- Type 12 - Parameter Problem
- Timeout (sin respuesta)
- Casos especiales (múltiples paquetes, paquetes grandes, intervalos rápidos)

**Estadísticas:**
- Total de tests: 22
- Tiempo de ejecución: ~40 segundos
- Porcentaje de éxito: 100%

**Uso:**
```bash
sudo ./test_icmp_messages.sh
```

## Ejecutar Todas las Pruebas

Para ejecutar ambas suites de pruebas de forma secuencial:

```bash
sudo ./test_argument_parsing.sh && sudo ./test_icmp_messages.sh
```

## Resultados de las Pruebas

Ambos scripts muestran:
- ✅ **PASS** para tests exitosos
- ❌ **FAIL** con mensajes de error detallados
- 📊 Resumen final de estadísticas
- 🎯 Porcentaje de éxito

## Requisitos

- Privilegios de root (sudo) para operaciones con sockets RAW
- Conectividad de red para pruebas con hosts remotos
- Binario `ft_ping` compilado en el mismo directorio

## Notas

- Las pruebas usan `localhost` y servidores DNS públicos (8.8.8.8, 1.1.1.1)
- Algunas pruebas pueden demorar más en redes lentas
- Los tests de errores ICMP dependen de la configuración de la red
