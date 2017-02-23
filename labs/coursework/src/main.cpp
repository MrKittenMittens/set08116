#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

geometry geom;
effect eff;
free_camera cam;

//Declare meshes for objects/furnature
map<string, mesh> table;
map<string, mesh> room;
map<string, mesh> chair;
map<string, mesh> shelf;

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
	//load meshes for objects
	//Back wall
	room["wall_back_inner"] = mesh(geometry_builder::create_plane(20, 10));
	room["wall_back_inner"].get_transform().orientation = rotate(mat4(1.0f), half_pi<float>(), vec3(1.0f, 0.0f, 0.0f));
	room["wall_back_inner"].get_transform().position = vec3(0.0f, 0.0f, -10.0f);
	textures["wall_back_inner"] = texture("textures/wallpaperBF.png");

	//Front wall
	room["wall_front_inner"] = mesh(geometry_builder::create_plane(20, 10));
	room["wall_front_inner"].get_transform().orientation = rotate(mat4(1.0f), (half_pi<float>() + pi<float>()), vec3(1.0f, 0.0f, 0.0f));
	room["wall_front_inner"].get_transform().position = vec3(0.0f, 0.0f, 10.0f);
	textures["wall_front_inner"] = texture("textures/wallpaperBF.png");

	//Right wall
	room["wall_left_inner"] = mesh(geometry_builder::create_plane(10, 20));
	room["wall_left_inner"].get_transform().position = vec3(-10.0f, 0.0f, 0.0f);
	room["wall_left_inner"].get_transform().orientation = rotate(mat4(1.0f), (half_pi<float>() + pi<float>()), vec3(0.0f, 0.0f, 1.0f));
	textures["wall_left_inner"] = texture("textures/wallpaperLR.png");

	//Left wall
	room["wall_right_inner"] = mesh(geometry_builder::create_plane(10, 20));
	room["wall_right_inner"].get_transform().position = vec3(10.0f, 0.0f, 0.0f);
	room["wall_right_inner"].get_transform().orientation = rotate(mat4(1.0f), half_pi<float>(), vec3(0.0f, 0.0f, 1.0f));
	textures["wall_right_inner"] = texture("textures/wallpaperLR.png");

	//floor
	room["floor"] = mesh(geometry_builder::create_plane(20, 20));
	room["floor"].get_transform().position = vec3(0.0f, -5.0f, 0.0f);
	textures["floor"] = texture("textures/floor.jpg");

	//Load meshes for table
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

	//Load meshes for chair
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


	//Shelf
	//Just the shelf
	shelf["shelf"] = mesh(geometry_builder::create_box(vec3(2.0f, 0.25f, 14.0f)));
	shelf["shelf"].get_transform().position = vec3(-9.125f, 0.0f, 0.0f);
	textures["shelf"] = texture("textures/shelf.jpg");



	/*
	//Not rotating for now, will fix later if rotation becomes less of an issue
	chair["chair_front_left"].get_transform().orientation = rotate(mat4(1.0f), quarter_pi<float>()/2, vec3(0.0f, 1.0f, 0.0f));
	chair["chair_front_right"].get_transform().orientation = rotate(mat4(1.0f), quarter_pi<float>()/2, vec3(0.0f, 1.0f, 0.0f));
	chair["chair_back_left"].get_transform().orientation = rotate(mat4(1.0f), quarter_pi<float>() / 2, vec3(0.0f, 1.0f, 0.0f));
	chair["chair_seat"].get_transform().orientation = rotate(mat4(1.0f), quarter_pi<float>() / 2, vec3(0.0f, 1.0f, 0.0f));

	*/

	// Load in shaders
	eff.add_shader("shaders/basic.vert", GL_VERTEX_SHADER);
	eff.add_shader("shaders/basic.frag", GL_FRAGMENT_SHADER);
    // Build effect
    eff.build();

  // Set camera properties
  cam.set_position(vec3(0.0f, 0.0f, 10.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}


bool update(float delta_time) {
	// The ratio of pixels to rotation
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height = (quarter_pi<float>() * (static_cast<float>(renderer::get_screen_height()) /
		static_cast<float>(renderer::get_screen_width()))) /
		static_cast<float>(renderer::get_screen_height());

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
	cam.rotate(delta_x, -delta_y);
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
	cam.move(translation);
	// Update the camera
	cam.update(delta_time);
	// Update cursor pos
	cursor_x = current_x;
	cursor_y = current_y;
  return true;
}

bool render() {
	//render the room
	for (auto &e : room) {
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		//bind textures to renderer and render correct walls
		renderer::bind(textures[e.first], 0);
		glUniform1i(eff.get_uniform_location("tex"), 0);
		//Render the object
		renderer::render(m);
	}

	//render the table
	for (auto &e : table) {
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		//bind textures to renderer and render correct walls
		renderer::bind(textures[e.first], 0);
		glUniform1i(eff.get_uniform_location("tex"), 0);
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
		auto V = cam.get_view();
		auto P = cam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		//bind textures to renderer and render correct walls
		renderer::bind(textures[e.first], 0);
		glUniform1i(eff.get_uniform_location("tex"), 0);
		//Render the object
		renderer::render(m);
	}

	for (auto &e : shelf) {
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		//bind textures to renderer and render correct walls
		renderer::bind(textures[e.first], 0);
		glUniform1i(eff.get_uniform_location("tex"), 0);
		//Render the object
		renderer::render(m);
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