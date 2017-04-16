#version 440

// Incoming texture containing frame information
uniform sampler2D tex;

// Our colour filter - calculates colour intensity
const vec3 intensity = vec3(0.299, 0.587, 0.184);

// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Sample texture colour
  vec4 tex_colour = texture(tex, tex_coord);
  // Calculate grey value
  float i = dot(intensity, vec3(tex_colour).xyz);
  // Use greyscale to as final colour
  // - ensure alpha is 1
  colour = vec4(i,i,i,1.0);
  colour += vec4(0.314,0.169,-0.090,1.0); // sepia
  //colour += vec4(0.2, 0.0, 0.2, 1.0); 
  colour.a = 1.0;
  // *********************************
}