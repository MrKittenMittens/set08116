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

// Incoming normal
layout(location = 1) in vec3 transformed_normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;
layout(location = 0) in vec3 vertexPosition;
// Outgoing colour
layout(location = 0) out vec4 colour;

//ball light calculations
vec4 calculate_balls(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_direction, in vec4 tex_colour, in vec3 eye_pos){

	// Get distance
  float dist = distance(point.position, position);

  // attenuation
  float k_att = point.constant + (point.linear * dist) + (point.quadratic * dist * dist);

  // Calculate light colour
  vec4 light_colour = point.light_colour / k_att;
  light_colour.a = 1.0f;

  // direction
  vec3 light_direction = normalize(point.position - position);
  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient

  // diffuse
  float kd = max(dot(normal, light_direction), 0.0);
  vec4 diffuse = kd * (mat.diffuse_reflection * light_colour);

  // Calculate view direction
  view_direction = normalize(eye_pos - position);

  // Calculate half vector
  vec3 H = normalize(light_direction + view_direction);

  // Calculate specular component
  float ks = pow(max(dot(H, normal), 0.0), mat.shininess);
  vec4 specular = ks * (light_colour * mat.specular_reflection);

  // Sample texture
  tex_colour = texture(tex, tex_coord);

  // Calculate primary colour component
  vec4 primary = mat.emissive + diffuse;
  vec4 secondary = specular;

  // Calculate final colour - remember alpha
  primary.a = 1;
  secondary.a = 1;
  vec4 return_colour = primary * tex_colour + secondary;

  return return_colour;
}

void main() {

  // *********************************
  // Calculate ambient component
  vec4 ambient = mat.diffuse_reflection * light.ambient_intensity;

  // Calculate diffuse component
  // Calculate k
  float diffuseK = max(dot(transformed_normal, light.light_dir),0.0);
  
  // Calculate diffuse
  vec4 diffuse = diffuseK * mat.diffuse_reflection * light.light_colour;

  // Calculate view direction
  vec3 view_dir = normalize(eye_pos-vertexPosition);
  
  // Calculate half vector between view_dir and light_dir
  vec3 H = normalize(light.light_dir+view_dir);

  // Calculate specular component
  // Calculate k
  float specularK = pow(max(dot(transformed_normal, H),0.0),mat.shininess);
  
  // Calculate specular
  vec4 specular = specularK * (mat.specular_reflection * light.light_colour);

  // Sample texture
  vec4 tex_colour = texture(tex, tex_coord);

  // Calculate primary colour component
  // Set primary
  vec4 primary = mat.emissive + ambient + diffuse;
  
  // Set secondary
  vec4 secondary = specular;

  // Calculate final colour - remember alpha
  primary.a = 1.0;
  secondary.a = 1.0;
  //point lights
  for (int i = 0; i < 3; i++)
  {
	colour += calculate_balls(points[i], mat, position, normal, view_direction, texture_colour, eye_pos);
  }
   colour = primary*tex_colour + secondary;
  // *********************************
}
