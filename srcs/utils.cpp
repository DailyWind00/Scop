#include "config.hpp"

// Display the help message
void	displayHelp(char *executable_name) {
	cout << BGreen <<"=== 42 Scop by DailyWind ===\n" << ResetColor;
	cout << "Usage: " << executable_name<< " [options] <.obj file>\n";
	cout << "Options:\n";
	cout << "\t-v, --verbose\t\t\tVerbose mode\n";
	cout << "\t-A, --autorotate [option]\tAutorotate the object (desactivate camera control)\n";
	cout << LightGray << "\t\t│ Options availables	 :\n" << ResetColor;
	cout << LightGray << "\t\t│ x, pitch\t: autorotate around x axis\n" << ResetColor;
	cout << LightGray << "\t\t│ y, yaw\t: autorotate around y axis\n" << ResetColor;
	cout << LightGray << "\t\t│ z, roll\t: autorotate around z axis\n" << ResetColor;
	cout << "\t-r, --resizable\t\t\tResizable window\n";
	cout << "\t-w, --wireframe\t\t\tWireframe mode (can be changed during execution)\n";
	cout << "\t-h, --help\t\t\tDisplay this information\n";
}

// Display the commands available in the program
void	displayCommands() {
	cout << BLightBlue << "=== Commands ===\n" << ResetColor;
	cout << "ESC\t\t\t: Close the window\n";
	cout << "F1\t\t\t: Enable/Disable wireframe mode\n";
	cout << "Left/Right arrows\t: Change shader\n";
	printVerbose((string)BLightBlue + "================" + ResetColor);
}

// Print the message on the standard output if VERBOSE is set to true
void	printVerbose(const string &message, bool newline) {
	if (VERBOSE) {
		cout << message;
		if (newline)
			cout << endl;
	}
}