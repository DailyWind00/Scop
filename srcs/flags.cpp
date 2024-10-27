#include "config.hpp"

bool VERBOSE = false;
bool RESIZABLE = false;
bool WIREFRAME = false;
float SPEED = 1.0f;
ROTATION AUTOROTATE = ROTATION::NONE;
bool INVERSE_AUTOROTATE = false;
KEYBOARD_LANGUAGE KEYBOARD = KEYBOARD_LANGUAGE::QWERTY;
bool SHOW_TOOLTIP = true;

static int setflagVerbose(bool value) {
	VERBOSE = value;
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

static int setflagInverseAutorotate(bool value) {
	if (AUTOROTATE == ROTATION::NONE)
		cout << BYellow << "Notice : Autorotate not set : flag ignored" << ResetColor << endl;
	else
		INVERSE_AUTOROTATE = value;
	return 1;
}

static int setflagSpeed(int &i, int argc, char **argv) {
	if (i == argc - 1)
		throw runtime_error("No speed argument");

	stringstream ss(argv[++i]);
	ss >> SPEED;

	if (!ss)
		throw runtime_error("Invalid speed argument");
	if (SPEED <= 0)
		throw runtime_error("Speed must be greater than 0");
	
	return 2;
}

static int setflagResizable(bool value) {
	RESIZABLE = value;
	return 1;
}

static int setflagWireframe(bool value) {
	WIREFRAME = value;
	return 1;
}

static int setflagShowTooltip(bool value) {
	SHOW_TOOLTIP = value;
	return 1;
}

static int setflagKeyboardLanguage(string &arg, int &i, int argc, char **argv) {
	if (i == argc - 1)
		throw runtime_error("No keyboard language argument");

	arg = argv[++i];
	if (arg == "azerty")
		KEYBOARD = KEYBOARD_LANGUAGE::AZERTY;
	else if (arg == "qwerty")
		KEYBOARD = KEYBOARD_LANGUAGE::QWERTY;
	else
		throw runtime_error("Invalid keyboard language argument");

	return 2;
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
			flags += setflagVerbose(true);

		else if (arg == "-A" || arg == "--autorotate")
			flags += setflagAutorotate(arg, i, argc, argv);

		else if (arg == "-N" || arg == "--inverse-autorotate")
			flags += setflagInverseAutorotate(true);

		else if (arg == "-s" || arg == "--speed")
			flags += setflagSpeed(i, argc, argv);

		else if (arg == "-r" || arg == "--resizable")
			flags += setflagResizable(true);

		else if (arg == "-w" || arg == "--wireframe")
			flags += setflagWireframe(true);

		else if (arg == "-k" || arg == "--keyboard")
			flags += setflagKeyboardLanguage(arg, i, argc, argv);

		else if (arg == "--no-tooltip")
			flags += setflagShowTooltip(false);

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