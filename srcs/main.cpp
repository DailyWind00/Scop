#include "config.hpp"

bool VERBOSE = false;
bool RESIZABLE = false;
bool WIREFRAME = false;

static void	displayHelp(char *executable_name) {
	cout << BGreen <<"=== 42 Scop by DailyWind ===\n" << ResetColor;
	cout << "Usage: " << executable_name<< " [options] <.obj file>\n";
	cout << "Options:\n";
	cout << "\t-v, --verbose\t\tVerbose mode\n";
	cout << "\t-r, --resizable\t\tResizable window\n";
	cout << "\t-w, --wireframe\t\tWireframe mode\n";
	cout << "\t-h, --help\t\tDisplay this information\n";
}

// return the number of flags
static int	checkFlags(int argc, char **argv) {
	int i;
	int flags = 0;

	for (i = 1; i < argc; i++) {
		string arg(argv[i]);
		if (arg[0] != '-' && arg.substr(0, 2) != "--" && i != argc - 1) {
			cerr << BRed << "Not a flag : " << arg << ResetColor << '\n';
			displayHelp(argv[0]);
			exit(EXIT_FAILURE);
		}

		if (arg == "-v" || arg == "--verbose") {
			VERBOSE = true;
			flags++;
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
			if (i == argc - 1) // object file
				return flags;
			cerr << BRed << "Unknown flag : " << arg << ResetColor << '\n';
			exit(EXIT_FAILURE);
		}
	}

	return flags;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		displayHelp(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if (checkFlags(argc, argv) == argc - 1) {
		cerr << BRed << "No file specified\n" << ResetColor;
		displayHelp(argv[0]);
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
		std::cerr << BRed <<  "Critical Error : " << e.what() << ResetColor <<'\n';
		exit(EXIT_FAILURE);
	}
}