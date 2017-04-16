#version 440

//declare M
uniform mat4 M;
//declare Transformation matrix
uniform mat4 MVP;
// declare normal matrix
uniform mat3 N;
//uv scroll
uniform vec2 UV_SCROLL;

// Incoming position
layout(location = 0) in vec3 pos_in;

// Incoming normal
layout(location = 2) in vec3 normal_in;

// Incoming texture coordinate
layout(location = 10) in vec2 tex_coord_in;

// Outgoing position
layout(location = 0) out vec3 position;

// Outgoing transformed normal
layout(location = 1) out vec3 normal;

// Outgoing texture coordinate
layout(location = 2) out vec2 tex_coord;

void main() {
  // Calculate screen position
  gl_Position = MVP * vec4(pos_in, 1.0);

  // Output values to fragment shader
	position = vec3(M * vec4(pos_in, 1.0f));
  normal = N * normal_in;
  tex_coord = tex_coord_in + UV_SCROLL;



}