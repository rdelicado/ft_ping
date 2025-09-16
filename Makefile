MAKEFLAGS += --no-print-directory

# --- Configuración básica --- #
NAME        = ft_ping
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g

INCLUDES    = -Iinclude
LIBS        = -lm

# --- Directorios --- #
SRC_DIR     = src/
OBJ_DIR     = obj/

# --- Fuentes y objetos --- #
SRC_FILES   = $(shell find $(SRC_DIR) -name "*.c")
OBJ_FILES   = $(patsubst $(SRC_DIR)%.c,$(OBJ_DIR)%.o,$(SRC_FILES))

# --- Colores (opcional) --- #
GREEN       = \033[0;32m
RED         = \033[0;31m
BLUE        = \033[0;34m
NC          = \033[0m

# --- Reglas --- #
all: $(NAME)

$(NAME): $(OBJ_FILES)
	@echo "$(GREEN)[✔] Enlazando objetos...$(NC)"
	@$(CC) $(CFLAGS) $(OBJ_FILES) -o $@ $(LIBS)
	@echo "$(GREEN)[✔] $(NAME) compilado correctamente!$(NC)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	@echo "$(BLUE)[⚙] Compilando: $<$(NC)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $@

# --- Limpieza --- #
clean:
	@echo "$(RED)[✗] Eliminando objetos...$(NC)"
	@rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)[✗] Eliminando ejecutable...$(NC)"
	@rm -f $(NAME)

re: fclean all

# --- PHONY --- #
.PHONY: all clean fclean re