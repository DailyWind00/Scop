#version 400 core

in vec3 fragmentColor;

out vec4 screenColor;

// Function to adjust colors for Deuteranopia
vec3 adjustForDeuteranopia(vec3 color) {
    float newR = 0.625 * color.r + 0.375 * color.g;  // Shift red towards green
    float newG = 0.7 * color.g + 0.3 * color.b;      // Shift green towards blue
    float newB = color.b;                            // Blue stays unchanged
    
    return vec3(newR, newG, newB);  // Return the adjusted color
}

void main() {
    // Adjust the fragment color for Deuteranopia
    vec3 colorBlindFriendlyColor = adjustForDeuteranopia(fragmentColor);

    // Output the adjusted color
    screenColor = vec4(colorBlindFriendlyColor, 1.0);
}