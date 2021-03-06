#version 440

// A directional light structure
struct directional_light {
  vec4 ambient_intensity;
  vec4 light_colour;
  vec3 light_dir;
};

// A material structure
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// Directional light for the scene
uniform directional_light light;
// Material of the object
uniform material mat;
// Position of the camera
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;

// Incoming position
layout(location = 0) in vec3 position;
// Incoming normal
layout(location = 1) in vec3 normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {

  // *********************************
  // Calculate ambient component
  vec4 ambient = light.ambient_intensity * mat.diffuse_reflection;
  // Calculate diffuse component
  float kd = max(dot(transformed_normal, light.light_dir), 0.0);
  vec4 diffuse = kd * (mat.diffuse_reflection * light.light_colour);
  // Calculate view direction
  vec3 world_pos = vec3(M * vec4(position, 1.0));
  vec3 view_dir = normalize(vec3(eye_pos - world_pos));
  // Calculate half vector
  vec3 H = normalize(light.light_dir + view_dir);
  // Calculate specular component
  float ks = pow(max(dot(H, transformed_normal), 0.0), mat.shininess);
  vec4 specular = ks * (light.light_colour * mat.specular_reflection);
  // Sample texture
  vec4 tex_colour = texture(tex, tex_coord);
  // Calculate primary colour component
  primary = ambient + diffuse;
  secondary = specular;
  // Calculate final colour - remember alpha
  primary.a = 1;
  secondary.a = 1;
  colour = primary * tex_colour + secondary;
  // *********************************
}