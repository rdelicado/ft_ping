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
	unsigned long val = strtoul(dec, NULL, 10);
	if (val > 0xFFFFFFFFUL) return false;
	struct in_addr addr;
	addr.s_addr = htonl(val);
	if (!inet_ntop(AF_INET, &addr, out, outlen)) return 0;
	return true;
}
