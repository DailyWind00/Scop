#include "config.hpp"

bool VERBOSE = false;
bool RESIZABLE = false;
bool WIREFRAME = false;
ROTATION AUTOROTATE = ROTATION::NONE;
bool INVERSE_AUTOROTATE = false;

static int setflagVerbose() {
	VERBOSE = true;
	return 1;
}

static int setflagAutorotate(string &arg, int &i, int argc, char **argv) {
	if (i == argc - 1)
		throw runtime_error("No autorotate argument");
	arg = argv[++i];
	if (arg == "pitch" || arg == "x")
		AUTOROTATE = ROTATION::PITCH;
	else if (arg == "yaw" || arg == "y")
		AUTOROTATE = ROTATION::YAW;
	else if (arg == "roll" || arg == "z")
		AUTOROTATE = ROTATION::ROLL;
	else
		throw runtime_error("Invalid autorotate argument");
	return 2;
}

static int setflagInverseAutorotate() {
	if (AUTOROTATE == ROTATION::NONE)
		cout << BRed << "Flag error : Autorotate not set : flag ignored" << ResetColor << endl;
	else
		INVERSE_AUTOROTATE = true;
	return 1;
}

static int setflagResizable() {
	RESIZABLE = true;
	return 1;
}

static int setflagWireframe() {
	WIREFRAME = true;
	return 1;
}

// return the number of flags
int	checkFlags(int argc, char **argv) {
	int i;
	int flags = 0;

	for (i = 1; i < argc; i++) {
		string arg(argv[i]);
		if (arg[0] != '-' && arg.substr(0, 2) != "--" && i != argc - 1) {
			throw runtime_error("Not a flag \"" + arg + "\"");
		}

		if (arg == "-v" || arg == "--verbose")
			flags += setflagVerbose();

		else if (arg == "-A" || arg == "--autorotate")
			flags += setflagAutorotate(arg, i, argc, argv);

		else if (arg == "-N" || arg == "--inverse-autorotate")
			flags += setflagInverseAutorotate();

		else if (arg == "-r" || arg == "--resizable")
			flags += setflagResizable();

		else if (arg == "-w" || arg == "--wireframe")
			flags += setflagWireframe();

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