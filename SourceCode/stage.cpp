#include "stage.h"
#include "world.h"
#include "game.h"
#include "entity.h"
#include "extra/bass.h"


Stage::Stage()
{

}

Stage::~Stage()
{

}

void Stage::render(Camera* camera)
{

}

void Stage::update(float dt)
{

}

void Stage::initStage()
{

}


// -------------------- STAGE TITLE -------------------- //

void StageTitle::render(Camera* camera)
{
	// Quad render
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// To draw the quad
	shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");

	// Enable shader
	shader->enable();

	// Upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/chains_title.tga"));
	shader->setUniform("u_time", Game::instance->time);

	// Draw Call
	back->render(GL_TRIANGLES);

	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/font_map.tga"));
	letters->render(GL_TRIANGLES);

	// Disable shader
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void StageTitle::update(float dt)
{
	// Change to the next stage
	if (Input::wasKeyPressed(SDL_SCANCODE_O) || Input::gamepads[0].wasButtonPressed(A_BUTTON))
	{
		Game::instance->stages[STAGE_INTRO]->initStage();
		Game::instance->current_stage = Game::instance->stages[STAGE_INTRO];
	}
}

void StageTitle::initStage()
{
	initQuads();
}

void StageTitle::initQuads()
{
	// We create a quad for the title screen by defining 2 triangles
	back = new Mesh();
	back->createQuad(0, 0, 2, 2, false);


	// We create a quad for the title screen by defining 2 triangles
	letters = new Mesh();

	// --- PRESS TO START ---
	// Three vertces of the first triangle
	letters->vertices.push_back(Vector3(-0.4, -0.7, 0));
	letters->uvs.push_back(Vector2(0. / 800., 461. / 600.));
	letters->vertices.push_back(Vector3(-0.4, -0.8, 0));
	letters->uvs.push_back(Vector2(0. / 800., 412. / 600.));
	letters->vertices.push_back(Vector3(0.4, -0.8, 0));
	letters->uvs.push_back(Vector2(289. / 800., 412. / 600.));

	// Three vertices of the second triangle
	letters->vertices.push_back(Vector3(-0.4, -0.7, 0));
	letters->uvs.push_back(Vector2(0. / 800., 461. / 600.));
	letters->vertices.push_back(Vector3(0.4, -0.8, 0));
	letters->uvs.push_back(Vector2(289. / 800., 412. / 600.));
	letters->vertices.push_back(Vector3(0.4, -0.7, 0));
	letters->uvs.push_back(Vector2(289. / 800., 461. / 600.));

	// --- PICK & RUN ---
	// Three vertces of the first triangle
	letters->vertices.push_back(Vector3(0., 0.3, 0));
	letters->uvs.push_back(Vector2(0. / 800., 40. / 600.));
	letters->vertices.push_back(Vector3(0., 0.0, 0));
	letters->uvs.push_back(Vector2(0. / 800., 0. / 600.));
	letters->vertices.push_back(Vector3(0.8, 0.0, 0));
	letters->uvs.push_back(Vector2(155. / 800., 0. / 600.));

	// Three vertices of the second triangle
	letters->vertices.push_back(Vector3(0.0, 0.3, 0));
	letters->uvs.push_back(Vector2(0. / 800., 40. / 600.));
	letters->vertices.push_back(Vector3(0.8, 0.0, 0));
	letters->uvs.push_back(Vector2(155. / 800., 0. / 600.));
	letters->vertices.push_back(Vector3(0.8, 0.3, 0));
	letters->uvs.push_back(Vector2(155. / 800., 40. / 600.));
}


// -------------------- STAGE INTRO -------------------- //

void StageIntro::render(Camera* camera)
{
	// --- Player Mesh ---
	shader = Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs");
	mesh = Mesh::Get("data/characters/male.mesh");
	animation = Animation::Get("data/characters/looking_around.skanim");
	animation->assignTime(Game::instance->time);
	Matrix44 model;

	//enable shader
	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture_1", Texture::Get("data/characters/robber.tga"));
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Game::instance->time);

	//do the draw call
	mesh->renderAnimated(GL_TRIANGLES, &animation->skeleton);

	//disable shader
	shader->disable();

	// --- World Mesh ---
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	mesh = Mesh::Get("data/sphere.obj");
	model.setTranslation(11., 14., 0.);
	model.scale(5., 5., 5.);
	model.rotate(Game::instance->time / 3., Vector3(0, -1, 0)); // Anticlockwise rotation

	//enable shader
	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/mapamundi.tga"));
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Game::instance->time);

	//do the draw call
	mesh->render(GL_TRIANGLES);

	//disable shader
	shader->disable();

	// --- Background Sphere ---
	shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	mesh = Mesh::Get("data/sphere.obj");
	model.setTranslation(Game::instance->camera->eye.x, Game::instance->camera->eye.y, Game::instance->camera->eye.z);
	model.scale(50., 50., 50.);
	model.rotate(Game::instance->time / 40., Vector3(0, 1, 0));

	//enable shader
	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/back_smoke.tga"));
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Game::instance->time);

	//do the draw call
	mesh->render(GL_TRIANGLES);

	//disable shader
	shader->disable();

	// --- GUI & HUD ---
	shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//enable shader
	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/font_map.tga"));
	shader->setUniform("u_time", Game::instance->time);

	//do the draw call
	quad->render(GL_TRIANGLES);

	//disable shader
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Show money value
	std::string cash_string = std::to_string(Game::instance->money);
	cash_string.erase(cash_string.find('.') + 3, std::string::npos);
	drawText(475 * Game::instance->window_width / 800., 45 * Game::instance->window_height / 600., cash_string, Vector3(1, 1, 1), (30 - cash_string.size()) / 4);
}

void StageIntro::update(float dt)
{
	// Change to the next stage
	if (Input::wasKeyPressed(SDL_SCANCODE_O) || Input::gamepads[0].wasButtonPressed(A_BUTTON))
	{
		Game::instance->audio->stopAudio(); // Stop the intro audio when playing the game
		Game::instance->stages[STAGE_PLAY]->initStage();
		Game::instance->current_stage = Game::instance->stages[STAGE_PLAY];
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_I) || Input::gamepads[0].wasButtonPressed(B_BUTTON))
	{
		Game::instance->stages[STAGE_TITLE]->initStage();
		Game::instance->current_stage = Game::instance->stages[STAGE_TITLE];
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_N) || Input::gamepads[0].wasButtonPressed(X_BUTTON))
	{
		Game::instance->stages[STAGE_CONTROLS]->initStage();
		Game::instance->previous_stage = STAGE_INTRO;
		Game::instance->current_stage = Game::instance->stages[STAGE_CONTROLS];
	}
}

void StageIntro::initStage()
{
	initQuads();
}

void StageIntro::initQuads()
{
	// We create a quad for the title screen by defining 2 triangles
	quad = new Mesh();

	// --- MISSION SELECTED ---
	// Three vertces of the first triangle
	quad->vertices.push_back(Vector3(-0.2, -0.5, 0));
	quad->uvs.push_back(Vector2(0. / 800., 412. / 600.));
	quad->vertices.push_back(Vector3(-0.2, -0.6, 0));
	quad->uvs.push_back(Vector2(0. / 800., 361. / 600.));
	quad->vertices.push_back(Vector3(0.4, -0.6, 0));
	quad->uvs.push_back(Vector2(340. / 800., 361. / 600.));

	// Three vertices of the second triangle
	quad->vertices.push_back(Vector3(-0.2, -0.5, 0));
	quad->uvs.push_back(Vector2(0. / 800., 412. / 600.));
	quad->vertices.push_back(Vector3(0.4, -0.6, 0));
	quad->uvs.push_back(Vector2(340. / 800., 361. / 600.));
	quad->vertices.push_back(Vector3(0.4, -0.5, 0));
	quad->uvs.push_back(Vector2(340. / 800., 412. / 600.));

	// --- NAME OF THE MISSION ---
	// Three vertces of the first triangle
	quad->vertices.push_back(Vector3(0.43, -0.45, 0));
	quad->uvs.push_back(Vector2(0. / 800., 361. / 600.));
	quad->vertices.push_back(Vector3(0.43, -0.65, 0));
	quad->uvs.push_back(Vector2(0. / 800., 310. / 600.));
	quad->vertices.push_back(Vector3(0.93, -0.65, 0));
	quad->uvs.push_back(Vector2(160. / 800., 310. / 600.));

	// Three vertices of the second triangle
	quad->vertices.push_back(Vector3(0.43, -0.45, 0));
	quad->uvs.push_back(Vector2(0. / 800., 361. / 600.));
	quad->vertices.push_back(Vector3(0.93, -0.65, 0));
	quad->uvs.push_back(Vector2(160. / 800., 310. / 600.));
	quad->vertices.push_back(Vector3(0.93, -0.45, 0));
	quad->uvs.push_back(Vector2(160. / 800., 361. / 600.));

	// --- MONEY ---
	// Three vertces of the first triangle
	quad->vertices.push_back(Vector3(-0.2, 0.85, 0));
	quad->uvs.push_back(Vector2(300. / 800., 600 / 600.));
	quad->vertices.push_back(Vector3(-0.2, 0.72, 0));
	quad->uvs.push_back(Vector2(300. / 800., 561. / 600.));
	quad->vertices.push_back(Vector3(0.1, 0.72, 0));
	quad->uvs.push_back(Vector2(390. / 800., 561. / 600.));

	// Three vertices of the second triangle
	quad->vertices.push_back(Vector3(-0.2, 0.85, 0));
	quad->uvs.push_back(Vector2(300. / 800., 600. / 600.));
	quad->vertices.push_back(Vector3(0.1, 0.72, 0));
	quad->uvs.push_back(Vector2(390. / 800., 561. / 600.));
	quad->vertices.push_back(Vector3(0.1, 0.85, 0));
	quad->uvs.push_back(Vector2(390. / 800., 600. / 600.));

	// --- $ ---
	// Three vertces of the first triangle
	quad->vertices.push_back(Vector3(0.7, 0.88, 0));
	quad->uvs.push_back(Vector2(210. / 800., 600 / 600.));
	quad->vertices.push_back(Vector3(0.7, 0.70, 0));
	quad->uvs.push_back(Vector2(210. / 800., 559. / 600.));
	quad->vertices.push_back(Vector3(0.8, 0.70, 0));
	quad->uvs.push_back(Vector2(239. / 800., 559. / 600.));

	// Three vertices of the second triangle
	quad->vertices.push_back(Vector3(0.7, 0.88, 0));
	quad->uvs.push_back(Vector2(210. / 800., 600. / 600.));
	quad->vertices.push_back(Vector3(0.8, 0.70, 0));
	quad->uvs.push_back(Vector2(239. / 800., 559. / 600.));
	quad->vertices.push_back(Vector3(0.8, 0.88, 0));
	quad->uvs.push_back(Vector2(239. / 800., 600. / 600.));

	// --- A, B, X ---
	// Three vertces of the first triangle
	quad->vertices.push_back(Vector3(-0.1, -0.75, 0));
	quad->uvs.push_back(Vector2(0. / 800., 310 / 600.));
	quad->vertices.push_back(Vector3(-0.1, -0.85, 0));
	quad->uvs.push_back(Vector2(0. / 800., 259. / 600.));
	quad->vertices.push_back(Vector3(0.8, -0.85, 0));
	quad->uvs.push_back(Vector2(695. / 800., 259. / 600.));

	// Three vertices of the second triangle
	quad->vertices.push_back(Vector3(-0.1, -0.75, 0));
	quad->uvs.push_back(Vector2(0. / 800., 310. / 600.));
	quad->vertices.push_back(Vector3(0.8, -0.85, 0));
	quad->uvs.push_back(Vector2(695. / 800., 259. / 600.));
	quad->vertices.push_back(Vector3(0.8, -0.75, 0));
	quad->uvs.push_back(Vector2(695. / 800., 310. / 600.));
}


// -------------------- STAGE PLAY -------------------- //

void StagePlay::render(Camera* camera)
{
	// Only in the play stage we will call the render of the world
	world->renderWorld(camera);

	glClear(GL_DEPTH_BUFFER_BIT);

	// Minimapa
	glViewport(0.7 * Game::instance->window_width, 0.7 * Game::instance->window_height, 0.3 * Game::instance->window_width, 0.3 * Game::instance->window_height);

	camera->eye = Vector3((world->world_size + 6 * world->cell_size) / 2, world->world_size * 0.85, (world->world_size + 6 * world->cell_size) / 2 - 1);
	camera->center = Vector3((world->world_size + 6 * world->cell_size) / 2, 0, (world->world_size + 6 * world->cell_size) / 2);
	camera->enable();

	world->renderMiniMap(camera);

	// Reset the values
	glViewport(0, 0, Game::instance->window_width, Game::instance->window_height);

	camera->eye = Vector3(current_player->model.getTranslation() + Vector3(0, 60, -3));
	camera->center = Vector3(current_player->model * Vector3(0, 18, 0));
	camera->enable();

	// Quad render
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");

	shader->enable();
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/font_map.tga"));
	shader->setUniform("u_time", Game::instance->time);
	quad->render(GL_TRIANGLES);
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Show time and money values
	std::string time_string = std::to_string(Game::instance->curr_time);
	time_string.erase(time_string.find('.') + 3, std::string::npos);
	drawText(20 * Game::instance->window_width / 800., 550 * Game::instance->window_height / 600., time_string, Vector3(1, 1, 1), 3);
	std::string cash_string = std::to_string(current_player->cash);
	cash_string.erase(cash_string.find('.') + 3, std::string::npos);
	drawText(690 * Game::instance->window_width / 800., 550 * Game::instance->window_height / 600., cash_string, Vector3(1, 1, 1), 3);
}

void StagePlay::update(float dt)
{
	Game::instance->curr_time = Game::instance->time - (Game::instance->start_time + Game::instance->pause_time);

	// First we update the world
	world->updateWorld(dt);
	
	// We update the camera following the player
	camera->eye = Vector3(current_player->model.getTranslation() + Vector3(0, 60, -3));
	camera->center = Vector3(current_player->model * Vector3(0, 18, 0));

	// Check if we are close to someone we can steal
	Character* closestPerson = closestPersonToSteal();
	if (closestPerson != NULL && (Input::wasKeyPressed(SDL_SCANCODE_P) || Input::gamepads[0].wasButtonPressed(A_BUTTON)))
	{
		if(closestPerson->cash > 0.001) 
			// The police goes faster if you steal to someone new
			current_cop->dt_speed += 0.1;

		current_player->cash += closestPerson->cash;
		closestPerson->cash = 0;
	}

	// We go to the end screen by pressing O
	if (current_cop->model.getTranslation().distance(current_player->model.getTranslation()) < 5)
	{
		Game::instance->last_loot = current_player->cash;
		Game::instance->money += Game::instance->last_loot; // When the game is finished, we sum the money to the global money
		Game::instance->stages[STAGE_END]->initStage();
		Game::instance->current_stage = Game::instance->stages[STAGE_END];
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_M) || Input::gamepads[0].wasButtonPressed(START_BUTTON))
	{
		Game::instance->stages[STAGE_PAUSE]->initStage();
		Game::instance->current_stage = Game::instance->stages[STAGE_PAUSE];
	}
}

void StagePlay::initWorld()
{
	// Once we change the the play stage, we need to initialize a new world
	world = new World();
	world->initialize();
	
	// Find the character that will be the player
	for (int i = 0; i < world->characters.size(); ++i)
	{
		Character* aux_char = world->characters[i];
		if (aux_char->type == PLAYER) current_player = aux_char;
		if (aux_char->type == COP) current_cop = aux_char;
	}

	// We set the camera following the player
	camera = Game::instance->camera;
	camera->eye = Vector3(current_player->model.getTranslation() + Vector3(0, 60, -1));
	camera->center = Vector3(current_player->model * Vector3(0, 18, 0));
}

Character* StagePlay::closestPersonToSteal()
{
	// Initialize some vars
	float min_distance = INFINITY;
	Character* characterToSteal = NULL;

	// Loop for all the characters existing in the world
	for (int i = 0; i < world->characters.size(); ++i)
	{
		Character* character = world->characters[i];
		// Compute the distance between the player and the character we are checking
		float distance = current_player->model.getTranslation().distance(character->model.getTranslation());

		// If is close enought and its a NPC, save the closest character
		if (distance < 6.0 && character->type == NPC && distance < min_distance)
		{
			// Update the new min distance and the character that is being stealed
			min_distance = distance;
			characterToSteal = character;
		}
	}
	// Return tha character
	return characterToSteal;
}

void StagePlay::initStage()
{
	initQuads();
	initWorld();

	// Reset the time atributes
	Game::instance->start_time = Game::instance->time;
	Game::instance->pause_time = 0.;
	Game::instance->last_loot = 0.;

	Game::instance->audio->playAudio("data/sounds/game.wav");
}

void StagePlay::initQuads()
{
	// We create a quad for the title screen by defining 2 triangles
	quad = new Mesh();

	// --- TIME ---
	// Three vertces of the first triangle
	quad->vertices.push_back(Vector3(-0.95, -0.7, 0));
	quad->uvs.push_back(Vector2(240. / 800., 600. / 600.));
	quad->vertices.push_back(Vector3(-0.95, -0.77, 0));
	quad->uvs.push_back(Vector2(240. / 800., 562. / 600.));
	quad->vertices.push_back(Vector3(-0.8, -0.77, 0));
	quad->uvs.push_back(Vector2(299. / 800., 562. / 600.));

	// Three vertices of the second triangle
	quad->vertices.push_back(Vector3(-0.95, -0.7, 0));
	quad->uvs.push_back(Vector2(240. / 800., 600. / 600.));
	quad->vertices.push_back(Vector3(-0.8, -0.77, 0));
	quad->uvs.push_back(Vector2(299. / 800., 562. / 600.));
	quad->vertices.push_back(Vector3(-0.8, -0.7, 0));
	quad->uvs.push_back(Vector2(299. / 800., 600. / 600.));

	// --- MONEY ---
	// Three vertces of the first triangle
	quad->vertices.push_back(Vector3(0.7, -0.7, 0));
	quad->uvs.push_back(Vector2(300. / 800., 600. / 600.));
	quad->vertices.push_back(Vector3(0.7, -0.77, 0));
	quad->uvs.push_back(Vector2(300. / 800., 561. / 600.));
	quad->vertices.push_back(Vector3(0.95, -0.77, 0));
	quad->uvs.push_back(Vector2(390. / 800., 561. / 600.));

	// Three vertices of the second triangle
	quad->vertices.push_back(Vector3(0.7, -0.7, 0));
	quad->uvs.push_back(Vector2(300. / 800., 600. / 600.));
	quad->vertices.push_back(Vector3(0.95, -0.77, 0));
	quad->uvs.push_back(Vector2(390. / 800., 561. / 600.));
	quad->vertices.push_back(Vector3(0.95, -0.7, 0));
	quad->uvs.push_back(Vector2(390. / 800., 600. / 600.));
}


// -------------------- STAGE CONTROLS -------------------- //

void StageControls::render(Camera* camera)
{
	// Quad render
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// To draw the quad
	shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");

	// Enable shader
	shader->enable();

	// Upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/controls.tga"));
	shader->setUniform("u_time", Game::instance->time);

	// Draw Call
	quad->render(GL_TRIANGLES);

	// Disable shader
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
}

void StageControls::update(float dt)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_I) || Input::wasKeyPressed(SDL_SCANCODE_N) || Input::gamepads[0].wasButtonPressed(B_BUTTON) || Input::gamepads[0].wasButtonPressed(X_BUTTON))
	{
		Game::instance->current_stage = Game::instance->stages[Game::instance->previous_stage];
	}
}

void StageControls::initStage()
{
	initQuads();
}

void StageControls::initQuads()
{
	// We create a quad for the title screen by defining 2 triangles
	quad = new Mesh();
	quad->createQuad(0, 0, 2, 2, false);
}


// -------------------- STAGE PAUSE -------------------- //

void StagePause::render(Camera* camera)
{
	Game::instance->stages[STAGE_PLAY]->render(camera);

	// Quad render
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// To draw the quad
	shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");

	// Enable shader
	shader->enable();

	// Upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/pause_screen.tga"));
	shader->setUniform("u_time", Game::instance->time);

	// Draw Call
	quad->render(GL_TRIANGLES);

	// Disable shader
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

}

void StagePause::update(float dt)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_O) || Input::gamepads[0].wasButtonPressed(A_BUTTON) || Input::gamepads[0].wasButtonPressed(START_BUTTON))
	{
		Game::instance->current_stage = Game::instance->stages[STAGE_PLAY];
		Game::instance->pause_time = Game::instance->time - Game::instance->curr_time - Game::instance->start_time;
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_I) || Input::gamepads[0].wasButtonPressed(B_BUTTON))
	{
		Game::instance->audio->stopAudio();
		Game::instance->audio->playAudio("data/sounds/title.wav");
		Game::instance->camera->lookAt(Vector3(7.f, 15.f, 15.f), Vector3(7.f, 13.f, 0.f), Vector3(0.f, 1.f, 0.f));
		Game::instance->stages[STAGE_INTRO]->initStage();
		Game::instance->current_stage = Game::instance->stages[STAGE_INTRO];
	}
	if (Input::wasKeyPressed(SDL_SCANCODE_N) || Input::gamepads[0].wasButtonPressed(X_BUTTON))
	{
		Game::instance->stages[STAGE_CONTROLS]->initStage();
		Game::instance->previous_stage = STAGE_PAUSE;
		Game::instance->current_stage = Game::instance->stages[STAGE_CONTROLS];
	}
}

void StagePause::initStage()
{
	initQuads();
}

void StagePause::initQuads()
{
	// We create a quad for the title screen by defining 2 triangles
	quad = new Mesh();
	quad->createQuad(0, 0, 2, 2, false);
}


// -------------------- STAGE END -------------------- //

void StageEnd::render(Camera* camera)
{
	// Quad render
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// To draw the quad
	shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");

	// Enable shader
	shader->enable();

	// Upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/dollars_background.tga"));
	shader->setUniform("u_time", Game::instance->time);

	// Draw Call
	back->render(GL_TRIANGLES);

	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/alpha_black.tga"));
	alpha_black->render(GL_TRIANGLES);

	shader->setUniform("u_texture_1", Texture::Get("data/gui_hud/font_map.tga"));
	letters->render(GL_TRIANGLES);

	// Disable shader
	shader->disable();

	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);

	// Show time and money values
	std::string time_string = std::to_string(Game::instance->curr_time);
	time_string.erase(time_string.find('.') + 3, std::string::npos);
	drawText(420 * Game::instance->window_width / 800., 200 * Game::instance->window_height / 600., time_string, Vector3(1, 1, 1), 6);
	std::string cash_string = std::to_string(Game::instance->last_loot);
	cash_string.erase(cash_string.find('.') + 3, std::string::npos);
	drawText(420 * Game::instance->window_width / 800., 285 * Game::instance->window_height / 600., cash_string, Vector3(1, 1, 1), 6);
}

void StageEnd::update(float dt)
{
	if (Input::wasKeyPressed(SDL_SCANCODE_O) || Input::gamepads[0].wasButtonPressed(A_BUTTON))
	{
		Game::instance->audio->stopAudio();
		Game::instance->audio->playAudio("data/sounds/title.wav");
		Game::instance->stages[STAGE_INTRO]->initStage();
		Game::instance->current_stage = Game::instance->stages[STAGE_INTRO];
		Game::instance->camera->lookAt(Vector3(7.f, 15.f, 15.f), Vector3(7.f, 13.f, 0.f), Vector3(0.f, 1.f, 0.f)); // Set the camera position
	}
}

void StageEnd::initStage()
{
	// First compute the score in order to show diferent quads
	computeScore();
	initQuads();
}

void StageEnd::initQuads()
{
	// We create a quad for the title screen by defining 2 triangles
	back = new Mesh();
	back->createQuad(0, 0, 2, 2, false);

	// We create a quad for the title screen by defining 2 triangles
	alpha_black = new Mesh();
	alpha_black->createQuad(0, -0.1, 1.6, 1.4, false);

	// We create a quad for the title screen by defining 2 triangles
	letters = new Mesh();

	// --- MISSION: EXIT ---
	// Three vertces of the first triangle
	letters->vertices.push_back(Vector3(-0.8, 0.75, 0));
	letters->uvs.push_back(Vector2(220. / 800., 512. / 600.));
	letters->vertices.push_back(Vector3(-0.8, 0.65, 0));
	letters->uvs.push_back(Vector2(220. / 800., 462. / 600.));
	letters->vertices.push_back(Vector3(-0.3, 0.65, 0));
	letters->uvs.push_back(Vector2(495. / 800., 462. / 600.));

	// Three vertices of the second triangle
	letters->vertices.push_back(Vector3(-0.8, 0.75, 0));
	letters->uvs.push_back(Vector2(220. / 800., 512. / 600.));
	letters->vertices.push_back(Vector3(-0.3, 0.65, 0));
	letters->uvs.push_back(Vector2(495. / 800., 462. / 600.));
	letters->vertices.push_back(Vector3(-0.3, 0.75, 0));
	letters->uvs.push_back(Vector2(495. / 800., 512. / 600.));

	// --- TIME ---
	// Three vertces of the first triangle
	letters->vertices.push_back(Vector3(-0.43, 0.33, 0));
	letters->uvs.push_back(Vector2(240. / 800., 600. / 600.));
	letters->vertices.push_back(Vector3(-0.43, 0.2, 0));
	letters->uvs.push_back(Vector2(240. / 800., 562. / 600.));
	letters->vertices.push_back(Vector3(-0.12, 0.2, 0));
	letters->uvs.push_back(Vector2(299. / 800., 562. / 600.));

	// Three vertices of the second triangle
	letters->vertices.push_back(Vector3(-0.43, 0.33, 0));
	letters->uvs.push_back(Vector2(240. / 800., 600. / 600.));
	letters->vertices.push_back(Vector3(-0.12, 0.2, 0));
	letters->uvs.push_back(Vector2(299. / 800., 562. / 600.));
	letters->vertices.push_back(Vector3(-0.12, 0.33, 0));
	letters->uvs.push_back(Vector2(299. / 800., 600. / 600.));

	// --- MONEY ---
	// Three vertces of the first triangle
	letters->vertices.push_back(Vector3(-0.45, 0.05, 0));
	letters->uvs.push_back(Vector2(300. / 800., 600. / 600.));
	letters->vertices.push_back(Vector3(-0.45, -0.1, 0));
	letters->uvs.push_back(Vector2(300. / 800., 561. / 600.));
	letters->vertices.push_back(Vector3(-0.1, -0.1, 0));
	letters->uvs.push_back(Vector2(390. / 800., 561. / 600.));

	// Three vertices of the second triangle
	letters->vertices.push_back(Vector3(-0.45, 0.05, 0));
	letters->uvs.push_back(Vector2(300. / 800., 600. / 600.));
	letters->vertices.push_back(Vector3(-0.1, -0.1, 0));
	letters->uvs.push_back(Vector2(390. / 800., 561. / 600.));
	letters->vertices.push_back(Vector3(-0.1, 0.05, 0));
	letters->uvs.push_back(Vector2(390. / 800., 600. / 600.));

	// --- S, $ ---
	// Three vertces of the first triangle
	letters->vertices.push_back(Vector3(0.63, 0.35, 0));
	letters->uvs.push_back(Vector2(780. / 800., 600. / 600.));
	letters->vertices.push_back(Vector3(0.63, -0.12, 0));
	letters->uvs.push_back(Vector2(780. / 800., 501. / 600.));
	letters->vertices.push_back(Vector3(0.7, -0.12, 0));
	letters->uvs.push_back(Vector2(800. / 800., 501. / 600.));

	// Three vertices of the second triangle
	letters->vertices.push_back(Vector3(0.63, 0.35, 0));
	letters->uvs.push_back(Vector2(780. / 800., 600. / 600.));
	letters->vertices.push_back(Vector3(0.7, -0.12, 0));
	letters->uvs.push_back(Vector2(800. / 800., 501. / 600.));
	letters->vertices.push_back(Vector3(0.7, 0.35, 0));
	letters->uvs.push_back(Vector2(800. / 800., 600. / 600.));

	// --- GRADE ---
	// Three vertces of the first triangle
	letters->vertices.push_back(Vector3(-0.55, -0.4, 0));
	letters->uvs.push_back(Vector2(90. / 800., 562. / 600.));
	letters->vertices.push_back(Vector3(-0.55, -0.6, 0));
	letters->uvs.push_back(Vector2(90. / 800., 512. / 600.));
	letters->vertices.push_back(Vector3(-0.05, -0.6, 0));
	letters->uvs.push_back(Vector2(200. / 800., 512. / 600.));

	// Three vertices of the second triangle
	letters->vertices.push_back(Vector3(-0.55, -0.4, 0));
	letters->uvs.push_back(Vector2(90. / 800., 562. / 600.));
	letters->vertices.push_back(Vector3(-0.05, -0.6, 0));
	letters->uvs.push_back(Vector2(200. / 800., 512. / 600.));
	letters->vertices.push_back(Vector3(-0.05, -0.4, 0));
	letters->uvs.push_back(Vector2(200. / 800., 562. / 600.));

	// --- A, B, C ---
	// Three vertces of the first triangle
	letters->vertices.push_back(Vector3(0.15, -0.3, 0));
	letters->uvs.push_back(Vector2((score * 30. - 30.) / 800., 561. / 600.));
	letters->vertices.push_back(Vector3(0.15, -0.7, 0));
	letters->uvs.push_back(Vector2((score * 30. - 30.) / 800., 513. / 600.));
	letters->vertices.push_back(Vector3(0.35, -0.7, 0));
	letters->uvs.push_back(Vector2((score * 30. - 1) / 800., 513. / 600.));

	// Three vertices of the second triangle
	letters->vertices.push_back(Vector3(0.15, -0.3, 0));
	letters->uvs.push_back(Vector2((score * 30. - 30.) / 800., 561. / 600.));
	letters->vertices.push_back(Vector3(0.35, -0.7, 0));
	letters->uvs.push_back(Vector2((score * 30. - 1) / 800., 513. / 600.));
	letters->vertices.push_back(Vector3(0.35, -0.3, 0));
	letters->uvs.push_back(Vector2((score * 30. - 1) / 800., 561. / 600.));
}

void StageEnd::computeScore()
{
	// First compute the score
	score = Game::instance->last_loot / Game::instance->curr_time;
	
	// Then set to 1, 2 or 3
	if (Game::instance->curr_time < 13 || Game::instance->last_loot == 0) score = 3;
	else if (score > 15) score = 1;
	else if (score < 15 && score > 8) score = 2;
	else if (score < 8) score = 3;
}
