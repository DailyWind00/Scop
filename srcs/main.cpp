#include "config.hpp"

int main(int argc, char **argv) {
	if (argc < 2) {
		cout << "> Run the program with the -h flag to display the help message\n";
		exit(EXIT_FAILURE);
	}

	try {
		if (checkFlags(argc, argv) == argc - 1) {
			cerr << BRed << "No file specified\n" << ResetColor;
			cout << "> Run the program with the -h flag to display the help message\n";
			exit(EXIT_FAILURE);
		}
	} catch(const std::exception& e) {
		cerr << BRed << "Flag error : " << e.what() << ResetColor << endl;
		cout << "> Run the program with the -h flag to display the help message\n";
		exit(EXIT_FAILURE);
	}

	const string file_name(argv[argc - 1]);

	try {
		OBJ obj(file_name);
		printVerbose("File " + file_name + " loaded successfully");

		GLFWwindow *window = CreateWindow(obj);
		RenderObject(window, obj);
		DestroyWindow(window);
	}
	catch(const std::exception& e) {
		cerr << BRed <<  "Critical Error : " << e.what() << ResetColor <<'\n';
		exit(EXIT_FAILURE);
	}
}