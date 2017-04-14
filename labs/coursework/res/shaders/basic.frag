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

// Spot light info
struct spot_light {
  vec4 light_colour;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float power;
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
//spotlight
uniform spot_light spotlight;
//material of objects
uniform material mat;
//eye pos
uniform vec3 eye_pos;
uniform bool greyscale;

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



//spotlight calculation
vec4 calculate_spotlight(in spot_light spotlight, in material mat, in vec3 position, in vec3 normal, in vec3 view_direction, in vec4 tex_colour)
{
	//Calculate light direction
	vec3 light_direction = normalize(spotlight.position - position);

	//calculate distance to the light
	float dist = distance(spotlight.position, position);

	//attenuation
	float k_attenuation = spotlight.constant + (spotlight.linear * dist) + (spotlight.quadratic * dist * dist);

	//intensity
	float intensity = pow(max(dot(-1.0f * spotlight.direction, light_direction), 0.0f), spotlight.power);

	//colour
	vec4 light_colour = (intensity/k_attenuation) * spotlight.light_colour;

	//phong shading again
	float k_diffuse = max(dot(normal, light_direction), 0.0f);
	vec4 diffuse = k_diffuse * (mat.diffuse_reflection * light_colour);

	//half vector
	vec3 half_vec = normalize(light_direction + view_direction);

	//specular
	float k_specular = pow(max(dot(half_vec, normal), 0.0f), mat.shininess);
	vec4 specular = k_specular * (light_colour * mat.specular_reflection);

	//primary/secondary
	vec4 primary = mat.emissive + diffuse;
	vec4 secondary = specular;
	//ensure alpha is 1
	primary.a = 1;
	secondary.a = 1;
	//return colour
	vec4 return_colour = primary * tex_colour + secondary;
	return return_colour;
}



void main() {
  //define colour
  colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);


  //calculate view direction and texture
  vec3 view_direction = normalize(eye_pos - position);
  vec4 texture_colour = texture(tex, tex_coord);
  //point lights
  for (int i = 0; i < 3; i++)
  {
	colour += calculate_balls(points[i], mat, position, normal, view_direction, texture_colour, eye_pos);
  }
  colour += calculate_spotlight(spotlight, mat, position, normal, view_direction, texture_colour);






}