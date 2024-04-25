#define _USE_MATH_DEFINES 
#include <GL/glut.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <string>
using namespace std;

struct Rectangle {
	float x; // x-coordinate of the rectangle
};

struct Clouds {
	float x;
	float y;
	float clrint;
};
struct Pothole {
	float x; // x-coordinate of the pothole
	float y; // y-coordinate of the pothole
	float horizontalAxis; // Horizontal axis length of the pothole
	float verticalAxis; // Vertical axis length of the pothole
};

struct orange {
	float x;
	float y;
};

struct Slab {
	float x;// x-coordinate of the slab
	float y;
	int width;
	int length;
};

Slab slab = { 800.0, 500.0, 10.0, 450.0 };

vector<Rectangle> rectangles;// Vector to store multiple rectangles
vector<Clouds> cloud;
Pothole pothole = { 1800.0, 340.0, 40.0, 30.0 };
vector<orange> org;

float cartY = 360.0; // Initial y-coordinate of the cart
bool gameOver = false;
bool isJumping = false; // Flag to indicate if the cart is jumping
bool isOnSlab = false;
float jumpHeight = 225.0;
float speed = 4;
int score = 0;


void myinit(void)
{
	glClearColor(0.529, 0.808, 0.922, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1900, 0.0, 1000);
}

void resetGame() {
	// Reset cart position
	cartY = 360.0;
	// Reset slab position
	slab.x = 500.0;
	// Reset pothole position
	pothole.x = 1500.0;
	// Reset jumping flags
	isJumping = false;
	isOnSlab = false;
	// Reset game over flag
	gameOver = false;
}

void setPixel(GLint x, GLint y)
{
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
}

void drawLine(float x1, float y1, float x2, float y2)
{
	glBegin(GL_LINES);
	glVertex2f(x1, y1);
	glVertex2f(x2, y2);
	glEnd();
}

void thickLine(int x1, int y1, int x2, int y2, int thickness) {
	// Calculate the angle of the line
	float angle = atan2((float)(y2 - y1), (float)(x2 - x1)) * 180.0 / M_PI;

	// Calculate the perpendicular angle
	float perpendicularAngle = angle + 90.0;

	// Convert angles to radians
	angle *= M_PI / 180.0;
	perpendicularAngle *= M_PI / 180.0;

	// Calculate half thickness
	float halfThickness = thickness / 2.0;

	// Calculate offsets
	float offsetX = halfThickness * cos(perpendicularAngle);
	float offsetY = halfThickness * sin(perpendicularAngle);

	// Draw rectangle as line
	glBegin(GL_QUADS);
	glVertex2f(x1 - offsetX, y1 - offsetY);
	glVertex2f(x1 + offsetX, y1 + offsetY);
	glVertex2f(x2 + offsetX, y2 + offsetY);
	glVertex2f(x2 - offsetX, y2 - offsetY);
	glEnd();
}
void mpCircle(int centerX, int centerY, int radius) {
	int x = radius;
	int y = 0;
	int radiusError = 1 - x;

	while (x >= y) {
		// Plot points in all octants
		glBegin(GL_POINTS);
		// Octant 1
		glVertex2i(centerX + x, centerY + y);
		// Octant 2
		glVertex2i(centerX + y, centerY + x);
		// Octant 3
		glVertex2i(centerX - y, centerY + x);
		// Octant 4
		glVertex2i(centerX - x, centerY + y);
		// Octant 5
		glVertex2i(centerX - x, centerY - y);
		// Octant 6
		glVertex2i(centerX - y, centerY - x);
		// Octant 7
		glVertex2i(centerX + y, centerY - x);
		// Octant 8
		glVertex2i(centerX + x, centerY - y);
		glEnd();

		// Update y based on the circle's midpoint
		y++;
		if (radiusError < 0) {
			radiusError += 2 * y + 1;
		}
		else {
			x--;
			radiusError += 2 * (y - x) + 1;
		}
	}
}

void drawGradientCircle(int centerX, int centerY, int radius) {
	// Iterate over each pixel in the circle
	for (int y = centerY - radius; y <= centerY + radius; ++y) {
		for (int x = centerX - radius; x <= centerX + radius; ++x) {
			// Calculate distance from the current pixel to the center of the circle
			float distance = sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
			if (distance <= radius) {
				// Calculate gradient factor based on distance from the center
				float gradientFactor = distance / radius;
				// Interpolate color from orange to yellow based on gradient factor
				float red = 1.0 - (0.471*gradientFactor);
				float green = 1.0 - (0.192*gradientFactor);
				float blue = 0.922*gradientFactor;
				// Set the color for the current pixel
				glColor3f(red, green, blue);
				// Draw the pixel
				glBegin(GL_POINTS);
				glVertex2i(x, y);
				glEnd();
			}
		}
	}
}
void drawCircle(float x, float y, float radius) {
	glBegin(GL_TRIANGLE_FAN);
	for (float angle = 0.0f; angle <= 360.0f; angle += 5.0f) {
		float radian = angle * (3.1415926f / 180.0f);
		glVertex2f(x + cos(radian) * radius, y + sin(radian) * radius);
	}
	glEnd();
}
void drawEllipse(float centerX, float centerY, float horizontalAxis, float verticalAxis) {
	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i++) {
		float angle = i * M_PI / 180.0;
		float x = centerX + horizontalAxis * cos(angle);
		float y = centerY + verticalAxis * sin(angle);
		glVertex2f(x, y);
	}
	glEnd();
}
void drawClouds(float x, float y, float clrint) {

	glColor3f(clrint, clrint, clrint);

	// Draw multiple circles to create the cloud shape
	// Adjust the positions and sizes to your liking
	drawCircle(x, y, 50);
	drawCircle(x+50,y+25, 60);
	drawCircle(x+70, y-20, 70);
	drawCircle(x+120, y+10, 60);
	drawCircle(x+160, y-10, 50);
}
void drawquad(float x, float y, int width, int length)
{
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x + length, y);
	glVertex2f(x + length, y - width);
	glVertex2f(x, y - width);
	glEnd();
	glColor3f(0, 0, 0);
	thickLine(x, y, x + length, y, 2);
	thickLine(x + length, y, x + length, y - width, 2);
	thickLine(x + length, y - width, x, y - width, 2);
	thickLine(x, y - width, x, y, 2);
}
void drawSlab(float x, float y, int width, int length)
{
	glColor3f(0.9, 0.3, 0.3);
	glBegin(GL_POLYGON);
	glVertex2f(x, y);
	glVertex2f(x+length, y);
	glVertex2f(x+length, y-width);
	glVertex2f(x, y-width);
	glEnd();
	glColor3f(0, 0, 0);
	thickLine(x, y, x + length, y, 2);
	thickLine(x + length, y, x + length, y - width, 2);
	thickLine(x + length, y - width, x, y - width, 2);
	thickLine(x, y - width, x, y, 2);


}
void drawCart(float x, float y)
{

	glColor3f(1, 0 ,0);
	glBegin(GL_QUADS);
	//cart
	glVertex2f(x - 75, y + 100);
	glVertex2f(x + 75, y + 100);
	glVertex2f(x + 50, y);
	glVertex2f(x - 50, y);
	glEnd();
	glColor3f(0, 0, 0); // Boundary color (black)
	// Draw top line
	thickLine(x - 75, y + 100, x + 75, y + 100, 2);
	// Draw bottom line
	thickLine(x - 50, y, x + 50, y, 2);
	// Draw left side line
	thickLine(x - 75, y + 100, x - 50, y, 2);
	// Draw right side line
	thickLine(x + 75, y + 100, x + 50, y, 2);
	glColor3f(0,0,0);
	//cart wheels
	drawCircle(x - 40, y-15, 15);
	drawCircle(x + 40, y-15, 15);
	glColor3f(0,0,0);
	//cart handle
	thickLine(x - 75, y + 100, x - 90, y + 100,2);
	thickLine(x - 85, y + 90, x - 95, y + 110,2);
	// lightning bolt
	glColor3f(1, 1, 0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(x + 10, y + 80);
	glVertex2f(x, y + 60);
	glVertex2f(x + 25, y + 60);
	glVertex2f(x -10, y + 20);
	glVertex2f(x, y + 45);
	glVertex2f(x - 25, y + 45);
	glVertex2f(x + 10, y + 80);

	glEnd();


	// Fill the lightning bolt shape
	glColor3f(1, 1, 0); // Set color to yellow
	glBegin(GL_TRIANGLES);
	glVertex2f(x + 10, y + 80);
	glVertex2f(x - 7.5, y + 45);
	glVertex2f(x -25 , y + 45);

	glBegin(GL_TRIANGLES);
	glVertex2f(x - 10 , y + 20);
	glVertex2f(x + 25 , y + 60);
	glVertex2f(x + 6, y + 60);

	glBegin(GL_TRIANGLES);
	glVertex2f(x, y + 60);
	glVertex2f(x + 6, y + 60);
	glVertex2f(x, y + 45);


	glBegin(GL_TRIANGLES);
	glVertex2f(x, y + 45);
	glVertex2f(x - 7.5, y + 45);
	glVertex2f(x, y + 60);
	glEnd();

}
void draworange(float x, float y)
{
	glColor3f(1, 0.5, 0);
	drawCircle(x, y, 10);
	glColor3f(0, 0.6, 0);
	thickLine(x, y + 10, x + 10, y + 10, 2);
}

void keyboard(unsigned char key, int x, int y) {
	if (key == ' ' && !isJumping && !gameOver && cartY == 360) { // Check if spacebar is pressed and cart is not already jumping
		isJumping = true; // Set jumping flag to true
	}
	else if (key == 13 && gameOver) {
		// Check if Enter key is pressed and the game is over
		resetGame(); // Reset the game state
		glutPostRedisplay(); // Redraw the display
	}
}

void performJumpAnimation() {
	if (!isOnSlab) 
	{
		if (isJumping) {
			cartY += 6.0; // Increment cart's y-coordinate to move it upward
			if (cartY >= 360.0 + jumpHeight) { // Check if cart has reached the maximum jump height
				isJumping = false; // Set jumping flag to false
			}
		}
		else {
			if (cartY > 360.0) {
				cartY -= 6.0; // Decrement cart's y-coordinate to move it downward
			}
		}
	}
}
void updateroadstripPosition()
{
	for (auto& rectangle : rectangles) {
		rectangle.x -= speed; // Move the rectangle to the left
		if (rectangle.x < -50.0) // Check if the rectangle has crossed the left edge
			rectangle.x = 1900.0; // Move the rectangle to the right edge
	}
}
void updatecloudsPosition()
{
	for (auto& clouds : cloud) {
		clouds.x -= 1; // Move the rectangle to the left
		if (clouds.x < -500.0) // Check if the rectangle has crossed the left edge
			clouds.x = 2000.0; // Move the rectangle to the right edge
	}
}
void updatePotholePosition() {
	pothole.x -= speed; // Move the pothole to the left
	if (pothole.x < -50.0) // Check if the pothole has crossed the left edge
		pothole.x = 2000.0; // Move the pothole to the right edge
}

void updateSlabPosition() {
	slab.x -= speed; // Move the slab to the left
	if (slab.x + slab.length <= 80)
	{
		isOnSlab = false;
	}
	if (slab.x < -slab.length) // Check if the slab has crossed the left edge
		slab.x = 2000.0; // Move the slab to the right edge
}
void updateorgposition() {
	for (auto& orng : org) {
		orng.x -= speed; // Move the rectangle to the left
		if (orng.x < -100.0) // Check if the rectangle has crossed the left edge
			orng.x = 2000.0; // Move the rectangle to the right edge
	}
}

bool checkCollision(float cartX) {
	float ellipseX = pothole.horizontalAxis;
	float ellipseY = pothole.verticalAxis;
	float ellipseXSquare = ellipseX * ellipseX;
	float ellipseYSquare = ellipseY * ellipseY;
	float xDiff1 = cartX - 50;
	float xDiff2 = cartX + 50;
	float yDiff = cartY - 15;
	float leftdiff = ((xDiff1 - pothole.x) * (xDiff1 - pothole.x)) / ellipseXSquare + ((yDiff - pothole.y) * (yDiff - pothole.y)) / ellipseYSquare;
	float rightdiff = ((xDiff2 - pothole.x) * (xDiff2 - pothole.x)) / ellipseXSquare + ((yDiff - pothole.y) * (yDiff - pothole.y)) / ellipseYSquare;
	float xtopdiff1 = cartX - 75;
	float xtopdiff2 = cartX + 75;
	float ytopdiff = cartY + 50;
	if (cartX + 50 >= slab.x && cartX + 50 <= slab.x + slab.length && cartY - 30 >= 500) {
		isOnSlab = true;
	}
	return leftdiff <= 1 || rightdiff <= 1 || (xDiff2 >= slab.x && xDiff2 <= slab.x + slab.length && yDiff - 15 < slab.y && yDiff + 115 > slab.y);
}

bool checkfruitcoll(float cartX)
{
	for (auto& orng : org)
	{
		float orgrsq = 100;
		float xDiff1 = cartX - 50;
		float xDiff2 = cartX + 50;
		float yDiff = cartY - 15;
		float leftdiff = ((xDiff1 - orng.x) * (xDiff1 - orng.x)) / orgrsq + ((yDiff - orng.y) * (yDiff - orng.y)) / orgrsq;
		float rightdiff = ((xDiff2 - orng.x) * (xDiff2 - orng.x)) / orgrsq + ((yDiff - orng.y) * (yDiff - orng.y)) / orgrsq;
		return leftdiff <= 1 || rightdiff <= 1;
	}
	
}

void drawStrokeText(float x, float y, float scale, const char* text) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(scale, scale,1);
	
	// Set the desired font (available fonts are GLUT_STROKE_ROMAN, GLUT_STROKE_MONO_ROMAN)
	void* font = GLUT_STROKE_ROMAN;
	// Loop through each character in the text
	for (const char* c = text; *c != '\0'; ++c) {
		glutStrokeCharacter(font, *c); // Render the character
	}
	glPopMatrix();
}
void drawStrokeTextandscore(float x, float y, float scale, const char* text, int value) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glScalef(scale, scale, 1);
	string sc = to_string(value);
	string res = text + sc;
	// Set the desired font (available fonts are GLUT_STROKE_ROMAN, GLUT_STROKE_MONO_ROMAN)
	void* font = GLUT_STROKE_ROMAN;
	// Loop through each character in the text
	for (const char& c : res) {
		glutStrokeCharacter(font, c); // Render the character
	}
	glPopMatrix();
}

void drawBitmapText(const char* string, float x, float y) {
	glRasterPos2f(x, y); // Set the position for the text
	for (const char* c = string; *c != '\0'; ++c) {
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c); // Use the default 24-pixel font
	}
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 0.0);
	glPointSize(2.0);
		glColor3f(0.588, 0.294, 0); // Set color to brown
		//ground
		glBegin(GL_QUADS);
		glVertex2f(0, 240); // Top-left corner
		glVertex2f(1900, 240); // Top-right corner
		glVertex2f(1900, 0); // Bottom-right corner
		glVertex2f(0, 0); // Bottom-left corner
		glEnd();

		//ground to grass gradient
		glBegin(GL_QUADS);
		glColor3f(0.0, 1.0, 0.0); // Green color (top)
		glVertex2f(0, 290);
		glVertex2f(1900, 290);
		glColor3f(0.588, 0.294, 0); // Brown color (bottom)
		glVertex2f(1900, 240.0);
		glVertex2f(0, 240.0);
		glEnd();

		//road
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_QUADS);
		glVertex2f(0, 380); // Top-left corner
		glVertex2f(1900, 380); // Top-right corner
		glVertex2f(1900, 290); // Bottom-right corner
		glVertex2f(0, 290); // Bottom-left corner
		glEnd();

		//sun
		drawGradientCircle(1500, 800, 80);
		glColor3f(1, 1, 0);
		drawCircle(1500, 800, 50);

		//clouds
		for (const auto& clouds : cloud) {
			drawClouds(clouds.x, clouds.y, clouds.clrint);
			glEnd();
		}
		//road strips
		glColor3f(1.0, 1.0, 1.0); // White color
		for (const auto& rectangle : rectangles) {
			glBegin(GL_QUADS);
			glVertex2f(rectangle.x, 330.0);
			glVertex2f(rectangle.x + 50.0, 330.0);
			glVertex2f(rectangle.x + 50.0, 340.0);
			glVertex2f(rectangle.x, 340.0);
			glEnd();
		}
		glColor3f(0.9, 0.9, 0);

		for (const auto& orng : org)
		{
			draworange(orng.x, orng.y);
		}
		

		if (isOnSlab) {
			drawCart(150, 530);
		}
		else {
			drawCart(150, cartY);
		}

		glColor3f(0, 0, 0); // Black color for the pothole
		drawEllipse(pothole.x, pothole.y, pothole.horizontalAxis, pothole.verticalAxis);
		glColor3f(0.6, 0.6, 0.6);
		drawEllipse(pothole.x+35, pothole.y-5, pothole.horizontalAxis, pothole.verticalAxis);

		//slab
		drawSlab(slab.x, slab.y, slab.width, slab.length);

		if (checkfruitcoll)
		{
			score += 100;
		}

		if (checkCollision(150.0)) { // Assuming cart's x-coordinate is 150.0
			// Stop the game if collision occurs
			cout << "Game Over - You hit an obstacle!" << endl;
			gameOver = true;
		}

		glColor3f(1, 1, 0);
		drawquad(80, 970, 80, 130);
		string sc = to_string(score);
		string hsc;
		if (score < 1986)
		{
			hsc = "HS: 1986";
		}
		else
			hsc = sc;
		glColor3f(0, 0, 0);
		drawBitmapText(hsc.c_str(), 100, 940);
		drawBitmapText(sc.c_str(), 100, 900);

		if (gameOver) {
			glColor3f(1, 1, 1);
			drawquad(450, 620, 200, 1050);
			glColor3f(0, 0, 0); // black color for the "Game Over" message
			drawStrokeTextandscore(550, 550, 0.5, "Game Over - Score:", score);
			drawStrokeText(650, 450, 0.4, "Press Enter to Retry");
			speed = 4.0;
			score = 0;
		}

		glFlush();
	
}

void update(int value)
{
	if (!gameOver) {
		performJumpAnimation(); // Perform jump animation

		// Update the position of each rectangle (road strips)
		updateroadstripPosition();
		updatecloudsPosition();

		updatePotholePosition();
		updateSlabPosition();
		updateorgposition();
		speed = speed + 0.005;
		score += 1;

		glutPostRedisplay(); // Update the display
	}

	glutTimerFunc(7, update, 0); // Call update function after 7 milliseconds (143 FPS)
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitWindowSize(1900,1000);
	glutInitWindowPosition(10, 10);
	glutCreateWindow("Flash Cart");
	for (int i = 0; i < 13; ++i) {
		Rectangle rect;
		rect.x = 100.0 + i * 150.0; // Initial x-coordinate
		rectangles.push_back(rect);
	}
	for (int i = 0; i < 3; ++i) {
		orange orng;
		orng.x = slab.x + 200 + i * 150.0;// Initial x-coordinate
		orng.y = 515;
		org.push_back(orng);
	}
	float arr[5][3] = { {800,800,0.8}, {200, 750, 0.9}, {1100, 750, 0.75}, {1450, 900, 0.9}, {1700, 850, 0.85} };
	for (int i = 0; i < 5; ++i) {
		Clouds cld;
		cld.x = arr[i][0]; // Initial x-coordinate
		cld.y = arr[i][1];
		cld.clrint = arr[i][2];
		cloud.push_back(cld);
	}
	myinit();
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(0, update, 0);
	glutMainLoop();
	return 0;
}
