#version 400 core

in vec3 fragmentColor;

out vec4 screenColor;

// Function to adjust colors for Tritanopia
vec3 adjustForTritanopia(vec3 color) {
    float newR = 0.95 * color.r + 0.05 * color.g;  // Slight shift in red channel
    float newG = 0.0 * color.r + 0.43 * color.g + 0.57 * color.b;  // Major adjustment in green channel
    float newB = 0.0 * color.r + 0.475 * color.g + 0.525 * color.b;  // Small shift in blue channel
    
    return vec3(newR, newG, newB);  // Return the adjusted color
}

void main() {
    // Adjust the fragment color for Tritanopia
    vec3 colorBlindFriendlyColor = adjustForTritanopia(fragmentColor);

    // Output the adjusted color
    screenColor = vec4(colorBlindFriendlyColor, 1.0);
}