#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"
#include "world.h"
#include "stage.h"

#include <cmath>


// Some globals
float angle = 0;
Game* Game::instance = NULL;

Game::Game(int window_width, int window_height, SDL_Window* window)
{
	this->window_width = window_width;
	this->window_height = window_height;
	this->window = window;
	instance = this;
	must_exit = false;

	fps = 0;
	frame = 0;
	time = 0.0f;
	elapsed_time = 0.0f;
	mouse_locked = false;

	// OpenGL flags
	glEnable( GL_CULL_FACE ); // Render both sides of every triangle
	glEnable( GL_DEPTH_TEST ); // Check the occlusions using the Z buffer

	// Create our camera
	camera = new Camera();
	camera->lookAt(Vector3(7.f, 15.f, 15.f), Vector3(7.f, 13.f, 0.f), Vector3(0.f, 1.f, 0.f)); // Position the camera
	camera->setPerspective(70.f,window_width/(float)window_height,0.1f,10000.f); // Set the projection, we want to be perspective

	// Create stages
	stages.push_back(new StageTitle());
	stages.push_back(new StageIntro());
	stages.push_back(new StagePlay());
	stages.push_back(new StageControls());
	stages.push_back(new StagePause());
	stages.push_back(new StageEnd());

	// Choose the first default stage
	current_stage = stages[STAGE_TITLE];
	current_stage->initStage();
	
	// Hide the cursor
	SDL_ShowCursor(!mouse_locked); //hide or show the mouse

	// Init audio
	if (BASS_Init(1, 44100, 0, 0, NULL) == false)
	{
		//error abriendo la tarjeta de sonido...
		std::cout << "Error when opening the soundcard" << " \n";
	}

	audio = new Audio();
	Game::instance->audio->playAudio("data/sounds/title.wav");
}

// What to do when the image has to be draw
void Game::render(void)
{
	// Set the clear color (the background color)
	glClearColor(0.0, 0.0, 0.0, 1.0);

	// Clear the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the camera as default
	camera->enable();

	// Set flags
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
   
	// Create model matrix for cube
	Matrix44 m;
	m.rotate(time, Vector3(0, 1, 0));

	// Call only the render of the stage we are currently
	current_stage->render(camera);

	// Draw the floor grid
	// drawGrid();

	// Render the FPS, Draw Calls, etc
	if (settings == true) 
		drawText(2, 2, getGPUStats(), Vector3(1, 1, 1), 2);

	// Swap between front buffer and back buffer
	SDL_GL_SwapWindow(this->window);
}

void Game::update(double seconds_elapsed)
{
	// Update only the stage we currently are
	current_stage->update(seconds_elapsed);
}

// Keyboard event handler (sync input)
void Game::onKeyDown( SDL_KeyboardEvent event )
{
	switch(event.keysym.sym)
	{
		case SDLK_ESCAPE: must_exit = true; break; // ESC key, kill the app
		case SDLK_F1: Shader::ReloadAll(); break; 
		case SDLK_0: (settings == false) ? settings = true : settings = false; break;
	}
}

void Game::onKeyUp(SDL_KeyboardEvent event)
{
}

void Game::onGamepadButtonDown(SDL_JoyButtonEvent event)
{

}

void Game::onGamepadButtonUp(SDL_JoyButtonEvent event)
{

}

void Game::onMouseButtonDown( SDL_MouseButtonEvent event )
{
	if (event.button == SDL_BUTTON_MIDDLE) // Middle mouse
	{
		mouse_locked = !mouse_locked;
		SDL_ShowCursor(!mouse_locked);
	}
}

void Game::onMouseButtonUp(SDL_MouseButtonEvent event)
{
}

void Game::onMouseWheel(SDL_MouseWheelEvent event)
{
}

void Game::onResize(int width, int height)
{
    std::cout << "window resized: " << width << "," << height << std::endl;
	glViewport( 0,0, width, height );
	camera->aspect =  width / (float)height;
	window_width = width;
	window_height = height;
}