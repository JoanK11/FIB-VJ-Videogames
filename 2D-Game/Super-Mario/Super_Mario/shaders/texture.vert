#version 330

uniform mat4 projection, modelview;
uniform vec2 texCoordDispl;
uniform mat4 viewMatrix;
in vec2 position;
in vec2 texCoord;
out vec2 texCoordFrag;
out vec4 modelPos;
void main()
{
	// Pass texture coordinates to access a given texture atlas
	texCoordFrag = texCoord + texCoordDispl;
	// Transform position from pixel coordinates to clipping coordinates
	modelPos = modelview * vec4(position, 0.0, 1.0);
	gl_Position = projection *viewMatrix* modelview * vec4(position, 0.0, 1.0);
}

