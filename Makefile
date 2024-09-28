all:
	@cmake -B build
	@cmake --build build
	@mv build/Scop .

clean:
	@rm -rf build

fclean: clean
	@rm -f Scop

re: fclean all

.PHONY: all clean fclean re