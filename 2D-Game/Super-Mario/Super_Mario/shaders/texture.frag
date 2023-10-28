#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform float min_height;
in vec2 texCoordFrag;
out vec4 outColor;
in vec4 modelPos;

void main()
{
	// Discard fragment if texture sample has alpha < 0.5
	// otherwise compose the texture sample with the fragment's interpolated color
	if(min_height < modelPos.y) discard;
	vec4 texColor = texture(tex, texCoordFrag);
	if(texColor.a < 0.5f)
	texColor = vec4(0.,1,0.,1.);
	//	discard;
	outColor = color * texColor;
}

