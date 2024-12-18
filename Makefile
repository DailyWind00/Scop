MAKEFLAGS += --no-print-directory
GREEN = \033[1;32m
RESET = \033[0m

all:
	@cmake -B build
	@make -C build -j
	@mv build/Scop .
	@echo "$(GREEN)Scop is ready to use : ./Scop [options] <.obj file>$(RESET)"

clean:
	@rm -rf build

fclean: clean
	@rm -f Scop

re: fclean all

.PHONY: all clean fclean re