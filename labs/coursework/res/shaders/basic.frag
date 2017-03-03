#version 440

// Sampler used to get texture colour
uniform sampler2D tex;

//point light info
struct point_light{
	vec4 light_colour;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

//Material info
struct material{
	vec4 emissive;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};

//ball lights
uniform point_light points[3];
//material of objects
uniform material mat;
//eye pos
uniform vec3 eye_pos;

//incoming position
layout(location = 0) in vec3 position;

//normal
layout(location = 1) in vec3 normal;

//incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Incoming primary colour
layout(location = 3) in vec4 primary;

// Incoming secondary colour
layout(location = 4) in vec4 secondary;


//outgoing colour
layout(location = 0) out vec4 colour;


//ball light calculations
vec4 calculate_balls(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour, in vec3 eye_pos){
	// Get distance between point light and vertex
  float d = distance(point.position, position);
  // Calculate attenuation factor
  float k_att = point.constant + (point.linear * d) + (point.quadratic * d * d);
  // Calculate light colour
  vec4 light_colour = point.light_colour / k_att;
  light_colour.a = 1.0f;
  // Calculate light dir
  vec3 light_dir = normalize(point.position - position);
  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient

  // Calculate diffuse component
  float kd = max(dot(normal, light_dir), 0.0);
  vec4 diffuse = kd * (mat.diffuse_reflection * light_colour);

  // Calculate view direction
  view_dir = normalize(eye_pos - position);

  // Calculate half vector
  vec3 H = normalize(light_dir + view_dir);

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
  //define colour
  colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);


  //calculate view direction and texture
  vec3 view_dir = normalize(eye_pos - position);
  vec4 texture_colour = texture(tex, tex_coord);
  //point lights
  for (int i = 0; i < 3; i++)
  {
	colour += calculate_balls(points[i], mat, position, normal, view_dir, texture_colour, eye_pos);
  }
}