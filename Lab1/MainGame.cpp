#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
	//Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\Ball.obj");
	fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //new shader
	toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //new shader
	rimShader.init("..\\res\\Rim.vert", "..\\res\\Rim.frag");
	what.init("..\\res\\what.vert", "..\\res\\what.frag");
	bump.init("..\\res\\bump.vert", "..\\res\\bump.frag");
	noBump.init("..\\res\\noBump.vert", "..\\res\\noBump.frag");
	geomShader.initGeo("..\\res\\shaderGeoText.vert", "..\\res\\shaderGeoText.frag", "..\\res\\shaderGeoText.geom");

	texture1.load ("..\\res\\bricks.jpg"); //load texture
	texture2.load("..\\res\\rock.jpg"); //load texture
	bumpMapping.loadNormals("..\\res\\n.jpg");

	myCamera.initCamera(glm::vec3(2, 0, -4), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};

	skybox.init(faces);
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

//void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
//{
//	
//	ALint state; 
//	alGetSourcei(Source, AL_SOURCE_STATE, &state);
//	/*
//	Possible values of state
//	AL_INITIAL
//	AL_STOPPED
//	AL_PLAYING
//	AL_PAUSED
//	*/
//	if (AL_PLAYING != state)
//	{
//		audioDevice.playSound(Source, pos);
//	}
//}
void MainGame::linkRimShader()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	rimShader.setMat4("modelMatrix", transform.GetModel());
	//rimShader.setMat4("viewMatrix", myCamera.getView());
	rimShader.setFloat("rimPower", 3.0f);
	rimShader.setVec3("rimColor", glm::vec3(0.8f, 0.0f, 0.0f));
	rimShader.setVec3("camPos", myCamera.getPos());
}

void MainGame::linkFogShader()
{
	//fogShader.setMat4("u_pm", myCamera.getProjection());
	//fogShader.setMat4("u_vm", myCamera.getProjection());
	fogShader.setFloat("maxDist", 20.0f);
	fogShader.setFloat("minDist", 0.0f);
	fogShader.setVec3("fogColor", glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkToon()
{
	toonShader.setMat4("modelMatrix", transform.GetModel());
	toonShader.setVec3("lightDir", glm::vec3(0.5f, 0.5f, 0.5f));
}

void MainGame::linkWhat()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	what.setVec3("cameraPosition", myCamera.getPos());
	what.setFloat("atmosphereRadius", 1.3);
	what.setFloat("planetRadius", 5.5);
	what.setVec3("planetCenter", *transform.GetPos());
	what.setVec3("atmosphereColor", glm::vec4(0.5,0.5,0.5,0.1));
}
void MainGame::linkBumpMapping()
{
	GLuint t1L = glGetUniformLocation(bump.getID(), "diffuse"); //texture 1 location
	GLuint t2L = glGetUniformLocation(bump.getID(), "normalT");

	//set textures
	glActiveTexture(GL_TEXTURE0); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, texture2.getID());
	glUniform1i(t1L, 0);

	glActiveTexture(GL_TEXTURE1); //set acitve texture unit
	glBindTexture(GL_TEXTURE_2D, bumpMapping.getID());
	glUniform1i(t2L, 1);
}

void MainGame::linkGeoShader() 
{
	geomShader.setFloat("time", counter);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.8f, 0.8f, 0.8f, 1.0f); //sets our background colour
	skybox.draw(&myCamera);
	
	transform.SetPos(glm::vec3(-1.0, 0.0, 0.0));
	transform.SetRot(glm::vec3(0.0, counter, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	
	geomShader.Bind();		
	linkGeoShader();
	
	//Texture texture("..\\res\\bricks.jpg");



	//texture.Bind(0);
	geomShader.Update(transform, myCamera);


	mesh1.updateSphereData(*transform.GetPos(), 0.62f);		
	mesh1.draw();

	myCamera.setLook(*transform.GetPos());
	counter = counter + 0.02f;

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 