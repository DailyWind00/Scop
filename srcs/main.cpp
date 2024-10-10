#include "config.hpp"

bool VERBOSE = false;
bool RESIZABLE = false;
bool WIREFRAME = false;
char AUTOROTATE = NONE;

// return the number of flags
static int	checkFlags(int argc, char **argv) {
	int i;
	int flags = 0;

	for (i = 1; i < argc; i++) {
		string arg(argv[i]);
		if (arg[0] != '-' && arg.substr(0, 2) != "--" && i != argc - 1) {
			throw runtime_error("Not a flag \"" + arg + "\"");
		}

		if (arg == "-v" || arg == "--verbose") {
			VERBOSE = true;
			flags++;
		}
		else if (arg == "-A" || arg == "--autorotate") {
			if (i == argc - 1)
				throw runtime_error("No autorotate argument");
			arg = argv[++i];
			if (arg == "pitch" || arg == "x")
				AUTOROTATE = PITCH;
			else if (arg == "yaw" || arg == "y")
				AUTOROTATE = YAW;
			else if (arg == "roll" || arg == "z")
				AUTOROTATE = ROLL;
			else
				throw runtime_error("Invalid autorotate argument");
			flags += 2;
		}
		else if (arg == "-r" || arg == "--resizable") {
			RESIZABLE = true;
			flags++;
		}
		else if (arg == "-w" || arg == "--wireframe") {
			WIREFRAME = true;
			flags++;
		}
		else if (arg == "-h" || arg == "--help") {
			displayHelp(argv[0]);
			exit(EXIT_SUCCESS);
		}
		else {
			if (i == argc - 1 && arg[0] != '-') // object file
				return flags;
			throw runtime_error("Unknown flag \"" + arg + "\"");
		}
	}

	return flags;
}

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