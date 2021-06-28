#include "entity.h"
#include "game.h"
#include "utils.h"
#include "mesh.h"
#include "world.h"
#include "texture.h"
#include "fbo.h"
#include "shader.h"
#include "input.h"
#include "animation.h"


Entity::Entity()
{

}

Entity::~Entity()
{

}

void Entity::render(Camera* camera)
{

}

void Entity::update(float dt)
{

}

bool Entity::isCharacter() const
{
	return false;
}


// -------------------- ENTITY MESH -------------------- //

EntityMesh::EntityMesh()
{
	
}

EntityMesh::~EntityMesh()
{

}

void EntityMesh::render(Camera* camera)
{
	//enable shader
	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1,1,1,1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix );
	for (int i = 0; i < textures.size(); ++i)	//send many textures if the case is given
	{
		std::string uniform_name = "u_texture_";
		uniform_name.push_back(std::to_string(i + 1).at(0));
		const char* u_name = uniform_name.c_str();
		shader->setUniform(u_name, textures[i]);
	}
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Game::instance->time);
	
	//do the draw call
	meshes[0]->render( GL_TRIANGLES );

	//disable shader
	shader->disable();
}

void EntityMesh::update(float dt)
{

}

bool EntityMesh::isCharacter() const
{
	return false;
}


// -------------------- CHARACTER -------------------- //

Character::Character()
{

}

Character::~Character()
{

}

void Character::render(Camera* camera)
{
	if (!bone_matrices.size())
		return;

	//enable shader
	shader->enable();

	//upload uniforms
	shader->setUniform("u_color", Vector4(1, 1, 1, 1));
	shader->setUniform("u_viewprojection", camera->viewprojection_matrix);
	shader->setUniform("u_texture", textures[0]);
	shader->setUniform("u_model", model);
	shader->setUniform("u_time", Game::instance->time);
	shader->setUniform("u_bones", bone_matrices);

	//do the draw call
	meshes[0]->render(GL_TRIANGLES);

	//disable shader
	shader->disable();
}

void Character::update(float dt)
{
	prev_pos = model.getTranslation();		// store previous position of this instance

	float time_step = dt * 85;		// refresh factor

	speed = min(speed, 1);		// clamp the speed to 1, and decrease it 10%.
	speed *= 0.90;

	yaw = fmod(yaw, 2 * M_PI);	// redefine the yaw between [0, 2*PI]

	if (type == PLAYER)
	{

		bool moved = false;

		if (Input::isKeyPressed(SDL_SCANCODE_LSHIFT) || Input::gamepads[0].isButtonPressed(RB_BUTTON)) speed *= 1.08;	//speed boost
		if (Input::isKeyPressed(SDL_SCANCODE_W) || Input::isKeyPressed(SDL_SCANCODE_UP) || (Input::gamepads[0].direction & PAD_UP))
		{
			yaw = lerp(0, yaw, 0.5) * (yaw < M_PI) + lerp(2 * M_PI, yaw, 0.5) * (yaw >= M_PI);		// Interpolate between current yaw and the desired yaw using 0.5 factor and
																									// deciding the closest direction to rotate using the boolean factors.
			speed += 0.05 * !moved;		// increase speed only if none of the other direction triggers have increased it yet (not useful for this first direction but yes for the others)
			moved = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_S) || Input::isKeyPressed(SDL_SCANCODE_DOWN) || (Input::gamepads[0].direction & PAD_DOWN))
		{
			yaw = lerp(M_PI, yaw, 0.5);
			speed += 0.05 * !moved;
			moved = true;
		}
		if (Input::isKeyPressed(SDL_SCANCODE_A) || Input::isKeyPressed(SDL_SCANCODE_LEFT) || (Input::gamepads[0].direction & PAD_LEFT))
		{
			yaw = lerp(3 * M_PI / 2, yaw, 0.5) * (yaw > (M_PI / 2)) + lerp(3 * M_PI / 2, yaw + 2 * M_PI, 0.5) * (yaw < (M_PI / 2));
			speed += 0.05 * !moved;
			moved = true;
		}

		if (Input::isKeyPressed(SDL_SCANCODE_D) || Input::isKeyPressed(SDL_SCANCODE_RIGHT) || (Input::gamepads[0].direction & PAD_RIGHT))
		{
			yaw = lerp(M_PI / 2, yaw, 0.5) * (yaw < (3 * M_PI / 2)) + lerp(5 * M_PI / 2, yaw, 0.5) * (yaw > (3 * M_PI / 2));
			speed += 0.05 * !moved;
			moved = true;
		}
		Vector3 dir = Vector3(-sin(yaw), 0, cos(yaw)).normalize();		// define the direction in Vector3 using yaw (adapting it to the current space)
		dt_speed_player = speed * (time_step);		// make it dependent of the frame rate
		move(dir);		// move the character using dir direction and the instance's speed.
	}
	else
	{
		speed = min(1,time_step * dt_speed * min(World::instance->time, 1));	//for non player characters set the speed constant in terms of the frame rate (min is used to give acceleration at the beggining)

		if (type == NPC)
		{
			Vector3 dir = Vector3(-sin(yaw), 0, cos(yaw)).normalize();

			if (std::fmod(World::instance->time, 0.5) < 0.3)		// every given time lapse, give a random direction to NPC's within a small variance
			{
				dir = dir + Vector3((random() * 2 - 1)*0.2, 0, (random() * 2 - 1)*0.2);
			}
			move(dir);
		}
	}

	if (visible)	// if the character is inside the camera frustrum, update the skeleton
		updateSkeleton(dt);
}

void Character::move(Vector3 dir)
{
	Vector3 pos = model.getTranslation();
	
	model.setRotation(yaw, Vector3(0., 1., 0.));	// set the character model to identity and rotate it
	if (type == PLAYER)		// translate it using the current position stored and the speed using the refresh rate
		model.translateGlobal(dir.x * dt_speed_player + pos.x, dir.y * dt_speed_player + pos.y, dir.z * dt_speed_player + pos.z);
	else				// if it is not a player there is no need to use dt_speed_player because its speed already uses the refresh rate
		model.translateGlobal(dir.x * speed + pos.x, dir.y * speed + pos.y, dir.z * speed + pos.z);
	model.scale(0.5, 0.5, 0.5);

	// get the yaw of non player characters to rotate correctly
	if (type != PLAYER)
		yaw = atan(-dir.x / (dir.z + 0.01)) * (dir.z >= 0) + ( atan(-dir.x / (dir.z + 0.01)) + M_PI) * (dir.z < 0);
}

void Character::follow(Entity* ent)
{
	Vector3 dir = (ent->model.getTranslation() - this->model.getTranslation() + Vector3(0.00000001, 0.00000001, 0.00000001)).normalize();
	move(dir);
}

bool Character::isCharacter() const
{
	return true;
}

void Character::updateSkeleton(float dt)
{
	float t = World::instance->time;
	float w = 0.0;
	Vector3 vel = Vector3(-sin(yaw), 0, cos(yaw)).normalize();

	if (speed > 0.01)
	{
		Matrix44 R;
		R.rotate(-yaw, Vector3(0, 1, 0));
		vel = R * vel;
	}

	Animation* anim = NULL;
	Animation* animB = NULL;
	const char* idle_name = "data/characters/idle.skanim";

	if (speed < 0.01) // Idle
	{
		anim = Animation::Get(idle_name);
		anim->assignTime(t);
		sk = anim->skeleton;
	}
	else if (speed < 0.5) // Walk
	{
		anim = Animation::Get(idle_name);
		anim->assignTime(t);

		w = clamp(speed*2, 0, 1);

		animB = Animation::Get("data/characters/walking.skanim");
		animB->assignTime(t);

		blendSkeleton(&anim->skeleton, &animB->skeleton, w, &sk);
	}
	else // Run
	{
		w = clamp(speed - 0.5, 0, 1);

		anim = Animation::Get("data/characters/walking.skanim");
		anim->assignTime(t);
		
		animB = Animation::Get("data/characters/running.skanim");
		animB->assignTime((t / anim->duration) * animB->duration);

		blendSkeleton(&anim->skeleton, &animB->skeleton, w, &sk);
	}

	sk.computeFinalBoneMatrices(bone_matrices, meshes[0]);
}