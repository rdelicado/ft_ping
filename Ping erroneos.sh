# Errores en ICMP https://rlworkman.net/howtos/iptables/spanish/chunkyhtml/a4189.html

# 1. Destino inalcanzable (Host Unreachable)
sudo ./ft_ping 10.255.255.1
# (Esa IP normalmente no existe en redes locales, deberías ver:
# From [IP]: Host Unreachable (ICMP type 3, code 1))

# 2. TTL Exceeded (Time Exceeded)
sudo ./ft_ping --ttl 1 8.8.8.8
# (Con TTL=1, el primer router descarta el paquete y deberías ver:
# From [IP]: Time to live exceeded (ICMP type 11))

# 3. Fragmentation needed and DF set (-M do = No fragmentar)
# Ping estándar con DF activado (no fragmentar):
	sudo ping -M do -s 1472 -c 1 8.8.8.8
# 1472 es el máximo tamaño de datos ICMP para MTU 1500 (Ethernet).
# Si usas un tamaño mayor:
sudo ping -M do -s 1473 -c 1 8.8.8.8
# Debería fallar con “Message too long” o “Frag needed”.
# 2. Tu ft_ping con tamaño grande (sin DF, no lo tengo implementado):
sudo ./ft_ping -s 1472 -c 1 8.8.8.8
sudo ./ft_ping -s 1473 -c 1 8.8.8.8
# Si no tienes DF implementado, el kernel fragmentará el paquete y no verás error.
# Si implementas DF, deberías ver el mensaje “Fragmentation needed and DF set”.