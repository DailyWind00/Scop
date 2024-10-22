#include "config.hpp"

// Display the help message
void	displayHelp(char *executable_name) {
	cout << BGreen <<"=== 42 Scop by DailyWind ===\n" << ResetColor;
	cout << "Usage: " << executable_name<< " [options] <.obj file>\n";
	cout << "Options:\n";
	cout << "\t-v, --verbose\t\t\tVerbose mode\n";
	cout << "\t-A, --autorotate [option]\tAutorotate the object (desactivate camera control)\n";
	cout << LightGray << "\t\t│ > Options availables :                   │\n" << ResetColor;
	cout << LightGray << "\t\t│ x, pitch\t: autorotate around x axis │\n" << ResetColor;
	cout << LightGray << "\t\t│ y, yaw\t: autorotate around y axis │\n" << ResetColor;
	cout << LightGray << "\t\t│ z, roll\t: autorotate around z axis │\n" << ResetColor;
	cout << LightGray << "\t\t╰──────────────────────────────────────────╯\n" << ResetColor;
	cout << "\t-N, --inverse-autorotate\tInverse the autorotation\n";
	cout << "\t-r, --resizable\t\t\tResizable window\n";
	cout << "\t-w, --wireframe\t\t\tWireframe mode (can be changed during execution)\n";
	cout << "\t-k, --keyboard [option]\t\tChange the keyboard language\n";
	cout << LightGray << "\t\t│ > Options availables :                     │\n" << ResetColor;
	cout << LightGray << "\t\t│ azerty\t: set the keyboard to azerty │\n" << ResetColor;
	cout << LightGray << "\t\t│ qwerty\t: set the keyboard to qwerty │\n" << ResetColor;
	cout << LightGray << "\t\t╰────────────────────────────────────────────╯\n" << ResetColor;
	cout << "\t    --no-tooltip\t\tDisable the commands tooltip in the standard output\n";
	cout << "\t-h, --help\t\t\tDisplay this information\n";
}

// Display the commands available in the program
void	displayCommands() {
	if (!SHOW_TOOLTIP)
		return;

	cout << BLightBlue << "=== Commands ===\n" << ResetColor;
	if (AUTOROTATE == ROTATION::NONE) {
		cout << "> Object rotation :\n";
		if (KEYBOARD == KEYBOARD_LANGUAGE::AZERTY) {
			cout << "Q/D\t\t\t: Rotate the object on the Pitch axis\n";
			cout << "Z/S\t\t\t: Rotate the object on the Yaw axis\n";
			cout << "A/E\t\t\t: Rotate the object on the Roll axis\n";
		} else {
			cout << "A/D\t\t\t: Rotate the object on the Pitch axis\n";
			cout << "W/S\t\t\t: Rotate the object on the Yaw axis\n";
			cout << "Q/E\t\t\t: Rotate the object on the Roll axis\n";
		}
		cout << endl;
		cout << "> Others commands :\n";
	}
	cout << "F1\t\t\t: Enable/Disable wireframe mode\n";
	cout << "F5\t\t\t: Recompile the current shader\n";
	cout << "Left/Right arrows\t: Change shader\n";
	cout << "Scroll\t\t\t: Zoom in/out\n";
	cout << "ESC\t\t\t: Close the window\n";
	printVerbose((string)BLightBlue + "================" + ResetColor);
}

ifstream	openReadFile(const string &filename) {
	ifstream file(filename);
	if (!file.is_open())
		throw runtime_error("Failed to open file " + filename + (string)strerror(errno));
	return file;
}

// Print the message on the standard output if VERBOSE is set to true
void	printVerbose(const string &message, bool newline) {
	if (VERBOSE) {
		cout << message;
		if (newline)
			cout << endl;
	}
}