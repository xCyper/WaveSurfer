#pragma once

namespace Tmpl8 {

class Surface;


class Game
{
private:
	
public:
	class SHARK
	{
	public:
		int x, y, width, height;
		int speed = 11;
		void MoveShark();
	};
	class OBSTACLE
	{
	public:
		int x, y, top, bottom, x_o;
		int speed = 7;
		void MoveObstacle();
	};
	class SURFER
	{
	public:
		int x = 100;
		int y = 300;
		int width = 34;
		int height = 52;
		int lift = -15;
		double velocity = 0;
		double gravity = 1.2;
		void ApplyGravity();
	};
	void CreateObstacles();
	static void Menu();
	static void ResetVars();
	void PrintHighScore();
	void DrawMenu();
	void DrawObjects(float deltaTime);
	void DrawObstacles();
	void DrawSurfer();
	void DrawBackground();
	void Start();
	void DrawShark();
	void PrintScore();
	void InitVars();
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	bool TickCounter(float deltaTime);
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown(int button);
	void MouseMove(int x, int y) {}
	void KeyUp( int key ) { /* implement if you want to handle keys */ }
	void KeyDown(int key);
	Surface* screen;
};

}; // namespace Tmpl8