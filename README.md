# Scop ✅125/100
3D rendering of a .obj object

To compile the project, you must have CMake, Make and OpenGL :
```shell
make         # Compile the project
make clean   # Remove the build Cmake folder
make fclean  # Remove the build Cmake folder and the executable
make re      # Recompile the project

# Then, to run the project, do :
./Scop [flags] <.obj file>

# You can display the help message using :
./Scop -h
```
## Informations

You have benchmark objects in the folder ./objects

You can get different error messages with different colors :
  - $${\color{cyan}Cyan}$$   : Tips, usually right after a warning/error to help remove it
  - $${\color{yellow}Yellow}$$ : Notice, information and error handled by the program
  - $${\color{orange}Orange}$$ : Warning, non-critical error
  - $${\color{red}Red}$$ : Critical error

#
<p align="center">
  <img src="skull_exemple.gif"/>
</p>
