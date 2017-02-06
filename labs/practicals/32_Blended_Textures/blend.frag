#version 440

// Main textures
uniform sampler2D tex[2];
// Blend map
uniform sampler2D blend;

// Incoming texture coordinate
layout(location = 0) in vec2 tex_coord;
// Outgoing fragment colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Sample the two main textures
	tex[0] = texture(grass, tex_coord);
	tex[1] = texture(stonygrass, tex_coord);
  // Sample the blend texture
  blend = texture(blend_map1, tex_coord);
  // Mix the main samples using r component from blend value
  colour = mix(tex[0], tex[1], 0.5);
  // *********************************
}