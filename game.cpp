/* WaveSurfer
Made by Sam van der Hoeven for my Breda University of Applied Sciences (BUAS) application.
In this game you play as a surfer who has to surf the waves.
Avoid the obstacles to score points and beat your high score.
Press space bar to go up the waves and the gravity will pull you down.
The game is based on the game 'flappy bird'.
Artwork provided by a friend of mine, Wout van Galen, who drew everything.
*/

#include "game.h"
#include "surface.h"
#include <ctime> //time for srand()
#include <windows.h> //cursor / points 
#include <string>   //to_string()

namespace Tmpl8
{
	int obstacleHole, obstacleWidth, screenWidth, screenHeight, obstacleSpacing, score, highScore; 
	POINT cursor; //location of cursor on screen for clicking buttons
	float deltaTime_o; //for calculating tickrate
	double tickRate; //tickRate 
	bool running, hitbox; //booleans to see if game is running and whether to display hitbpx
	Sprite startButton(new Surface("assets/start.png"), 1); //sprite for startbutton
	Sprite surfer(new Surface ("assets/surfer.png"), 1); //sprite for surfer
	Sprite background(new Surface ("assets/background.png"), 1); //sprite for background
	Sprite shark(new Surface ("assets/shark.png"), 1); //sprite for shark
	Game::SURFER Surfer; //globally define surfer class
	Game::OBSTACLE Obstacle[6]; //globally define 7 obstacles (in case of bigger width)
	Game::SHARK Shark; //globally define shark

	void Game::SURFER::ApplyGravity()
	{
		Surfer.velocity += Surfer.gravity; //calculate velocity
		Surfer.y += Surfer.velocity; //double to int conversion, half pixels dont exist

		if (Surfer.y >= screenHeight - Surfer.height-1)//check is surfer hits floor
		{
			Surfer.y = screenHeight - Surfer.height-1; //place player at floor
			Surfer.velocity = 0; //reset velocity
		}else if (Surfer.y <= 0) //check if surfer hits roof
		{
			Surfer.velocity = 0; //stop moving up if surfer hits roof
			Surfer.y = 0;
		}
		return;
	}

	void Game::InitVars()//initialize variables on first use
	{
		score = 0;
		obstacleSpacing = 225; //how far are the obstacle apart
		tickRate = 31.25; //32 fps
		deltaTime_o = 0.f; //to calculate framerate
		obstacleHole = Surfer.height * 4; //size of the hole between obstacles
		obstacleWidth = 20; //width of the obstacles 
		screenHeight = screen->GetHeight(); //globally declare screen height for functions in other classes
		screenWidth = screen->GetWidth(); //globally declare screen width for functions in other classes
		hitbox = false; //globally declare whether to show hitbox
		highScore = 0;
		Shark.width = shark.GetWidth() * 1.5;
		Shark.height = shark.GetHeight() * 1.5;
		Shark.x = screenWidth - Shark.width;
		Shark.y = screenHeight;
		return;
	}

	void Game::ResetVars() //reset variables
	{
		Surfer.y = 300; 
		Surfer.velocity = 0;
		Shark.x = screenWidth - Shark.width;
		Shark.y = screenHeight;
		return;
	}

	void Game::Init() //initialize game
	{
		srand(time(0)); //set random
		InitVars(); //calculate all variables
		Menu(); //go to menu
		return;
	}
	
	void Game::Start() //start game after start button pressed
	{
		ShowCursor(false); //hide cursor when in game
		running = true; //start the game
		CreateObstacles(); //create the obstacles
		return;
	}

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
	}

	void Game::Menu() //back to menu
	{
		ResetVars(); //reset variables
		running = false; //stop running the gmae
		if(highScore < score) highScore = score; //set high score
		score = 0; //reset score
		ShowCursor(true); //show the cursor
		return;
	}

	void Game::MouseDown(int button)
	{
		if (button == 1)
		{
			GetCursorPos(&cursor); //get cursor position
			ScreenToClient(GetActiveWindow(), &cursor); //convert cursor position relative to game
			if (cursor.x >= screenWidth / 2 - 60 && cursor.x <= screenWidth / 2 - 60 + startButton.GetWidth() && cursor.y >= 200 && cursor.y <= 290)
				Start(); //if player clicks on start button start the game screenWidth / 2 - 7, 300, screenWidth / 2 + 7, 314,
			if (cursor.x >= screenWidth / 2 - 7 && cursor.x <= screenWidth / 2 + 7 && cursor.y >= 300 && cursor.y <= 314)
				hitbox = !hitbox;
		}
		return;
	}

	void Game::CreateObstacles()
	{
		for (int i = 0; i < screenWidth/obstacleSpacing; i++)
		{
			Obstacle[i].x = screenWidth + i * obstacleSpacing - obstacleWidth; //x coördinate of obstacles, all obstacles are spawned but some are outside of the screen
			Obstacle[i].y = 0; 
			Obstacle[i].top = rand() % (screenHeight - obstacleHole) + 1; //the top of the obstacles
			Obstacle[i].bottom = screenHeight - Obstacle[i].top - obstacleHole; //the bottom of the obstacle
		}

		return;
	}

	void Game::OBSTACLE::MoveObstacle()
	{
		x -= speed; //move obstacle towards surfer

		if (x <= 0) //if obstacle reaches end of screen
		{
			x = screenWidth - obstacleWidth-1; //move obstacle to the right of the screen
			top = rand() % (screenHeight - obstacleHole)+1; //calculate size of top
			bottom = screenHeight - top - obstacleHole; //calculate size of bottom
		}

		for (int i = 0; i < screenWidth / obstacleSpacing; i++)
		{
			if (Surfer.x + Surfer.width >= Obstacle[i].x && Surfer.x <= Obstacle[i].x + obstacleWidth) //if player x coördinate overlaps with obstacle
			{
				if (!(Surfer.y >= Obstacle[i].top && Surfer.y + Surfer.height < screenHeight - Obstacle[i].bottom)) 
					Menu(); //if player hits obstacle back to menu
				else
					if (Obstacle[i].x > Obstacle[i].x_o) //to make sure score doesnt get added multiple times
						score++; //if player doesnt hit obstacle add score
				Obstacle[i].x_o = Obstacle[i].x;
			}
		}

		return;
	}

	void Game::SHARK::MoveShark()
	{
		x -= speed; //move shark towards surfer
		if (y = screenHeight - Shark.height) y = rand() % 3 + 1 + screenHeight - Shark.height; //make it kinda wavey
		else y = screenHeight - Shark.height; //make it kinda wavey
		if (x <= 0) //if shark reaches end of screen
		{
			x = screenWidth - obstacleWidth - 1; //move shark to the right of the screen
		}
		
		if (Surfer.x + Surfer.width >= Shark.x && Surfer.x <= Shark.x + obstacleWidth && Surfer.y + Surfer.width >= Shark.y) //if player coördinates overlap with shark
			Menu(); //go back to menu
		return;
	}

	void Game::DrawShark()
	{
		shark.DrawScaled(Shark.x, Shark.y, Shark.width, Shark.height, screen); //draw shark
		//shark.Draw(screen, Shark.x, Shark.y); //draw shark
		if (hitbox)
			screen->Box(Shark.x, Shark.y, Shark.x + Shark.width, Shark.y + Shark.height, 0xffffff); //draw shark hitbox if enabled
		return;
	}

	void Game::PrintHighScore()
	{
		screen->Centre("High Score: ", 150, 0xffffff); //print text
		std::string strscore = std::to_string(highScore); //convert highscore to string
		//credit: https://www.techiedelight.com/convert-string-char-array-cpp/
		char* cstr = new char[strscore.length() + 1]; //create temporary char* for printing score
		strcpy(cstr, strscore.c_str()); //copy content of strstore to the char*
		screen->Print(cstr, screenWidth/2+35, 150, 0xffffff); //print highscore
		delete[] cstr; //cleanup
		return;
	}

	void Game::PrintScore()
	{
		std::string strscore = std::to_string(score); //convert score to string
		//credit: https://www.techiedelight.com/convert-string-char-array-cpp/
		char* cstr = new char[strscore.length() + 1]; //create temporary char* for printing score
		strcpy(cstr, strscore.c_str()); //copy content of strstore to the char*
		screen->Centre(cstr, 50, 0xffffff); //print score
		delete[] cstr; //cleanup
		return;
	}

	bool Game::TickCounter(float deltaTime)
	{
		deltaTime_o += deltaTime; //calculate elapsed time since last tick
		if (deltaTime_o < tickRate) return false; //if elapsed time is smaller then tickrate dont do anything
		deltaTime_o = 0; //if elapsed time is bigger then tickrate go on to the rest of the code and reset elapsed time

		return true;
	}

	void Game::KeyDown(int key)
	{
		if (key == 44)
		{
			Surfer.velocity = Surfer.lift; //move surfer up when pressing space
			if (!running) Start(); //start game byp ressing space
		}
		return;
	}

	void Game::DrawMenu()
	{
		startButton.Draw(screen, screenWidth / 2 - 60, 200); //if game is not running draw start button
		screen->Box(screenWidth / 2 - 7, 300, screenWidth / 2 + 7, 314, 0xffffff); //hitbox button
		screen->Centre("Show hitbox", 324, 0xffffff); //hitbox text
		if (hitbox)
		{
			screen->Line(screenWidth / 2 - 7, 300, screenWidth / 2 + 7, 314, 0xffffff); //hitbox cross
			screen->Line(screenWidth / 2 - 7, 314, screenWidth / 2 + 7, 300, 0xffffff); //hitbox cross
		}
		PrintHighScore();
		return;
	}

	void Game::DrawObstacles()
	{
		for (int i = 0; i < screenWidth / obstacleSpacing; i++)
		{
			if (Obstacle[i].x < (screenWidth - obstacleWidth))
			{
				screen->Bar(Obstacle[i].x, 0, Obstacle[i].x + obstacleWidth, Obstacle[i].top, 0x00ff00); //top of obstacle
				screen->Bar(Obstacle[i].x, screenHeight - Obstacle[i].bottom, Obstacle[i].x + obstacleWidth, screenHeight - 1, 0x00ff00); //bottom of obstacle
			}
		}
		return;
	}

	void Game::DrawSurfer()
	{
		surfer.Draw(screen, Surfer.x - 50, Surfer.y - 32); //draw surfer, -50 and -32 so that only surfer counts as hit not the water
		if(hitbox)
			screen->Box(Surfer.x, Surfer.y, Surfer.x + Surfer.width, Surfer.y + Surfer.height, 0x000000); //draw hitbox
		return;
	}

	void Game::DrawBackground()
	{
		background.Draw(screen, 0, 0); //draw background image 
		return;
	}

	void Game::DrawObjects(float deltaTime)
	{
		DrawBackground(); //draw background
		DrawSurfer(); //draw surfer
		DrawObstacles(); //draw obstacles
		DrawShark(); //draw shark
		PrintScore(); //print score

		if (!TickCounter(deltaTime)) return; //check if next tick needs to happen
		Surfer.ApplyGravity(); //apply gravity to the player
		Shark.MoveShark();
		for (int i = 0; i < screenWidth / obstacleSpacing; i++)
			Obstacle[i].MoveObstacle(); //move the obstacles
		return;
	}

	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
        screen->Clear(0); // clear the graphics window

		if (!running) //check if game is running or in menu
		{
			DrawMenu(); // draw the menu
			return; //return and do not carry out the rest of the code
		}

		DrawObjects(deltaTime); // draw the objects of the game
		return;
	}
};