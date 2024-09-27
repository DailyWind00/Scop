#include "config.hpp"
#include "OBJ.hpp"
#include "color.h"

using namespace std;

bool VERBOSE = false;

void	displayHelp(char *executable_name) {
	cout << "Usage: " << executable_name<< " [options] <.obj file>\n";
	cout << "Options:\n";
	cout << "\t-v, --verbose\t\tVerbose mode\n";
	cout << "\t-h, --help\t\tDisplay this information\n";
}

// return the number of flags
int	checkFlags(int argc, char **argv) {
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
		else if (arg == "-h" || arg == "--help") {
			displayHelp(argv[0]);
			exit(EXIT_SUCCESS);
		}
		else {
			if (i == argc - 1)
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
		cout << "File " << file_name << " loaded successfully\n";
	}
	catch(const std::exception& e) {
		std::cerr << BRed <<  "Critical Error : " << e.what() << ResetColor <<'\n';
		exit(EXIT_FAILURE);
	}
}