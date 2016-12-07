/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();



/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();
	theScore = 0;
	theSpawn = { { 60, -400 }, { 160, -900 }, { 260, -600 }, { 360, -1124 }, { 460, -900 }, { 560, -1637 }, { 660, -1124 }, { 760, -1400 }, { 860, -500 }, { 60, -1900 }, { 160, -2400 }, { 260, -2100 }, { 360, -2624 }, { 460, -2400 }, { 560, -3137 }, { 660, -2624 }, { 760, -2900 }, { 860, -2000 } };

	// Store the textures
	textureName = { "knott1", "knott2", "knott3", "knott4", "bullet", "theRocket", "theBackground", "MenuBackground", "EndBackground" };
	texturesToUse = { "Images\\Enemy(1).png", "Images\\Enemy(2).png", "Images\\Enemy(3).png", "Images\\Enemy(4).png", "Images\\bullet.png", "Images\\Player(1).png", "Images\\Background.png", "Images\\MenuBackground.png", "Images\\EndBackground.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Create textures for Game Dialogue (text)
	fontList = { "digital", "Airstrike" };
	fontsToUse = { "Fonts/digital-7.ttf", "Fonts/Airstrike (1).ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 60);
	}
	gameTextNames = { "TitleTxt", "CreateTxt",  "ThanksTxt", "SeeYouTxt", "ScoreTxt", "MovementTxt", "FireTxt"};
	gameTextList = { "crossfire", "Kill all 18 Knots", "Thanks for playing", "Crossfire!", "Final Score:", "Movement", "Fire" };
	for (int text = 0; text < gameTextNames.size(); text++)
	{
		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("Airstrike")->createTextTexture(theRenderer, gameTextList[text], SOLID, { 255, 175, 42, 255 }, { 0, 0, 0, 0 }));
	}
	gameTextList = { "Crossfire                               Score:", "theScore" };
	
	theTextureMgr->addTexture("Title", theFontMgr->getFont("Airstrike")->createTextTexture(theRenderer, gameTextList[0], SOLID, { 255, 175, 42, 255 }, { 0, 0, 0, 0 }));

	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn", "exit_btn2" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png", "Images/Buttons/button_exit2.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 40, 570 }, { 740, 500 }, { 400, 300 }, { 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}

	theGameState = MENU;
	theBtnType = EXIT;

	// Load game sounds
	soundList = { "theme", "shot", "explosion", "button" };
	soundTypes = { MUSIC, SFX, SFX, SFX };
	soundsToUse = { "Audio/BackgroundMusic.wav", "Audio/shot.wav", "Audio/explosion2.wav", "Audio/Button.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	

	theCross.setSpritePos({ 500, 500 });
	theCross.setTexture(theTextureMgr->getTexture("theRocket"));
	theCross.setSpriteDimensions(theTextureMgr->getTexture("theRocket")->getTWidth(), theTextureMgr->getTexture("theRocket")->getTHeight());
	theCross.setCrossVelocity({ 0, 0 });

	// Create vector array of textures

	for (int knot = 0; knot < 18; knot++)
	{
		
			theKnotts.push_back(new cKnott);
			theKnotts[knot]->setSpritePos(theSpawn[knot]);
			theKnotts[knot]->setSpriteTranslation({ 0, 60 });
			int randKnott = rand() % 4;
			theKnotts[knot]->setTexture(theTextureMgr->getTexture(textureName[randKnott]));
			theKnotts[knot]->setSpriteDimensions(theTextureMgr->getTexture(textureName[randKnott])->getTWidth(), theTextureMgr->getTexture(textureName[randKnott])->getTHeight());
			theKnotts[knot]->setActive(true);
		
	}

}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
		case MENU:
		{
			spriteBkgd.setSpritePos({ 0, 0 });
			spriteBkgd.setTexture(theTextureMgr->getTexture("MenuBackground"));
			spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("MenuBackground")->getTWidth(), theTextureMgr->getTexture("MenuBackground")->getTHeight());
			spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
			// Render the Title
			tempTextTexture = theTextureMgr->getTexture("TitleTxt");
			pos = { 20, 15, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			tempTextTexture = theTextureMgr->getTexture("CreateTxt");
			pos = { 20, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			tempTextTexture = theTextureMgr->getTexture("MovementTxt");
			pos = { 200, 335, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			tempTextTexture = theTextureMgr->getTexture("FireTxt");
			pos = { 350, 435, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			// Render Button
			theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
			theButtonMgr->getBtn("exit_btn")->setSpritePos({ 40, 650 });
			theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
			
		}
		break;
		case PLAYING:
		{
			spriteBkgd.setSpritePos({ 0, 0 });
			spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
			spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());
			spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
			// Render each Knott in the vector array

			for (int draw = 0; draw < theKnotts.size(); draw++)
			{
				theKnotts[draw]->render(theRenderer, &theKnotts[draw]->getSpriteDimensions(), &theKnotts[draw]->getSpritePos(), theKnotts[draw]->getSpriteRotAngle(), &theKnotts[draw]->getSpriteCentre(), theKnotts[draw]->getSpriteScale());
			}
			// Render each bullet in the vector array
			for (int draw = 0; draw < theBullets.size(); draw++)
			{
				theBullets[draw]->render(theRenderer, &theBullets[draw]->getSpriteDimensions(), &theBullets[draw]->getSpritePos(), 
					theBullets[draw]->getSpriteRotAngle(), &theBullets[draw]->getSpriteCentre(), theBullets[draw]->getSpriteScale());
			}
			// Render the Title
			cTexture* tempTextTexture = theTextureMgr->getTexture("Title");
			SDL_Rect pos = { 20, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			FPoint scale = { 1, 1 };		
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			theButtonMgr->getBtn("exit_btn2")->setSpritePos({ 400, 20 });
			theButtonMgr->getBtn("exit_btn2")->render(theRenderer, &theButtonMgr->getBtn("exit_btn2")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn2")->getSpritePos(), theButtonMgr->getBtn("exit_btn2")->getSpriteScale());
			//Render the Scoreboard
			theTextureMgr->addTexture("theScore", theFontMgr->getFont("Airstrike")->createTextTexture(theRenderer, to_string(theScore).c_str(), SOLID, { 255, 175, 42, 255 }, { 0, 0, 0, 0 }));
			tempTextTexture = theTextureMgr->getTexture("theScore");
			pos = { 930, 20, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			// render the rocket
			theCross.render(theRenderer, &theCross.getSpriteDimensions(), &theCross.getSpritePos(), theCross.getSpriteRotAngle(), &theCross.getSpriteCentre(), theCross.getSpriteScale());
			SDL_RenderPresent(theRenderer);
			
			
		}
		break;
		case END:
		{
			spriteBkgd.setSpritePos({ 0, 0 });
			spriteBkgd.setTexture(theTextureMgr->getTexture("EndBackground"));
			spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("EndBackground")->getTWidth(), theTextureMgr->getTexture("EndBackground")->getTHeight());
			spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
			tempTextTexture = theTextureMgr->getTexture("ThanksTxt");
			pos = { 20, 15, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			tempTextTexture = theTextureMgr->getTexture("SeeYouTxt");
			pos = { 20, 80, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			tempTextTexture = theTextureMgr->getTexture("ScoreTxt");
			pos = { 50, 400, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
			theButtonMgr->getBtn("menu_btn")->setSpritePos({ 450, 650 });
			theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
			theButtonMgr->getBtn("exit_btn")->setSpritePos({ 40, 650 });
			theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
			theTextureMgr->addTexture("theScore", theFontMgr->getFont("Airstrike")->createTextTexture(theRenderer, to_string(theScore).c_str(), SOLID, { 255, 175, 42, 255 }, { 0, 0, 0, 0 }));
			tempTextTexture = theTextureMgr->getTexture("theScore");
			pos = { 400, 400, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
			scale = { 1, 1 };
			tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		}
		break;
		case QUIT:
		{
			loop = false;
		}
		break;
		default:
			break;
	}
	SDL_RenderPresent(theRenderer);
}



void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	
	if (theGameState == MENU || theGameState == END)
	{
		theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
	}
	else
	{
		theGameState = theButtonMgr->getBtn("exit_btn2")->update(theGameState, END, theAreaClicked);
	}
	theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
	theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
	
	

	// Update the visibility and position of each knott
	vector<cKnott*>::iterator knottIterator = theKnotts.begin();
	while (knottIterator != theKnotts.end())
	{
		if ((*knottIterator)->isActive() == false)
		{
			knottIterator = theKnotts.erase(knottIterator);
		}
		else
		{
			(*knottIterator)->update(deltaTime);
			++knottIterator;
		}
	}
	// Update the visibility and position of each bullet
	vector<cBullet*>::iterator bulletIterartor = theBullets.begin();
	while (bulletIterartor != theBullets.end())
	{
		if ((*bulletIterartor)->isActive() == false)
		{
			bulletIterartor = theBullets.erase(bulletIterartor);
		}
		else
		{
			(*bulletIterartor)->update(deltaTime);
			++bulletIterartor;
		}
	}
	/*
	==============================================================
	| Check for collisions
	==============================================================
	*/
	for (vector<cBullet*>::iterator bulletIterartor = theBullets.begin(); bulletIterartor != theBullets.end(); ++bulletIterartor)
	{
		//(*bulletIterartor)->update(deltaTime);
		for (vector<cKnott*>::iterator knottIterator = theKnotts.begin(); knottIterator != theKnotts.end(); ++knottIterator)
		{
			if ((*knottIterator)->collidedWith(&(*knottIterator)->getBoundingRect(), &(*bulletIterartor)->getBoundingRect()))
			{
				// if a collision set the bullet and knott to false
				(*knottIterator)->setActive(false);
				(*bulletIterartor)->setActive(false);
				theSoundMgr->getSnd("explosion")->play(0);
				theScore += 1;
				theTextureMgr->deleteTexture("theScore");
			}
		}
	}


	


	// Update the Rockets position
	theCross.update(deltaTime);
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;


	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
					theSoundMgr->getSnd("button")->play(0);

				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_RIGHT:
					if (theCross.getSpritePos().x < 900)
						{
							theCross.setSpritePos({ (theCross.getSpritePos().x + 10), theCross.getSpritePos().y });
						}
				break;
				case SDLK_LEFT:
					if (theCross.getSpritePos().x > 20)
						{
							theCross.setSpritePos({ (theCross.getSpritePos().x - 10), theCross.getSpritePos().y });
						}
				break;
				case SDLK_SPACE:
				{
					theBullets.push_back(new cBullet);
					int numBullets = theBullets.size() - 1;
					theBullets[numBullets]->setSpritePos({ theCross.getBoundingRect().x + theCross.getSpriteCentre().x - 8, theCross.getBoundingRect().y + theCross.getSpriteCentre().y - 20 });
					theBullets[numBullets]->setSpriteTranslation({ 4.0f, 4.0f });
					theBullets[numBullets]->setTexture(theTextureMgr->getTexture("bullet"));
					theBullets[numBullets]->setSpriteDimensions(theTextureMgr->getTexture("bullet")->getTWidth(), theTextureMgr->getTexture("bullet")->getTHeight());
					theBullets[numBullets]->setBulletVelocity({ 4.0f, 4.0f });
					theBullets[numBullets]->setSpriteRotAngle(theCross.getSpriteRotAngle());
					theBullets[numBullets]->setActive(true);
					cout << "Bullet added to Vector at position - x: " << theCross.getBoundingRect().x << " y: " << theCross.getBoundingRect().y << endl;
				}
				theSoundMgr->getSnd("shot")->play(0);
				break;
				default:
					break;
				}

			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

