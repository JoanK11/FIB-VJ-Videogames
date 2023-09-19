#version 330

in vec2 position;
uniform vec2 scale;

void main() {
        vec2 new_position = position * scale;
	gl_Position = vec4(new_position, 0.0, 1.0);
}
