#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;


effect eff;
effect sbeff;
effect tableNorm;
effect mirrorEff;
effect geff;

std::array<map<string, mesh>, 6> meshes;
frame_buffer frame;
cubemap cube_map;
free_camera freeCam;
free_camera mirrorCam;
target_camera targetCam;
directional_light light;
vector <point_light> ball_lights(3);
spot_light spotlight;
bool cam_type = false;
float theta = 0.0f;
material mat;
bool greyscale = true;
bool built;

//Declare meshes for objects/furnature
map<string, mesh> table;
map<string, mesh> room;
map<string, mesh> chair;
map<string, mesh> shelf;
map<string, mesh> cupboard;
map<string, mesh> lamp;
map<string, mesh> mirror;
texture normalMap;
map<string, mesh> balls;
bool ball1_direction = true;
bool ball2_direction = false;
bool ball3_direction = true;
geometry screen_quad;
map<string, texture> textures;
double cursor_x = 0.0;
double cursor_y = 0.0;

bool initialise() {
	//dissable cursor, and capture position.
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	return true;
}

bool load_content() {


	// Create frame buffer - use screen width and height
	frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	// Create screen quad
	vector<vec3> positions{ vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f) };
	vector<vec2> tex_coords{ vec2(0.0f, 0.0f), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f) };
	screen_quad.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	screen_quad.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	screen_quad.set_type(GL_TRIANGLE_STRIP);



	// Create frame buffer - use screen width and height
	frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	//set materials

	mat.set_emissive(vec4(0.0f, 0.0f, 0.f, 1.0f));
	mat.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mat.set_shininess(5.0f);
	mat.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));

	//set light
	light.set_ambient_intensity(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	light.set_light_colour(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	light.set_direction(vec3(0.0f, 0.0f, 0.0f));


	//Building objects and meshes/textures
	{
		//load meshes for objects
		//Room
		{
			//Back wall
			room["wall_back_inner"] = mesh(geometry_builder::create_plane(20, 10, true));
			room["wall_back_inner"].get_transform().orientation = rotate(mat4(1.0f), half_pi<float>(), vec3(1.0f, 0.0f, 0.0f));
			room["wall_back_inner"].get_transform().position = vec3(-5.0f, 0.0f, -10.0f);
			room["wall_back_inner"].set_material(mat);
			textures["wall_back_inner"] = texture("textures/wallpaperBF.png");

			//Front wall
			room["wall_front_inner"] = mesh(geometry_builder::create_plane(20, 10, true));
			room["wall_front_inner"].get_transform().orientation = rotate(mat4(1.0f), (half_pi<float>() + pi<float>()), vec3(1.0f, 0.0f, 0.0f));
			room["wall_front_inner"].get_transform().position = vec3(-5.0f, 0.0f, 10.0f);
			room["wall_front_inner"].set_material(mat);
			textures["wall_front_inner"] = texture("textures/wallpaperBF.png");

			//Right wall
			room["wall_left_inner"] = mesh(geometry_builder::create_plane(10, 20, true));
			room["wall_left_inner"].get_transform().position = vec3(-10.0f, 2.5f, 7.5f);
			room["wall_left_inner"].get_transform().orientation = rotate(mat4(1.0f), (half_pi<float>() + pi<float>()), vec3(0.0f, 0.0f, 1.0f));
			room["wall_left_inner"].set_material(mat);
			textures["wall_left_inner"] = texture("textures/wallpaperLR.png");

			//Left wall
			room["wall_right_inner"] = mesh(geometry_builder::create_plane(10, 20, true));
			room["wall_right_inner"].get_transform().position = vec3(10.0f, -2.5f, 7.5f);
			room["wall_right_inner"].get_transform().orientation = rotate(mat4(1.0f), half_pi<float>(), vec3(0.0f, 0.0f, 1.0f));
			room["wall_right_inner"].set_material(mat);
			textures["wall_right_inner"] = texture("textures/wallpaperLR.png");

			//floor
			room["floor"] = mesh(geometry_builder::create_plane(20, 20));
			room["floor"].get_transform().position = vec3(0.0f, -5.0f, 0.0f);
			room["wall_back_inner"].set_material(mat);
			textures["floor"] = texture("textures/floor.jpg");
		}

		//Load meshes for table
		{
			mat.set_shininess(25.0f);
			//front left leg of table
			table["table_front_left"] = mesh(geometry_builder::create_box(vec3(0.5f, 3.5f, 0.5f)));
			table["table_front_left"].get_transform().position = vec3(6.5f, -3.25f, 0.0f);
			textures["table_front_left"] = texture("textures/wood.jpg");

			//front right leg of table
			table["table_front_right"] = mesh(geometry_builder::create_box(vec3(0.5f, 3.5f, 0.5f)));
			table["table_front_right"].get_transform().position = vec3(6.5f, -3.25f, 9.75f);
			textures["table_front_right"] = texture("textures/wood.jpg");

			//back left leg of table
			table["table_back_left"] = mesh(geometry_builder::create_box(vec3(0.5f, 3.5f, 0.5f)));
			table["table_back_left"].get_transform().position = vec3(9.75f, -3.25f, 0.0f);
			textures["table_back_left"] = texture("textures/wood.jpg");

			//back right leg of table
			table["table_back_right"] = mesh(geometry_builder::create_box(vec3(0.5f, 3.5f, 0.5f)));
			table["table_back_right"].get_transform().position = vec3(9.75f, -3.25f, 9.75f);
			textures["table_back_right"] = texture("textures/wood.jpg");

			//table top
			table["table_top"] = mesh(geometry_builder::create_box(vec3(3.75f, 0.25f, 10.25f)));
			table["table_top"].get_transform().position = vec3(8.125f, -1.375f, 4.875f);
			textures["table_top"] = texture("textures/wood.jpg");
		}

		//Load meshes for chair
		{
			//front left leg of chair
			chair["chair_front_left"] = mesh(geometry_builder::create_box(vec3(0.25f, 2.0f, 0.25f)));
			chair["chair_front_left"].get_transform().position = vec3(7.375f, -4.0f, 4.125f);
			textures["chair_front_left"] = texture("textures/woodSeat.png");

			//front right leg of chair
			chair["chair_front_right"] = mesh(geometry_builder::create_box(vec3(0.25f, 2.0f, 0.25f)));
			chair["chair_front_right"].get_transform().position = vec3(7.375f, -4.0f, 5.875f);
			textures["chair_front_right"] = texture("textures/woodSeat.png");

			//back left of chair
			chair["chair_back_left"] = mesh(geometry_builder::create_box(vec3(0.25f, 2.0f, 0.25f)));
			chair["chair_back_left"].get_transform().position = vec3(5.625f, -4.0f, 4.125f);
			textures["chair_back_left"] = texture("textures/woodSeat.png");

			//back right of chair
			chair["chair_back_right"] = mesh(geometry_builder::create_box(vec3(0.25f, 2.0f, 0.25f)));
			chair["chair_back_right"].get_transform().position = vec3(5.625f, -4.0f, 5.875f);
			textures["chair_back_right"] = texture("textures/woodSeat.png");

			//Seat of the chair
			chair["chair_seat"] = mesh(geometry_builder::create_box(vec3(2.0f, 0.25f, 2.0f)));
			chair["chair_seat"].get_transform().position = vec3(6.5f, -2.875f, 5.0f);
			textures["chair_seat"] = texture("textures/woodSeat.png");

			//back of the chair
			//back left flat
			chair["chair_side_left"] = mesh(geometry_builder::create_box(vec3(0.25f, 3.0f, 0.25f)));
			chair["chair_side_left"].get_transform().position = vec3(5.625f, -1.25f, 4.125f);
			textures["chair_side_left"] = texture("textures/woodSeat.png");

			//back right flat
			chair["chair_side_right"] = mesh(geometry_builder::create_box(vec3(0.25f, 3.0f, 0.25f)));
			chair["chair_side_right"].get_transform().position = vec3(5.625f, -1.25f, 5.875f);
			textures["chair_side_right"] = texture("textures/woodSeat.png");
	
			//top of chair
			chair["chair_top"] = mesh(geometry_builder::create_box(vec3(0.25f, 1.5f, 1.5f)));
			chair["chair_top"].get_transform().position = vec3(5.625f, -0.75f, 5.0f);
			textures["chair_top"] = texture("textures/woodSeat.png");
		}

		//Shelf
		{
			//Just the shelf
			shelf["shelf"] = mesh(geometry_builder::create_box(vec3(2.0f, 0.25f, 14.0f)));
			shelf["shelf"].get_transform().position = vec3(-9.125f, 0.0f, 0.0f);
			textures["shelf"] = texture("textures/shelf.jpg");
		}

		//cupboard
		{
			//back of cupboard
			cupboard["cupboard_back"] = mesh(geometry_builder::create_plane(5.0f, 8.0f));
			cupboard["cupboard_back"].get_transform().position = vec3(-1.0f, -1.0f, -9.75f);
			cupboard["cupboard_back"].get_transform().orientation = rotate(mat4(1.0f), half_pi<float>(), vec3(1.0f, 0.0f, 0.0f));
			textures["cupboard_back"] = texture("textures/cupboard.jpg");

			//Left of cupboard
			cupboard["cupboard_left"] = mesh(geometry_builder::create_box(vec3(0.5f, 8.0f, 1.5f)));
			cupboard["cupboard_left"].get_transform().position = vec3(-3.75f, -1.0f, -9.25f);
			textures["cupboard_left"] = texture("textures/cupboard.jpg");

			//Right of cupboard
			cupboard["cupboard_right"] = mesh(geometry_builder::create_box(vec3(0.5f, 8.0f, 1.5f)));
			cupboard["cupboard_right"].get_transform().position = vec3(1.75f, -1.0f, -9.25f);
			textures["cupboard_right"] = texture("textures/cupboard.jpg");

			//Top of cupboard
			cupboard["cupboard_top"] = mesh(geometry_builder::create_box(vec3(6.0f, 0.5f, 1.5f)));
			cupboard["cupboard_top"].get_transform().position = vec3(-1.0f, 3.25f, -9.25f);
			textures["cupboard_top"] = texture("textures/cupboard.jpg");

			//Shelves
			//First Shelf
			cupboard["cupboard_shelf1"] = mesh(geometry_builder::create_box(vec3(5.0f, 0.5f, 1.5f)));
			cupboard["cupboard_shelf1"].get_transform().position = vec3(-1.0f, 1.25f, -9.25f);
			textures["cupboard_shelf1"] = texture("textures/cupboard.jpg");

			//second Shelf
			cupboard["cupboard_shelf2"] = mesh(geometry_builder::create_box(vec3(5.0f, 0.5f, 1.5f)));
			cupboard["cupboard_shelf2"].get_transform().position = vec3(-1.0f, -1.25f, -9.25f);
			textures["cupboard_shelf2"] = texture("textures/cupboard.jpg");

			//First Shelf
			cupboard["cupboard_shelf3"] = mesh(geometry_builder::create_box(vec3(5.0f, 0.5f, 1.5f)));
			cupboard["cupboard_shelf3"].get_transform().position = vec3(-1.0f, -3.25f, -9.25f);
			textures["cupboard_shelf3"] = texture("textures/cupboard.jpg");
		}

		//Lamp
		{
			//Lamp base
			lamp["lamp_base"] = mesh(geometry_builder::create_cylinder(100, 100, vec3(3.5f, 2.5f, 3.5f)));
			lamp["lamp_base"].get_transform().position = vec3(0.0f, -3.75f, 0.0f);
			textures["lamp_base"] = texture("textures/lampMetal.jpg");

			//Lamp Top
			lamp["lamp_top"] = mesh(geometry_builder::create_cylinder(100, 100, vec3(3.5f, 2.5f, 3.5f)));
			lamp["lamp_top"].get_transform().position = vec3(0.0f, 3.75f, 0.0f);
			textures["lamp_top"] = texture("textures/lampMetal.jpg");

		}

		//mirror
		{
			mirror["mirrorPlane"] = mesh(geometry_builder::create_plane(5, 10, true));
			mirror["mirrorPlane"].get_transform().position = vec3(0.0f, 3.75f, 9.9f);
			mirror["mirrorPlane"].get_transform().orientation = rotate(mat4(1.0f), (half_pi<float>() + pi<float>()), vec3(1.0f, 0.0f, 0.0f));
			mirror["mirrorPlane"].set_material(mat);
			textures["mirrorPlane"] = texture("textures/mirror.jpg");
		}

		//gel/balls
		{
			mat.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
			mat.set_specular(vec4(0.0f, 1.0f, 0.0f, 1.0f));
			mat.set_emissive(vec4(0.5f, 0.5f, 0.5f, 1.0f));
			mat.set_shininess(50.0f);
			balls["first_ball"] = mesh(geometry_builder::create_sphere(100, 100, vec3(0.75f, 0.75f, 0.75f)));
			balls["first_ball"].get_transform().position = vec3(0.0f, 0.0f, 0.0f);
			balls["first_ball"].set_material(mat);
			textures["first_ball"] = texture("textures/gelgreen.jpg");

			mat.set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
			mat.set_specular(vec4(0.0f, 0.0f, 1.0f, 1.0f));
			balls["second_ball"] = mesh(geometry_builder::create_sphere(100, 100, vec3(0.5f, 0.5f, 0.5f)));
			balls["second_ball"].get_transform().position = vec3(-1.0f, 2.0f, 0.0f);
			balls["second_ball"].set_material(mat);
			textures["second_ball"] = texture("textures/gelblue.png");

			mat.set_diffuse(vec4(0.0f, 1.0f, 0.0f, 1.0f));
			mat.set_specular(vec4(1.0f, 0.0f, 0.0f, 1.0f));
			balls["third_ball"] = mesh(geometry_builder::create_sphere(100, 100, vec3(0.5f, 0.5f, 0.5f)));
			balls["third_ball"].get_transform().position = vec3(1.0f, -2.0f, 0.0f);
			balls["third_ball"].set_material(mat);
			textures["third_ball"] = texture("textures/gelred.jpg");
		}

		
	}


	normalMap = texture("textures/table_normal.png");
	//set spotlight
	spotlight.set_light_colour(vec4(0.9f, 0.9f, 0.9f, 1.0f));
	spotlight.set_position(vec3(0.0f, 5.0f, 0.0f));
	spotlight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spotlight.set_range(5.0f);
	spotlight.set_power(0.5f);

	//set ball lights
	ball_lights[0].set_position(balls["first_ball"].get_transform().position);
	ball_lights[0].set_range(5.0f);
	ball_lights[0].set_light_colour(vec4(0.0f, 1.0f, 0.0f, 1.0f));

	ball_lights[1].set_position(balls["second_ball"].get_transform().position);
	ball_lights[1].set_range(5.0f);
	ball_lights[1].set_light_colour(vec4(0.0f, 0.0f, 1.0f, 1.0f));

	ball_lights[2].set_position(balls["third_ball"].get_transform().position);
	ball_lights[2].set_range(5.0f);
	ball_lights[2].set_light_colour(vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//cube map
	{	
		array<string, 6> filenames = { "textures/dark_front.png", "textures/dark_back.png", "textures/dark_top.png",
										"textures/dark_bottom.png", "textures/dark_right.png", "textures/dark_left.png" };
		cube_map = cubemap(filenames);
	}


	eff.add_shader("shaders/ballbasic.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/ballbasic.frag", GL_FRAGMENT_SHADER);

	geff.add_shader("shaders/ballbasic.vert", GL_VERTEX_SHADER);
	geff.add_shader("shaders/greyscale.frag", GL_FRAGMENT_SHADER);

	mirrorEff.add_shader("shaders/ballbasic.vert", GL_VERTEX_SHADER);
	mirrorEff.add_shader("shaders/ballbasic.frag", GL_FRAGMENT_SHADER);

	tableNorm.add_shader("shaders/normal.frag", GL_FRAGMENT_SHADER);
	tableNorm.add_shader("shaders/normal.vert", GL_VERTEX_SHADER);
	tableNorm.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
	tableNorm.add_shader("shaders/part_normal_map.frag", GL_FRAGMENT_SHADER);
	//skybox shaders
	sbeff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
	sbeff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);
    // Build effect
    eff.build();
	sbeff.build();
	tableNorm.build();
	mirrorEff.build();
	geff.build();


  // Set camera properties
  freeCam.set_position(vec3(0.0f, 0.0f, 10.0f));
  freeCam.set_target(vec3(0.0f, 0.0f, 0.0f));
  freeCam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  mirrorCam.set_position(vec3(0.0f, 3.75f, 9.9f));
  mirrorCam.set_target(vec3(0.0f, 0.0f, 10.0f));
  
  return true;
}


bool update(float delta_time) {
	// The ratio of pixels to rotation
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) /
	static_cast<float>(renderer::get_screen_width()))) /
	static_cast<float>(renderer::get_screen_height());

// Increment theta - half a rotation per second
theta += pi<float>() * delta_time;



//Different camera types
{
	//release setcam
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_SPACE)) {
		freeCam.set_position(vec3(0.0f, 0.0f, 0.0f));
		freeCam.set_target(vec3(0.0f, 0.0f, 10.0f));
		mirrorCam.set_position(mirror["mirrorPlane"].get_transform().position);
		mirrorCam.set_target(vec3(0.0f, 0.0f, -10.0f));
		cam_type = false;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_1)) {
		targetCam.set_position(vec3(8.0f, 3.0f, 8.0f));
		targetCam.set_target(vec3(0.0f, 0.0f, 0.0f));
		cam_type = true;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_2)) {
		targetCam.set_position(vec3(8.0f, 3.0f, -8.0f));
		targetCam.set_target(vec3(0.0f, 0.0f, 0.0f));
		cam_type = true;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_3)) {
		targetCam.set_position(vec3(-8.0f, 3.0f, 8.0f));
		targetCam.set_target(vec3(0.0f, 0.0f, 0.0f));
		cam_type = true;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_4)) {
		targetCam.set_position(vec3(-8.0f, 3.0f, -8.0f));
		targetCam.set_target(vec3(0.0f, 0.0f, 0.0f));
		cam_type = true;
	}
}

//post processing
{
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_P))
	{
		greyscale = true;
		built = false;
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_G))
	{
		greyscale = false;
	}


}





//set ball and light movement
{
	//ball1 (green ball)
	if (ball1_direction) {
		balls["first_ball"].get_transform().position.y -= delta_time;
		if (balls["first_ball"].get_transform().position.y <= -3.0f)
		{
			ball1_direction = false;
		}

	}
	else {
		balls["first_ball"].get_transform().position.y += delta_time;
		if (balls["first_ball"].get_transform().position.y >= 3.0f)
			ball1_direction = true;
	}

	//ball2 (blue ball)
	if (ball2_direction) {
		balls["second_ball"].get_transform().position.y -= delta_time;
		balls["second_ball"].get_transform().position.x += sin(theta) * 0.05;
		balls["second_ball"].get_transform().position.z += cos(theta) * 0.05;
		if (balls["second_ball"].get_transform().position.y <= -3.0f)
		{
			ball2_direction = false;
		}

	}
	else {
		balls["second_ball"].get_transform().position.y += delta_time;
		balls["second_ball"].get_transform().position.x += sin(theta) * 0.05;
		balls["second_ball"].get_transform().position.z += cos(theta) * 0.05;
		if (balls["second_ball"].get_transform().position.y >= 3.0f)
			ball2_direction = true;
	}

	//ball3 (red ball)
	if (ball3_direction) {
		balls["third_ball"].get_transform().position.y -= delta_time;
		balls["third_ball"].get_transform().position.x -= sin(theta) * 0.05;
		balls["third_ball"].get_transform().position.z -= cos(theta) * 0.05;
		if (balls["third_ball"].get_transform().position.y <= -3.0f)
		{
			ball3_direction = false;
		}

	}
	else {
		balls["third_ball"].get_transform().position.y += delta_time;
		balls["third_ball"].get_transform().position.x -= sin(theta) * 0.05;
		balls["third_ball"].get_transform().position.z -= cos(theta) * 0.05;
		if (balls["third_ball"].get_transform().position.y >= 3.0f)
			ball3_direction = true;
	}

	//move lights to go with balls
	ball_lights[0].set_position(balls["first_ball"].get_transform().position);
	ball_lights[1].set_position(balls["second_ball"].get_transform().position);
	ball_lights[2].set_position(balls["third_ball"].get_transform().position);



	}

//cam movement
{
	double current_x;
	double current_y;
	// Get the current cursor position
	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);
	// Calculate delta of cursor positions from last frame
	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;
	// Multiply deltas by ratios and delta_time
	delta_x = delta_x * ratio_width;
	delta_y = delta_y * ratio_height;
	// Rotate cameras by delta
	// delta_y - x-axis rotation
	// delta_x - y-axis rotation
	freeCam.rotate(delta_x, -delta_y);
	//mirrorCam.rotate(-delta_x, delta_y);
	int shift_held = 1;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT_SHIFT))
		shift_held = 5;

	// Use keyboard to move the camera - WSAD
	vec3 translation(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(renderer::get_window(), 'W')) {
		translation.z += 5.0f * delta_time * shift_held;
	}
	if (glfwGetKey(renderer::get_window(), 'A')) {
		translation.x -= 5.0f * delta_time* shift_held;
	}
	if (glfwGetKey(renderer::get_window(), 'S')) {
		translation.z -= 5.0f * delta_time* shift_held;
	}
	if (glfwGetKey(renderer::get_window(), 'D')) {
		translation.x += 5.0f * delta_time* shift_held;
	}
	// Move camera
	freeCam.move(translation);
	// Update the camera
	freeCam.update(delta_time);
	targetCam.update(delta_time);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;
}
  return true;
}

bool render() {
	//render all meshes/objects
	{
		auto V = freeCam.get_view();
		auto P = freeCam.get_projection();
		if (cam_type) {
			V = targetCam.get_view();
			P = targetCam.get_projection();
		}
		// Set render target to frame buffer
		renderer::set_render_target(frame);
		// Set clear colour to white
		renderer::setClearColour(1.0f, 1.0f, 1.0f);
		// Clear frame
		renderer::clear();

		//render the room
		for (auto &e : room) {
			auto m = e.second;
			// Bind effect
			renderer::bind(eff);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			auto MVP = P * V * M;
			// Set MVP matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			//Set M Matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			//Set N Matrix uniform
			glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
			//bind material
			renderer::bind(m.get_material(), "mat");
			//bind point lights
			renderer::bind(ball_lights, "points");
			//bind texture
			renderer::bind(textures[e.first], 0);
			//Set tex uniform
			glUniform1i(eff.get_uniform_location("tex"), 0);
			//Set eye position
			if (cam_type)
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(freeCam.get_position()));
			else
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(targetCam.get_position()));
			//Render the object
			renderer::render(m);
		}

		//render the table
		for (auto &e : table) {
			auto m = e.second;
			// Bind effect
			renderer::bind(tableNorm);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			auto MVP = P * V * M;
			// Set MVP matrix uniform
			glUniformMatrix4fv(tableNorm.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			//Set M Matrix uniform
			glUniformMatrix4fv(tableNorm.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			//Set N Matrix uniform
			glUniformMatrix3fv(tableNorm.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
			//bind material
			renderer::bind(m.get_material(), "mat");
			//bind point lights
			renderer::bind(ball_lights, "points");
			//bind light
			renderer::bind(light, "light");
			//bind texture
			renderer::bind(textures[e.first], 0);
			//bind normalmap
			renderer::bind(normalMap, 1);
			// Set normal_map uniform
			glUniform1i(tableNorm.get_uniform_location("normal_map"), 1);
			//Set tex uniform
			glUniform1i(tableNorm.get_uniform_location("tex"), 0);
			//Set eye position
			if (cam_type)
				glUniform3fv(tableNorm.get_uniform_location("eye_pos"), 1, value_ptr(freeCam.get_position()));
			else
				glUniform3fv(tableNorm.get_uniform_location("eye_pos"), 1, value_ptr(targetCam.get_position()));
			//Render the object
			renderer::render(m);
		}

		//render the chair
		for (auto &e : chair) {
			auto m = e.second;
			// Bind effect
			renderer::bind(eff);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			auto MVP = P * V * M;
			// Set MVP matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			//Set M Matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			//Set N Matrix uniform
			glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
			//bind material
			renderer::bind(m.get_material(), "mat");
			//bind point lights
			renderer::bind(ball_lights, "points");
			//bind light
			//renderer::bind(light, "light");
			//bind texture
			renderer::bind(textures[e.first], 0);
			//Set tex uniform
			glUniform1i(eff.get_uniform_location("tex"), 0);
			//Set eye position
			if (cam_type)
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(freeCam.get_position()));
			else
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(targetCam.get_position()));
			//Render the object
			renderer::render(m);
		}

		for (auto &e : shelf) {
			auto m = e.second;
			// Bind effect
			renderer::bind(eff);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			auto MVP = P * V * M;
			// Set MVP matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			//Set M Matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			//Set N Matrix uniform
			glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
			//bind material
			renderer::bind(m.get_material(), "mat");
			//bind point lights
			renderer::bind(ball_lights, "points");
			//bind light
			//renderer::bind(light, "light");
			//bind texture
			renderer::bind(textures[e.first], 0);
			//Set tex uniform
			glUniform1i(eff.get_uniform_location("tex"), 0);
			//Set eye position
			if (cam_type)
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(freeCam.get_position()));
			else
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(targetCam.get_position()));
			//Render the object
			renderer::render(m);
		}

		for (auto &e : cupboard) {
			auto m = e.second;
			// Bind effect
			renderer::bind(eff);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			auto MVP = P * V * M;
			// Set MVP matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			//Set M Matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			//Set N Matrix uniform
			glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
			//bind material
			renderer::bind(m.get_material(), "mat");
			//bind point lights
			renderer::bind(ball_lights, "points");
			//bind light
			//renderer::bind(light, "light");
			//bind texture
			renderer::bind(textures[e.first], 0);
			//Set tex uniform
			glUniform1i(eff.get_uniform_location("tex"), 0);
			//Set eye position
			if (cam_type)
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(freeCam.get_position()));
			else
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(targetCam.get_position()));
			//Render the object
			renderer::render(m);
		}

		for (auto &e : lamp) {
			auto m = e.second;
			// Bind effect
			renderer::bind(eff);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			auto MVP = P * V * M;
			// Set MVP matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			//Set M Matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			//Set N Matrix uniform
			glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
			//bind material
			renderer::bind(m.get_material(), "mat");
			//bind point lights
			renderer::bind(ball_lights, "points");
			//bind light
			//renderer::bind(light, "light");
			//bind texture
			renderer::bind(textures[e.first], 0);
			//Set tex uniform
			glUniform1i(eff.get_uniform_location("tex"), 0);
			//Set eye position
			if (cam_type)
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(freeCam.get_position()));
			else
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(targetCam.get_position()));
			//Render the object
			renderer::render(m);
		}

		for (auto &e : balls) {
			auto m = e.second;
			// Bind effect
			renderer::bind(eff);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			auto MVP = P * V * M;
			// Set MVP matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			//Set M Matrix uniform
			glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			//Set N Matrix uniform
			glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr((m.get_transform().get_normal_matrix())));
			//bind material
			renderer::bind(m.get_material(), "mat");
			//bind point lights
			renderer::bind(ball_lights, "points");
			//bind light
			renderer::bind(spotlight, "spotlight");
			//renderer::bind(light, "light");
			//bind texture
			renderer::bind(textures[e.first], 0);
			//Set tex uniform
			glUniform1i(eff.get_uniform_location("tex"), 0);
			//Set eye position
			if (cam_type)
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(freeCam.get_position()));
			else
				glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(targetCam.get_position()));
			//Render the object
			renderer::render(m);
		}



		glDisable(GL_CULL_FACE);

		renderer::bind(sbeff);
		auto MVP = P * V * scale(mat4(), vec3(500.0f));
		glUniformMatrix4fv(sbeff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		renderer::bind(cube_map, 0);
		glUniform1i(sbeff.get_uniform_location("cubemap"), 0);
		renderer::render(geometry_builder::create_box());

		glEnable(GL_CULL_FACE);

		
		
		// *********************************
		// Render meshes
		for (auto &e : mirror) {
			auto m = e.second;
			// Bind effect
			renderer::bind(mirrorEff);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			auto V = mirrorCam.get_view();
			auto P = mirrorCam.get_projection();
			auto MVP = P * V * M;
			// Set MVP matrix uniform
			glUniformMatrix4fv(mirrorEff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
			// Create MV matrix
			auto MV = V * M;
			// Set MV matrix uniform
			glUniformMatrix4fv(mirrorEff.get_uniform_location("MV"), 1, GL_FALSE, value_ptr(MV));
			// Set M matrix uniform
			glUniformMatrix4fv(mirrorEff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
			// Set N matrix uniform
			glUniformMatrix3fv(mirrorEff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
			// Bind material
			renderer::bind(m.get_material(), "mat");
			// Bind light
			renderer::bind(ball_lights, "points");
			//spotlight
			renderer::bind(spotlight, "spotlight");
			// Bind texture
			renderer::bind(textures[e.first], 0);
			// Set tex uniform
			glUniform1i(mirrorEff.get_uniform_location("tex"), 0);
			// Set eye position
			glUniform3fv(mirrorEff.get_uniform_location("eye_pos"), 1, value_ptr(mirrorCam.get_position()));

			// Render mesh
			renderer::render(m);
		}

		//bind mirror effect
		renderer::bind(mirrorEff);
		//get M from the mirror
		auto M = mirror["mirrorPlane"].get_transform().get_transform_matrix();
		//P and V
		V = freeCam.get_view();
		P = freeCam.get_projection();
		//MVP
		MVP = P * M * V;
		//mvp matrix uniform
		glUniformMatrix4fv(mirrorEff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		//bind texture from buffer
		renderer::bind(frame.get_frame(), 1);
		//set tex uniform
		glUniform1i(mirrorEff.get_uniform_location("tex"), 1);
		//render mirror
		renderer::render(mirror["mirrorPlane"]);






		// Return clear colour to cyan
		renderer::setClearColour(0.0f, 1.0f, 1.0f);
		// *********************************
		// Set render target back to the screen
		renderer::set_render_target();

		effect EFF = eff;
		if (greyscale)
		{
			EFF = eff;
			built = true;
		}
		else
		{
			EFF = geff;
		}
		if (!built)
			EFF.build();

		// Set render target back to the screen
		renderer::set_render_target();
		// Bind Tex effect
		renderer::bind(EFF);
		// MVP is now the identity matrix
		MVP = mat4(1);
		// Set MVP matrix uniform
		glUniformMatrix4fv(EFF.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		// Bind texture from frame buffer
		renderer::bind(frame.get_frame(), 1);
		// Set the tex uniform
		glUniform1i(EFF.get_uniform_location("tex"), 1);
		// Render the screen quad
		renderer::render(screen_quad);
		// *********************************
		

		
	}
	return true;
}


void main() {
  // Create application
  app application("Graphics Coursework");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_initialise(initialise);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}