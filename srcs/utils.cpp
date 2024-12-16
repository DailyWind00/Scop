#include "config.hpp"

// Display the help message
void	displayHelp(char *executable_name) {
	cout << BGreen <<"=== 42 Scop by DailyWind ===\n" << ResetColor;
	cout << "Usage: " << executable_name<< " [options] <.obj file>\n";
	cout << "Options:\n";
	cout << "\t-v, --verbose\t\t\tVerbose mode\n";
	cout << "\t-A, --autorotate [option]\tAutorotate the object (desactivate camera control)\n";
	cout << LightGray << "\t\t│ > Options availables :                   │\n" << ResetColor;
	cout << LightGray << "\t\t│ x, pitch : autorotate around x axis      │\n" << ResetColor;
	cout << LightGray << "\t\t│ y, yaw   : autorotate around y axis      │\n" << ResetColor;
	cout << LightGray << "\t\t│ z, roll  : autorotate around z axis      │\n" << ResetColor;
	cout << LightGray << "\t\t╰──────────────────────────────────────────╯\n" << ResetColor;
	cout << "\t-N, --inverse-autorotate\tInverse the autorotation\n";
	cout << "\t-s, --speed [value]\t\tSet the rotation speed\n";
	cout << "\t-r, --resizable\t\t\tResizable window\n";
	cout << "\t-w, --wireframe\t\t\tWireframe mode (can be changed during execution)\n";
	cout << "\t-k, --keyboard [option]\t\tChange the keyboard language\n";
	cout << LightGray << "\t\t│ > Options availables :                     │\n" << ResetColor;
	cout << LightGray << "\t\t│ azerty : set the keyboard to azerty        │\n" << ResetColor;
	cout << LightGray << "\t\t│ qwerty : set the keyboard to qwerty        │\n" << ResetColor;
	cout << LightGray << "\t\t╰────────────────────────────────────────────╯\n" << ResetColor;
	cout << "\t    --no-tooltip\t\tDisable the commands tooltip in the standard output\n";
	cout << "\t-h, --help\t\t\tDisplay this information\n";
}

// Display the commands available in the program
void	displayCommands() {
	if (!SHOW_TOOLTIP)
		return;

	cout << BLightBlue << "=== Commands ===\n" << ResetColor;
	cout << "> Object translation :\n";
	cout << " J/L\t\t\t: Translate the object on the X axis\n";
	cout << " I/K\t\t\t: Translate the object on the Y axis\n";
	cout << " U/O\t\t\t: Translate the object on the Z axis\n";
	cout << endl;
	if (AUTOROTATE == ROTATION::NONE) {
		cout << "> Object rotation :\n";
		if (KEYBOARD == KEYBOARD_LANGUAGE::AZERTY) {
			cout << " Z/S\t\t\t: Rotate the object on the Pitch axis\n";
			cout << " Q/D\t\t\t: Rotate the object on the Yaw axis\n";
			cout << " A/E\t\t\t: Rotate the object on the Roll axis\n";
		} else {
			cout << " W/S\t\t\t: Rotate the object on the Pitch axis\n";
			cout << " A/D\t\t\t: Rotate the object on the Yaw axis\n";
			cout << " Q/E\t\t\t: Rotate the object on the Roll axis\n";
		}
		cout << endl;
	}
	cout << "> Others commands :\n";
	cout << " F1\t\t\t: Enable/Disable wireframe mode\n";
	cout << " F5\t\t\t: Recompile the current shader\n";
	cout << " Space\t\t\t: Reset the object\n";
	cout << " Scroll\t\t\t: Scale the object\n";
	cout << " Up/Down arrows\t\t: Switch rendering method (texture / vertex)\n";
	cout << " Left/Right arrows\t: Change shader\n";
	cout << " ESC\t\t\t: Close the window\n";
	printVerbose(BLightBlue + "================" + ResetColor);
}

// Open the file in read mode and return the ifstream object, throw an error if the file can't be opened
ifstream	openReadFile(const string &filename) {
	ifstream file(filename);
	if (!file.is_open())
		throw runtime_error("Failed to open file : " + (string)strerror(errno));
	return file;
}

// Load the texture using stb_image and return the data, throw an error if the texture can't be loaded
unsigned char *stbi_loader(const string &filename, int &width, int &height, int &nrChannels) {
	printVerbose("> Loading texture " + filename + " -> ", false);
	stbi_set_flip_vertically_on_load(true); // Using OpenGL coordinate system

	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, STBI_default);
	if (!data) {
		printVerbose(BRed + "Error" + ResetColor);
		return nullptr;
	}

	printVerbose(BGreen + "Texture loaded" + ResetColor);
	return data;
}

// Print the message on the standard output if VERBOSE is set to true
void	printVerbose(const string &message, bool newline) {
	if (VERBOSE) {
		cout << message;
		if (newline)
			cout << endl;
	}
}

// Clamp the value between min and max
float	clamp(float value, float min, float max) {
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

// Convert degrees to radians
float	toRadians(float degrees) {
	return degrees * M_PI / 180;
}