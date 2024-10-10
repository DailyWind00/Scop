#version 400 core

in vec3 fragmentColor;

out vec4 screenColor;

// Function to adjust colors for Protanopia
vec3 adjustForProtanopia(vec3 color) {
    float newR = 0.56667 * color.r + 0.43333 * color.g;  // Shift red heavily towards green
    float newG = 0.55833 * color.g + 0.44167 * color.b;  // Shift green towards blue
    float newB = color.b;                                // Blue remains unchanged
    
    return vec3(newR, newG, newB);  // Return the adjusted color
}

void main() {
    // Adjust the fragment color for Protanopia
    vec3 colorBlindFriendlyColor = adjustForProtanopia(fragmentColor);

    // Output the adjusted color
    screenColor = vec4(colorBlindFriendlyColor, 1.0);
}