#include "ft_ping.h"

// Devuelve true si la cadena es un número decimal válido (sin puntos)
bool is_decimal_format(const char *ip) {
	if (!ip || !*ip) return false;
	for (int i = 0; ip[i]; i++)
		if (!isdigit((unsigned char)ip[i]))
			return false;
	return true;
}

// Convierte decimal a notación por puntos. Devuelve true si OK, false si error.
bool decimal_to_ip(const char *dec, char *out, size_t outlen) {
	char *endptr;
	unsigned long val = strtoul(dec, &endptr, 10);
	
	// Verificar que se convirtió toda la cadena y no hay overflow
	if (*endptr != '\0' || val > 0xFFFFFFFFUL) return false;
	
	struct in_addr addr;
	addr.s_addr = htonl(val);
	if (!inet_ntop(AF_INET, &addr, out, outlen)) return false;
	return true;
}

// Maneja casos especiales como ping original
bool handle_special_decimal(const char *input, char *out, size_t outlen) {
	if (!input || !*input) return false;
	
	char *endptr;
	unsigned long val;
	
	// Manejar hexadecimal (0x12345)
	if (input[0] == '0' && (input[1] == 'x' || input[1] == 'X')) {
		val = strtoul(input, &endptr, 16);
		if (*endptr != '\0' || val > 0xFFFFFFFFUL) return false;
		
		struct in_addr addr;
		addr.s_addr = htonl(val);
		if (!inet_ntop(AF_INET, &addr, out, outlen)) return false;
		return true;
	}
	
	// Manejar IPs incompletas (192.168.1 -> 192.168.0.1, 192.168 -> 192.0.0.168)
	int dots = 0;
	bool only_digits_and_dots = true;
	for (int i = 0; input[i]; i++) {
		if (input[i] == '.') {
			dots++;
		} else if (!isdigit((unsigned char)input[i])) {
			only_digits_and_dots = false;
			break;
		}
	}
	
	if (only_digits_and_dots && dots > 0 && dots < 3) {
		unsigned long parts[4] = {0, 0, 0, 0};
		char *token, *str_copy = strdup(input);
		int part = 0;
		
		token = strtok(str_copy, ".");
		while (token && part < 4) {
			parts[part] = strtoul(token, NULL, 10);
			if (parts[part] > 255) {
				free(str_copy);
				return false;
			}
			part++;
			token = strtok(NULL, ".");
		}
		free(str_copy);
		
		// Convertir según número de partes como ping original
		if (dots == 1) {
			// 192.168 -> 192.0.0.168
			if (snprintf(out, outlen, "%lu.0.0.%lu", parts[0], parts[1]) >= (int)outlen) return false;
		} else if (dots == 2) {
			// 192.168.1 -> 192.168.0.1  
			if (snprintf(out, outlen, "%lu.%lu.0.%lu", parts[0], parts[1], parts[2]) >= (int)outlen) return false;
		}
		return true;
	}
	
	// Verificar si es solo dígitos (decimal puro)
	for (int i = 0; input[i]; i++) {
		if (!isdigit((unsigned char)input[i])) {
			return false;
		}
	}
	
	val = strtoul(input, &endptr, 10);
	if (*endptr != '\0') return false;
	
	// Casos especiales como ping:
	// 192 -> 0.0.0.192 (números de 0-255)
	if (val <= 255) {
		if (snprintf(out, outlen, "0.0.0.%lu", val) >= (int)outlen) return false;
		return true;
	} 
	// Conversión decimal normal para números más grandes
	else if (val <= 0xFFFFFFFFUL) {
		struct in_addr addr;
		addr.s_addr = htonl(val);
		if (!inet_ntop(AF_INET, &addr, out, outlen)) return false;
		return true;
	}
	
	return false;
}
