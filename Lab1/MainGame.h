#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "Display.h" 
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "transform.h"
#include "Audio.h"
#include "SkyBox.h"

enum class GameState{PLAY, EXIT};

class MainGame
{
public:
	MainGame();
	~MainGame();

	void run();

private:

	void initSystems(); //**Get rid of unnessary links**
	void processInput();
	void gameLoop();
	void drawGame();
	void linkBumpMapping();
	void linkFogShader();
	void linkToon();
	void linkRimShader();
	void linkGeoShader();
	void linkWhat();
	bool collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad);
	//void playAudio(unsigned int Source, glm::vec3 pos);

	Display _gameDisplay;
	GameState _gameState;
	Mesh mesh1;
	Mesh mesh2;
	Camera myCamera;
	Shader fogShader;
	Shader toonShader;
	Shader rimShader;
	Shader bump;
	Shader noBump;
	Shader what;
	Shader geomShader;

	Skybox skybox;

	Texture texture1;
	Texture texture2;
	Texture bumpMapping;
	
	//Audio audioDevice;

	float counter;
	unsigned int whistle;
	unsigned int backGroundMusic;
};

