#include "world.h"
#include "mesh.h"
#include "texture.h"
#include "shader.h"
#include "entity.h"
#include "camera.h"
#include "game.h"

World* World::instance = NULL;

World::World()
{
	instance = this;
	time = 0.0f;
}

World::~World()
{

}

void World::locateProps()
{
	Shader* basic_shader = Shader::Get("data/shaders/basic.vs", "data/shaders/texture.fs");
	Mesh* house_mesh = Mesh::Get("data/house/house.obj");
	Mesh* plane_mesh = new Mesh();

	// Create the characters of the world 
	addProp(Vector3((world_size + 6 * cell_size)/2, 0, (world_size + 6 * cell_size)/2), CHARACTER, Mesh::Get("data/characters/male.mesh"), Texture::Get("data/characters/robber.tga"), Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs"), Vector3(0.5, 0.5, 0.5), 0, PLAYER, 0);
	addProp(Vector3((world_size + 6 * cell_size)/2, 0, (world_size + 6 * cell_size)/2 + 40), CHARACTER, Mesh::Get("data/characters/male.mesh"), Texture::Get("data/characters/police.tga"), Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs"), Vector3(0.5, 0.5, 0.5), 0, COP, 0.3);

	float rad_scale = cell_size / house_mesh->radius;
	house_mesh->radius = cell_size;
		
	for (int x = round(cell_size/2); x < floor(world_size + 6 * cell_size); x += cell_size)
	{
		for (int y = round(cell_size / 2); y < floor(world_size + 6 * cell_size); y += cell_size)
		{
			double density = prop_density;

			if (x < cell_size * 3 || x > floor(world_size) + cell_size * 3 || y < cell_size * 3 || y > floor(world_size) + cell_size * 3)
				density = 1;
			else if (abs(x - characters[0]->model.getTranslation().x) < cell_size || abs(y - characters[0]->model.getTranslation().z) < cell_size)
				continue;

			Vector3 position = Vector3(x, 0., y);

			if (random() < density)
			{
				addProp(position, STATIC_PROP, Mesh::Get("data/house/house.obj"), Texture::Get("data/house/materials.tga"), basic_shader, Vector3(rad_scale, 1, rad_scale));
			}
			else if (random() < density * density)
			{
				float cash = random(2.0) * 50;
				if (random() < 0.5)
					addProp(position, CHARACTER, Mesh::Get("data/characters/male.mesh"), Texture::Get("data/characters/male.tga"), Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs"), Vector3(0.5, 0.5, 0.5), cash, NPC, 0.2);
				else
					addProp(position, CHARACTER, Mesh::Get("data/characters/female.mesh"), Texture::Get("data/characters/female.tga"), Shader::Get("data/shaders/skinning.vs", "data/shaders/texture.fs"), Vector3(0.5, 0.5, 0.5), cash, NPC, 0.2);
				}
		}
	}

}

void World::addProp(Vector3 position, int ent_type, Mesh* mesh, Texture* texture, Shader* shader, Vector3 scale, float cash, int character_type, float speed)
{
	EntityMesh* ent;

	if (ent_type == CHARACTER)
	{
		ent = new Character();
		((Character*)ent)->dynamic = true;
		((Character*)ent)->type = character_type;
		((Character*)ent)->dt_speed = speed;
		((Character*)ent)->yaw = random(2*M_PI);
		((Character*)ent)->cash = cash;
		characters.push_back((Character*)ent);
	}
	else if (ent_type == STATIC_PROP)
		ent = new EntityMesh();

	ent->textures.push_back(texture);
	ent->meshes.push_back(mesh);
	ent->meshes[0]->createCollisionModel();
	ent->shader = shader;
	ent->model.translate(position.x, position.y, position.z);
	ent->model.scale(scale.x, scale.y, scale.z);
	
	props.push_back(ent);

}

void World::initialize()
{
	// Create the plane
	EntityMesh* plane = new EntityMesh();
	Mesh* mesh = new Mesh();
	mesh->createSubdividedPlane(world_size + cell_size * 6, 32, true);
	plane->meshes.push_back(mesh);
	plane->textures.push_back(Texture::Get("data/terrain/suelo.tga"));
	plane->shader = Shader::Get("data/shaders/plane.vs", "data/shaders/texture.fs");

	// Store the props in the container
	props.push_back(plane);

	// Create and locate some objects of the world
	locateProps();
}

void World::renderWorld(Camera* camera)
{
	props[0]->render(camera);

	// Loop for render all the props in the world
	for (int i = 1; i < props.size(); ++i)
	{
		// Frustum check
		// Compute bounding sphere center in world coordinates
		Vector3 sphere_center = props[i]->model * props[i]->meshes[0]->box.center;
		float sphere_radius = props[i]->meshes[0]->radius;

		if (props[i]->isCharacter()) sphere_radius = 15;
		
		// Discard objects whose bounding sphere is not inside the camera frustum
		if (!camera->testSphereInFrustum(sphere_center, sphere_radius))
		{
			if (props[i]->isCharacter())
				((Character*)props[i])->visible = false;

			continue;
		}
		if (props[i]->isCharacter())
			((Character*)props[i])->visible = true;

		props[i]->render(camera);	
	}
}

void World::updateWorld(float dt)
{
	time += dt;
	// Loop for update all the entities in the world

	for (int i = 0; i < props.size(); ++i)
	{
		// There is only need to update the entities if they are dynamic
		if (props[i]->dynamic)
		{
			props[i]->update(dt);
			if (props[i]->isCharacter())
			{
				if (((Character*)props[i])->type == COP)
					((Character*)props[i])->follow(characters[0]);
			}
			
			// --- Check collisions ---
			// Init some vars
			Vector3 incident_dir = (((Character*)props[i])->prev_pos - ((Character*)props[i])->model.getTranslation() + Vector3(0.000001, 0.000001, 0.000001)).normalize();

			Vector3 coll_point;
			Vector3 coll_normal;
			Vector3 character_center = props[i]->model.getTranslation() + Vector3(0., 5., 0.);

			for (int j = 1; j < props.size(); j++)
			{
				// If its checking with itself do not test it
				if (props[j]->model.getTranslation().distance(props[i]->model.getTranslation()) < 0.000001) {
					continue;
				}
				// First we do a general filter in order to optimize the computation time
				if (props[j]->model.getTranslation().distance(character_center) > 50)
					continue;
				
				// Store the mesh of the prop
				Mesh* mesh_aux = props[j]->meshes[0];

				// Finally test the collision
				if (mesh_aux->testSphereCollision(props[j]->model, character_center, 3., coll_point, coll_normal))
				{
					if (mesh_aux->testRayCollision(props[j]->model, character_center, incident_dir, coll_point, coll_normal, 1))
					{
						if (dot(incident_dir, coll_normal) < 0)
							coll_normal = Vector3(0,0,0);
					}
				}

				if (character_center.x < 3.5 * cell_size || character_center.z < 3.5 * cell_size || character_center.x > (world_size + 3 * cell_size) || character_center.z > (world_size + 3 * cell_size))
				{
    			coll_normal = (character_center.x < 4 * cell_size) * Vector3(1, 0, 0) + (character_center.z < 4 * cell_size) * Vector3(0, 0, 1) +
						(character_center.x > world_size + 3 * cell_size) * Vector3(-1, 0, 0) + (character_center.z > world_size + 3 * cell_size) * Vector3(0, 0, -1);
				}

				if (coll_normal.distance(Vector3(0, 0, 0)) > 0.1)
				{
					Vector3 reflected_dir = reflectDirection(incident_dir, coll_normal);
					((Character*)props[i])->speed /= 0.90;	// reflect a bit further
					((Character*)props[i])->move(reflected_dir);
					((Character*)props[i])->speed *= 0.90;
					break;
				}
			}
			
		}
	}
}

Vector3 World::reflectDirection(Vector3 incident_dir, Vector3 normal)
{
	normal = normal.normalize();
	Vector3 reflected_dir = (2 * dot(normal, incident_dir) * normal - incident_dir);
	reflected_dir.y = 0;
	reflected_dir.normalize();

	return reflected_dir;
}

void World::renderMiniMap(Camera* camera)
{
	props[0]->render(camera);

	// Loop for render all the props in the world
	for (int i = 1; i < props.size(); ++i)
	{
		if (props[i]->isCharacter() == true)
		{
			// Instead of showing the character mesh, we will show a diamond
			EntityMesh* cubo = new EntityMesh();
			Mesh* mesh = new Mesh();
			mesh->createQuad(((world_size + 6*cell_size - props[i]->model.getTranslation().x) / ((world_size + 6 * cell_size )/2) - 1)* 0.75 * pow(800.0 / Game::instance->window_width, 0.40), (props[i]->model.getTranslation().z / ((world_size + 6 * cell_size) / 2) - 1)* 1.02 * pow((600.0 / Game::instance->window_height), 0.025), 0.09, 0.12, false);
			cubo->meshes.push_back(mesh);
			cubo->shader = Shader::Get("data/shaders/quad.vs", "data/shaders/texture.fs");

			glDisable(GL_CULL_FACE);
			glDisable(GL_DEPTH_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			cubo->shader->enable();
			cubo->shader->setUniform("u_color", Vector4(1, 1, 1, 1));

			// Depending on the character type, we will show diferents images
			if (((Character*)props[i])->type == PLAYER)
				cubo->shader->setUniform("u_texture", Texture::Get("data/gui_hud/player_box.tga"));
			else if (((Character*)props[i])->type == NPC)
			{
				if (((Character*)props[i])->cash == 0)
					cubo->shader->setUniform("u_texture", Texture::Get("data/gui_hud/png_stolen_box.tga"));
				else
					cubo->shader->setUniform("u_texture", Texture::Get("data/gui_hud/png_box.tga"));
			}
			else if (((Character*)props[i])->type == COP)
				cubo->shader->setUniform("u_texture", Texture::Get("data/gui_hud/cop_box.tga"));

			cubo->meshes[0]->render(GL_TRIANGLES);
			cubo->shader->disable();

			glDisable(GL_BLEND);
			glEnable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);
		}
		else
		{
			props[i]->render(camera);
		}
	}
}