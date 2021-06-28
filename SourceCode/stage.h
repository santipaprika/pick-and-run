#ifndef STAGE_H
#define STAGE_H

#include "includes.h"
#include "camera.h"
#include "utils.h"
#include "mesh.h"
#include "shader.h"
#include "pathfinders.h"
#include "animation.h"
#include "texture.h"

class World;
class Character;
class EntityMesh;

class Stage
{
public:
	Stage(); //constructor
	virtual ~Stage(); //destructor

	virtual void render(Camera* camera);
	virtual void update(float dt);
	virtual void initStage();

};


class StageTitle : public Stage
{
public:
	void render(Camera* camera);
	void update(float dt);
	void initStage();
	void initQuads();

	// PENDENT POSAR EN CLASE GUI
	Shader* shader;
	Mesh* back;
	Mesh* letters;
};

class StageIntro : public Stage
{
public:
	void render(Camera* camera);
	void update(float dt);
	void initStage();
	void initQuads();

	Shader* shader;
	Mesh* mesh;
	Mesh* quad;
	Animation* animation;
};

class StagePlay : public Stage
{
public:
	void render(Camera* camera);
	void update(float dt);
	void initStage();
	void initQuads();
	void initWorld();

	Character* closestPersonToSteal();

	//int * findPath(EntityMesh * prop_moving, EntityMesh * prop_target);

	World* world;
	Character* current_player;
	Character* current_cop;
	Camera* camera;
	Shader* shader;
	Mesh* quad;
};

class StageControls : public Stage
{
public:
	void render(Camera* camera);
	void update(float dt);
	void initStage();
	void initQuads();

	Shader* shader;
	Mesh* quad;
};

class StagePause : public Stage
{
public:
	void render(Camera* camera);
	void update(float dt); 
	void initStage();
	void initQuads();

	Shader* shader;
	Mesh* quad;
};

class StageEnd : public Stage
{
public:
	void render(Camera* camera);
	void update(float dt);
	void initStage();
	void initQuads();
	void computeScore();

	Shader* shader;
	Mesh* back;
	Mesh* alpha_black;
	Mesh* letters;
	double score;
};


#endif