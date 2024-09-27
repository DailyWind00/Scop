NAME    = Scop
CC      = c++
RM      = rm -f
CFLAGS += -g -Wall -Wextra -Werror -O3
INCLUDE = -I includes -I includes/classes
DEBUG   = 0

SRC		= srcs/main.cpp \
			includes/classes/OBJ.cpp #

OBJ		= $(SRC:.cpp=.o)

all:
	@if [ -f $(NAME) ]; then \
		printf "\033[1;92m\n$(NAME) already compiled\n\033[0m"; \
		make -s redirect; \
	else \
		printf "\033[1;36m\nStarting $(NAME) compilation\n\e[0m"; \
		make -s redirect; \
	fi

redirect: $(NAME)

$(NAME): $(OBJ)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJ)
	@printf "\033[1;92m >>> $(NAME) compiled                                                     \n\033[0m"
	@if [ $(DEBUG) -eq 1 ]; then printf "\033[1;93m >>> Debug mode activated\n\033[0m"; fi
	@printf "\n\033[0m"

%.o: %.cpp
	@printf "\033[0;33mCompilation de %-33.33s\r\e[0m" $@
	@if [ $(DEBUG) -eq 1 ]; then $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ -D DEBUG=1; else $(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@; fi

clean:
	@$(RM) $(OBJ)
	@printf "\033[1;31m- Object files deleted\n\033[0m"

fclean: clean	
	@$(RM) $(NAME)
	@printf "\033[1;31m- Executable file deleted\n\033[0m"

re: fclean all

debug: fclean
	@make -s DEBUG=1 all

.PHONY: all redirect clean fclean re debug