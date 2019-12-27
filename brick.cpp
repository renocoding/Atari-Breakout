// brick.cpp
// method definitions for brick class
// Author: Reno Domel

#include <iostream>
#include "brick.h"
#include "gfx.h"
#include <unistd.h>
using namespace std;


// default constructor
Brick::Brick()
{
    curStrength = 1;
    brLeftEdge = brX-BRICKWIDTH/2;
	brRightEdge = brX+BRICKWIDTH/2;
	brTopEdge = brY-BRICKHEIGHT/2;
	brBottomEdge = brY+BRICKHEIGHT/2;
}

// custom constructor
Brick::Brick(int c, int x, int y)
{
    curStrength = c; // curStrength is how many hits are currently needed to remove brick
    brX = x;
    brY = y;

    // x and y values of bar edges, very useful for analysis
	brLeftEdge = x-BRICKWIDTH/2;
	brRightEdge = x+BRICKWIDTH/2;
	brTopEdge = y-BRICKHEIGHT/2;
	brBottomEdge = y+BRICKHEIGHT/2;
}


Brick::~Brick(){};

// checks to see if a brick is in contact with a ball and if it is alive
bool Brick::touchingBall(float &xm, float &ym, int r, float &vx, float &vy)
{
    // make sure brick is even alive
    if (curStrength > 0)
    {

        // for touching left border of bricks
        if ((xm + r > brLeftEdge) && (xm + r < brRightEdge) && (ym < brBottomEdge) && (ym > brTopEdge) && (vx > 0))
        {
            curStrength -= 1;
            vx = -vx;
            xm = brLeftEdge - r; // reset x position to avoid glitch
        }

        // for touching right border of bricks
        else if ((xm - r < brRightEdge) && (xm - r > brLeftEdge) && (ym  < brBottomEdge) && (ym  > brTopEdge) && (vx < 0))
        {
            curStrength -= 1;
            vx = -vx;
            xm = brRightEdge + r;
        }

        // for touching top border of bricks
        else if ((ym + r >= brTopEdge) && (ym + r <= brBottomEdge) && (xm < brRightEdge) && (xm > brLeftEdge) && (vy > 0))
        {
            curStrength -= 1;
            vy = -vy;
            ym = brTopEdge - r; // reset y position to avoid glitch
        }


        // for touching bottom border of bricks
        else if ((ym - r <= brBottomEdge) && (ym - r >= brTopEdge) && (xm< brRightEdge) && (xm > brLeftEdge) && (vy < 0))
        {
            curStrength -= 1;
            vy = -vy;
            ym = brBottomEdge + r;
        }


        return false;
    }

    return true;

}


void Brick::drawBrick()
{

    // actaully draw brick (if it's alive)
    if (curStrength > 0)
    {

        if (curStrength == 1)
        {
			// gfx_color(0, 255, 0);
            gfx_text(brX, brY + 5,"1"); // draw 1 in center
        }
        else if (curStrength == 2)
        {
			// gfx_color(244, 255, 27);
            gfx_text(brX, brY + 5,"2"); // draw 2 in center
        }
        else if (curStrength == 3)
        {
			// gfx_color(0, 55, 255);
            gfx_text(brX, brY + 5,"3"); // draw 3 in center
        }

        gfx_line(brX+BRICKWIDTH/2, brY-BRICKHEIGHT/2, brX+BRICKWIDTH/2, brY+BRICKHEIGHT/2); // right border
    	gfx_line(brX-BRICKWIDTH/2, brY-BRICKHEIGHT/2, brX-BRICKWIDTH/2, brY+BRICKHEIGHT/2); // left border
    	gfx_line(brX-BRICKWIDTH/2, brY-BRICKHEIGHT/2, brX+BRICKWIDTH/2, brY-BRICKHEIGHT/2); // bottom border
    	gfx_line(brX-BRICKWIDTH/2, brY+BRICKHEIGHT/2, brX+BRICKWIDTH/2, brY+BRICKHEIGHT/2); // top border

    }
}
