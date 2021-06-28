#ifndef ENTITY_H
#define ENTITY_H

#include "includes.h"
#include "utils.h"
#include "framework.h"
#include "animation.h"
#include <algorithm>

class Shader;
class Animation;
class Camera;
class Mesh;
class Texture;

enum { PLAYER , NPC , COP };
enum { STATIC_PROP, CHARACTER, };

class Entity
{
public:
	Entity();
	virtual ~Entity();

	Matrix44 model;
	bool dynamic = false;

	virtual void render(Camera* camera);
	virtual void update(float dt);

	virtual bool isCharacter() const;
};


class EntityMesh : public Entity
{
public:
	EntityMesh();
	virtual ~EntityMesh();
	std::vector<Mesh*> meshes;
	std::vector<Texture*> textures;
	Shader* shader;

	virtual void render(Camera* camera);
	virtual void update(float dt);

	virtual bool isCharacter() const;
};


class Character : public EntityMesh
{
public:
	Character();
	virtual ~Character();
	std::vector<Animation*> animations;
	int type;
	float yaw = 0;
	float speed = 0;
	float dt_speed_player = 0;
	float dt_speed = 0;
	bool visible = true;
	Vector3 prev_pos;

	float cash = 0;

	float anim_time_start;
	Skeleton sk;
	std::vector<Matrix44> bone_matrices;

	virtual void render(Camera* camera);
	virtual void update(float dt);

	void move(Vector3 dir);
	void follow(Entity * ent);

	virtual bool isCharacter() const;

	void Character::updateSkeleton(float dt);
};

#endif