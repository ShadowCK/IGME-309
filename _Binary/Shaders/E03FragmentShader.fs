#version 330

// Input variables
in vec3 Color;

// Uniform variables
uniform vec3 SolidColor = vec3(-1, -1, -1);
uniform bool Complimentary = false;

// Output variable
out vec4 fragmentColor;

void main()
{
    // Initialize the output color to the input color
    vec3 outputColor = Color;

    // Override the color if the SolidColor uniform is set
    if (SolidColor.r != -1.0 && SolidColor.g != -1.0 && SolidColor.b != -1.0)
    {
        outputColor = SolidColor;
    }

    // Invert the color if the complimentary uniform is set
    if (Complimentary == true)
    {
        outputColor = vec3(1.0f, 1.0f, 1.0f) - outputColor;
    }

    // Assign the final color to the output variable
    fragmentColor = vec4(outputColor, 1);
} 
