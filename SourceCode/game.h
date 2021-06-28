/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	This class encapsulates the game, is in charge of creating the game, getting the user input, process the update and render.
*/

#ifndef GAME_H
#define GAME_H

#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "audio.h"

class Stage;

enum { STAGE_TITLE , STAGE_INTRO , STAGE_PLAY, STAGE_CONTROLS , STAGE_PAUSE, STAGE_END };

class Game
{
public:
	static Game* instance;

	// Window
	SDL_Window* window;
	int window_width;
	int window_height;

	// Some globals
	long frame;
    float time;
	float elapsed_time;
	int fps;
	bool must_exit;

	double start_time = 0.; // Time when the stage_play started
	double curr_time = 0.; // Current time inside the stage_play
	double pause_time = 0.; // Time when we pressed pause
	float money = 0.; // Global money
	float last_loot = 0.; // Last loot stealed
	int previous_stage; // Indicates which was the last stage
	bool settings = false; // Flag to show the settings on the top

	// Audio
	Audio* audio;
	
	// Some vars
	std::vector<Stage*> stages; // Container of all the stages
	Stage* current_stage; // Points to the current stage
	Camera* camera; // Our global camera
	bool mouse_locked; // Tells if the mouse is locked (not seen)

	Game( int window_width, int window_height, SDL_Window* window );

	// Main functions
	void render( void );
	void update( double dt );

	// Events
	void onKeyDown( SDL_KeyboardEvent event );
	void onKeyUp(SDL_KeyboardEvent event);
	void onMouseButtonDown( SDL_MouseButtonEvent event );
	void onMouseButtonUp(SDL_MouseButtonEvent event);
	void onMouseWheel(SDL_MouseWheelEvent event);
	void onGamepadButtonDown(SDL_JoyButtonEvent event);
	void onGamepadButtonUp(SDL_JoyButtonEvent event);
	void onResize(int width, int height);
};


#endif 