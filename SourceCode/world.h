#ifndef WORLD_H
#define WORLD_H

#include "includes.h"
#include "utils.h"
#include "input.h"
#include "entity.h"
#include "framework.h"

enum { MAX_COPS = 2 };

//class Entity;
class Mesh;
class Texture;
class Shader;
//class EntityMesh;
//class Character;
class Camera;

class World
{
public:
	static World* instance;

	World(); //constructor
	virtual ~World(); //destructor

	void locateProps();
	void addProp(Vector3 position, int ent_type, Mesh * mesh, Texture * texture, Shader * shader, Vector3 scale = Vector3(1, 1, 1), float cash = 50., int character_type = NPC, float speed = 0.2);
	void initialize();

	void renderWorld(Camera* camera);
	void updateWorld(float dt);

	Vector3 reflectDirection(Vector3 incident_dir, Vector3 normal);

	void renderMiniMap(Camera * camera);

	//attributes
	float time;
	float world_size = 1024;
	int cell_size = 32;
	const double prop_density = 0.15;
	std::vector<Entity*> entities;
	std::vector<EntityMesh*> props;
	std::vector<Character*> characters;

	//map info
	uint8* map;

};

#endif