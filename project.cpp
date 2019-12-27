// Title: bounce.cpp
// Author: Reno Domel
// Date: 12/3/19
// Description: Main driver for Brickbreaker

#include <iostream>
#include "gfx.h"
#include <math.h>
#include <cmath>
#include <vector>
#include "brick.h"
#include <cstdlib>
#include <unistd.h>
using namespace std;


const int r = 5; // radius of ball

void drawBar(int, int, int, int);
void reCalculateVelocities(float &, float &, float);

int main()
{

	// initialize height and other variables
	vector <Brick> bricks; // will store brick objects
	int wd = 735; //window height
	int ht = 400; // window width
	char c = 0; // for user input


	int BARHEIGHT = 6;
	int BARWIDTH = 120;
	float barCenX = wd/2; // location of center of bar
	const float barCenY = ht - BARHEIGHT/2 - 5; // location of center of bar
	float barVX = 30; // how fast the bar moves when keys pressed

	// x and y values of bar edges
	float barLeftEdge = barCenX-BARWIDTH/2;
	float barRightEdge = barCenX+BARWIDTH/2;
	float barTopEdge = barCenY-BARHEIGHT/2;
	float barBottomEdge = barCenY+BARHEIGHT/2;


	int pausetime = 20000; // pause in between drawing
	srand(time(0)); // random launch for ball at start

	// draw three rows of bricks
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 10 ; i++)
		{
			Brick b1(abs(j-3), 70 * i + 35, 50 + 50 * j);
			bricks.push_back(b1);
		}
	}

	float xm = wd/2; // x position of ball to start
	float ym =  ht - BARHEIGHT/2 - 40; // y position of ball
	float vx = rand() % 3 - 1.5; // for moving the ball in x direction
	float vy = -1 * pow(pow(4.25,2) - pow(vx,2), 0.5); // for moving the ball in y direction

	bool noWinYet = true; // boolean for if player won
	bool gameLost = false; // boolean for if player lost




	// open window
	gfx_open(wd, ht, "BrickBreaker");
	gfx_color(255,255,255);


	bool gameStart = false; // holds ball in place until space bar is hit

	// show instruction screen until user quits or goes to game screen
	while (c != 'n' && c != 'q')
	{
		gfx_text(wd/2 - 160, ht/2 - 40, "WELCOME TO BRICKBREAKER");
		gfx_text(wd/2 - 160, ht/2 - 20, "Instructions:");
		gfx_text(wd/2 - 160, ht/2, "-Destroy all the bricks without the ball hitting the ground");
		gfx_text(wd/2 - 160, ht/2 + 20, "-Press 'a' to move left");
		gfx_text(wd/2 - 160, ht/2 + 40, "-Press 'd' to move right");
		gfx_text(wd/2 - 160, ht/2 + 60, "-You cannot hold 'a' or 'd'");
		gfx_text(wd/2 - 160, ht/2 + 80, "-Press SPACE to launch the ball the first time");
		gfx_text(wd/2 - 160, ht/2 + 100, "-Hitting the ball near the edges of the bar will get it to change directions!");
		gfx_text(wd/2 - 160, ht - 10, "PRESS 'n' TO ADVANCE NEXT SCREEN");
		c = gfx_wait();
		gfx_flush();
	}


	while(c!='q')
	{
		// gfx_color(255, 255, 255);

		// clear screens
		gfx_clear();

		// only continue if player has not won or loss
		if (gameLost == false && noWinYet == true)
		{

			// get bar location
			float barLeftEdge = barCenX-BARWIDTH/2;
			float barRightEdge = barCenX+BARWIDTH/2;
			float barTopEdge = barCenY-BARHEIGHT/2;
			float barBottomEdge = barCenY+BARHEIGHT/2;

			// clear window and draw ball bar
			gfx_clear();
			int brX = 100;
			int brY = 100;
			gfx_circle(xm, ym, r);

			// reset boolean variables
			noWinYet = false;
			gameLost = false;

			for(auto it = bricks.begin(); it < bricks.end(); it++)
			{
				// see if brick has made contact, otherwise draw it
				bool brickDead = (*it).touchingBall(xm, ym, r, vx, vy);
				(*it).drawBrick();

				// brick is alive, player did not win
				if (!brickDead)
				{
					noWinYet = true;
				}
			}

			// redraw bar at new location
			drawBar(barCenX,barCenY, BARHEIGHT, BARWIDTH);


			// determine if ball is on one of the walls besides bottom
			if (xm >= wd-r)
			{
				vx = -vx;
				xm = wd - r;
			}
			if(xm <= r)
			{
				vx = -vx;
				xm = r;
			}
			if(ym <= r)
			{
				vy = -vy;
				ym = r;
			}


			// hits the right part of the bar near the center
			if((ym >= (barTopEdge - r)) && (xm >= barCenX) && (xm <= (barCenX + BARWIDTH/4)))
			{
				if (vx > 0) // ball was going right
				{
					vy = -vy;
				}
				else // ball was going left
				{
					vx = -vx;
					vy = -vy;
				}
				ym = barTopEdge - r;
			}
			// hits the right part of the bar, but farther from the center
			else if((ym >= (barTopEdge - r)) && (xm >= barCenX + BARWIDTH/4) && (xm <= (barCenX + 3 * BARWIDTH/8)))
			{
				if (vx > 0) // ball was going right
				{
					vy = -vy;
				}
				// ball was going left; two options depending on ball contact angle
				else if (vx <= 0 && abs(atan(vy/vx)) < M_PI/6)
				{
					reCalculateVelocities(vx, vy, -M_PI/16);
				}
				else
				{
					reCalculateVelocities(vx, vy, M_PI/16);
				}
				ym = barTopEdge - r;
			}
			// hits the right part of the bar, but very far from the center
			else if((ym >= (barTopEdge - r)) && (xm >= barCenX + 3 * BARWIDTH/8) && (xm <= (barCenX + 4 * BARWIDTH/8)))
			{
				if (vx > 0) // ball was going right
				{
					vy = -vy;
				}
				// ball was going left; two options depending on ball contact angle
				else if (vx <= 0 && abs(atan(vy/vx)) < M_PI/6)
				{
					reCalculateVelocities(vx, vy, -M_PI/8);
				}
				else
				{
					reCalculateVelocities(vx, vy, M_PI/8);
				}
				ym =  barTopEdge - r;
			}
			// hits the left part of the bar near the center
			else if((ym >= (barTopEdge - r)) && (xm <= barCenX) && (xm >= (barCenX - BARWIDTH/4)))
			{
				if (vx < 0) // ball was going left
				{
					vy = -vy;
				}
				else // ball was going right
				{
					vx = -vx;
					vy = -vy;
				}
				ym = barTopEdge - r;
			}
			// hits the right part of the bar, but farther from the center
			else if((ym >= (barTopEdge - r)) && (xm <=(barCenX - BARWIDTH/4)) && (xm >= (barCenX - 3 * BARWIDTH/8)))
			{
				if (vx < 0) // ball was going left
				{
					vy = -vy;
				}
				// ball was going right; two options depending on ball contact angle
				else if (vx >= 0 && abs(atan(vy/vx)) < M_PI/6)
				{
					reCalculateVelocities(vx, vy, -M_PI/16);
					vx = -vx;
				}
				else
				{
					reCalculateVelocities(vx, vy, M_PI/16);
					vx = -vx;
				}
				ym = barTopEdge - r;
			}
			// hits the right part of the bar, but very far from the center
			else if((ym >= (barTopEdge - r)) && (xm <= (barCenX - 3 * BARWIDTH/8)) && (xm >= (barCenX - 4 * BARWIDTH/8)))
			{
				// ball was going left
				if (vx < 0)
				{
					vy = -vy;
				}
				// ball was going right; two options depending on ball contact angle
				else if (vx >= 0 && abs(atan(vy/vx)) < M_PI/6)
				{
					reCalculateVelocities(vx, vy, -M_PI/8);
					vx = -vx;
				}
				else
				{
					reCalculateVelocities(vx, vy, M_PI/8);
					vx = -vx;
				}
				ym = barTopEdge - r;
			}
			// missed bar
			else if (ym > (barTopEdge - r))
			{
				gameLost = true;
			}

			if (gfx_event_waiting())
			{
				c= gfx_wait();
				switch (c)
				{

					// user moved bar to the left
					case 'a':
						barCenX -= barVX;
						break;

					// user moved bar to right
					case 'd':
						barCenX+= barVX;
						break;

					// user quit
					case 'q':
						c = 'q';
						break;

					// star the game;
					case ' ':
						gameStart = true;
						break;
					default:
						cout << "Invalid input" << endl;
						break;
				};
			}


			// here we will move the ball, but only after game has started
			if (gameStart == true)
			{
				// move ball
				xm += vx;
				ym += vy;

				// accelerate ball
				if (vx > 0 ) // ball going right
				{
					vx += 0.0005;
				}
				else if (vx < 0) // ball going left
				{
					vx -= 0.0005;
				}
				if (vy > 0 ) // ball going down
				{
					vy += 0.0005;
				}
				else if (vx < 0) // ball going up
				{
					vy -= 0.0005;
				}
			}

			usleep(pausetime);
			gfx_flush();
		}

		// for exiting if player lost
		else
		{
			c = 'q';
			gfx_flush();
		}
	}

	// open window
	gfx_color(255,255,255);

	// display win message
	if (gameLost)
	{
		gfx_text(wd/2 - 100, ht/2, "Sorry! Better luck next time");
		gfx_flush();
		usleep(2000000);
	}
	// display lose messsage
	else if (!noWinYet)
	{
		gfx_text(wd/3 - 100, ht/2, "Congratulations! You've won!");
		gfx_flush();
		usleep(2000000);
	}



		return 0;
}


// for drawing bar
void drawBar(int barCenX, int barCenY, int BARHEIGHT, int BARWIDTH)
{
	gfx_line(barCenX+BARWIDTH/2, barCenY-BARHEIGHT/2, barCenX+BARWIDTH/2, barCenY+BARHEIGHT/2); // right border
	gfx_line(barCenX-BARWIDTH/2, barCenY-BARHEIGHT/2, barCenX-BARWIDTH/2, barCenY+BARHEIGHT/2); // left border
	gfx_line(barCenX-BARWIDTH/2, barCenY-BARHEIGHT/2, barCenX+BARWIDTH/2, barCenY-BARHEIGHT/2); // bottom border
	gfx_line(barCenX-BARWIDTH/2, barCenY+BARHEIGHT/2, barCenX+BARWIDTH/2, barCenY+BARHEIGHT/2); // top border
 }

// this function recalculates velocity vectors depending on angle of contact
 void reCalculateVelocities(float &vx, float &vy, float angleReduction)
 {
	 float hypotenuse = pow(pow(vx, 2) + pow(vy, 2), 0.5);
	 float angle = abs(atan(vy/vx));
	 float newAngle = angle - angleReduction; // reduce/add to angle

	 vx = hypotenuse * cos(abs(newAngle));
	 vy =  -1 *  hypotenuse * sin(abs(newAngle));
 }
