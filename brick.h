// brick.h
// Author: Reno Domel
// method/class declarations for Brick



// global variables
const int BRICKWIDTH = 40;
const int BRICKHEIGHT = 20;


class Brick
{
    public:
        // constructors and destructor
        Brick();
        Brick(int, int, int);
        ~Brick();

        // draw brick on board
        void drawBrick();

        // analyzing if brick was hit
        bool touchingBall(float &, float &, int, float &, float &);
    private:
        int curStrength;
        int brX;
        int brY;
        int brLeftEdge;
    	int brRightEdge;
    	int brTopEdge;
    	int brBottomEdge;
};
