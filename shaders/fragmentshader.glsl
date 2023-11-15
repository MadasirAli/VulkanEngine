# version 450

layout (location = 0) out outColor;
layout (location = 0) in fragColor;

void main()
{
	outColor = vec4(fragColor, 1.0);
}