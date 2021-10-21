#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<stack>
#include<math.h>
#include<GL/glut.h>
using namespace std;

int game_state[3][3]; // stores configuration of board
int playerturn;      // playerturn is decided
int final_result;   // who wins the game in end
bool gameover;     // if the game is over or not
stack<int> x_points;  // stores the xpoints marked 
stack<int> y_points;  // stores ypoints marked
float i = 0;		// for color variation
int flag = 0;       // for color variation

void initGL()
{
	//keeps the constant black colored background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void initialize_game()
{
	//starts the game
	playerturn = 1; // playerturn 1 when game starts
	for (int i = 0; i <= 2; i++)
	{
		for (int j = 0; j <= 2; j++)
		{
			game_state[i][j] = 0; // board configuration is made empty 
		}
	}
}

void play_again(unsigned char key, int x, int y) {
	switch (key) {
	case 'y':
		if (gameover == true)
		{
			// if game is over and user pressed y restart the game
			gameover = false;
			initialize_game();
		}
		break;
	case 'n':
		// if game is over and user pressed n , end the game
		if (gameover == true)
			exit(0);
		break;
	default:
		//any other key is pressed end it
		exit(0);

	}
}
void click(int button, int state, int x, int y)
{
	if (gameover == false && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (playerturn == 1) // if playerturn 1 make it a cross
		{
			if (game_state[(y - 50) / 100][x / 100] == 0)
			{
				game_state[(y - 50) / 100][x / 100] = 1; // 1 is for cross
				playerturn = 2;  // switch playerturn
				x_points.push((y - 50) / 100); // push the xposition in stack
				y_points.push(x / 100); //push the yposition in stack
			}
		}
		else
		{
			//playerturn 2 turn
			if (game_state[(y - 50) / 100][x / 100] == 0)
			{
				game_state[(y - 50) / 100][x / 100] = 2; // 2 is for circle
				playerturn = 1;
				x_points.push((y - 50) / 100); // push the xposition in stack
				y_points.push(x / 100); // push the yposition in stack
			}
		}
	}
	else if (gameover == false && button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		if (x_points.size() != 0 && y_points.size() != 0)
		{
			game_state[x_points.top()][y_points.top()] = 0;
			x_points.pop();
			y_points.pop();
			if (playerturn == 1)
				playerturn = 2;
			else
				playerturn = 1;
		}
	}
}

void drawString(void* font, const char* s, float x, float y) {
	unsigned int i;
	glRasterPos2f(x, y); // defines start position to write string
	for (i = 0; i < strlen(s); i++)
		glutBitmapCharacter(font, s[i]); // prints char
}

void drawlines()
{
	if (flag == 1)
	{
		i = i - 0.001; //color variation
		if (i <= 0)
			flag = 0;
	}
	else if(flag == 0 )
	{
		i = i + 0.001;
		if (i >= 1)
			flag = 1;
	}
	glLineWidth(10); // line width
	glBegin(GL_LINES);
	glColor3f(0, 0, 1);
	//2 vertical lines
	glVertex2f(100, 50);
	glVertex2f(100, 340);
	glVertex2f(200, 340);
	glVertex2f(200, 50);
	//2 horizontal lines
	glVertex2f(0, 150);
	glVertex2f(300, 150);
	glVertex2f(0, 250);
	glVertex2f(300, 250);
	glEnd();
	glLineWidth(3); // inside line
	glBegin(GL_LINES);
	glColor3f(i, i, 1.0);
	//2 vertical lines
	glVertex2f(100, 50);
	glVertex2f(100, 340);
	glVertex2f(200, 340);
	glVertex2f(200, 50);
	//2 horizontal lines
	glVertex2f(0, 150);
	glVertex2f(300, 150);
	glVertex2f(0, 250);
	glVertex2f(300, 250);
	glEnd();
}

void drawxo()
{
	if (gameover == true) {
		// if gameisover turn everything to green
		glColor3f(0, 1, 0);
	}
	else {
		glColor3f(0.2, i, 0.6);
	}
	for (int i = 0; i <= 2; i++)
	{
		for (int j = 0; j <= 2; j++)
		{
			if (game_state[i][j] == 1) //if it is 1 then draw x 
			{
				glLineWidth(8);
				glBegin(GL_LINES);
				glVertex2f(50 + j * 100 - 25, 100 + i * 100 - 25); // drawing lines
				glVertex2f(50 + j * 100 + 25, 100 + i * 100 + 25);
				glVertex2f(50 + j * 100 - 25, 100 + i * 100 + 25);
				glVertex2f(50 + j * 100 + 25, 100 + i * 100 - 25);
				glEnd();
			}
			else if (game_state[i][j] == 2) //if it is 2 then draw o
			{
				glBegin(GL_POLYGON);
				float rad = 25; // radius
				for (float ang = 0; ang <= 2 * 3.141; ang += 0.1)
				{
					// draw the vertices required.
					glVertex3f((50 + j * 100) + cos(ang) * rad, (100 + i * 100) + sin(ang) * rad, 0);
				}
				glEnd();
			}
		}
	}
}

bool checkifwin()
{
	int i, j;
	for (i = 0; i <= 2; i++)
	{
		for (j = 1; j <= 2; j++)
		{
			if (game_state[i][0] != 0 && game_state[i][0] == game_state[i][j]) // match the rows
			{
				if (j == 2)
					return true;
			}
			else
				break;
		}
	}
	for (i = 0; i <= 2; i++)
	{
		for (j = 1; j <= 2; j++)
		{
			if (game_state[0][i] != 0 && game_state[0][i] == game_state[j][i]) // match the colums
			{
				if (j == 2)
					return true;
			}
			else
				break;
		}
	}
	for (i = 1; i <= 2; i++)
	{
		if (game_state[0][0] != 0 && game_state[0][0] == game_state[i][i]) // diagonal match
		{
			if (i == 2)
				return true;
		}
		else
			break;
	}
	for (i = 1; i <= 2; i++)
	{
		if (game_state[2][0] != 0 && game_state[2][0] == game_state[2 - i][i]) //other diagonal
		{
			if (i == 2)
				return true;
		}
		else
			break;
	}
}
bool checkifdraw()
{
	int i, j;
	bool draw;

	for (i = 0; i <= 2; i++)
	{
		for (j = 0; j <= 2; j++)
		{
			if (game_state[i][j] == 0) // if box empty then no draw
				return false;
		}
	}
	return true;
}
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0, 0);
	if (playerturn == 1)
		drawString(GLUT_BITMAP_HELVETICA_18, "Player1's turn", 100, 30);
	else
		drawString(GLUT_BITMAP_HELVETICA_18, "Player2's turn", 100, 30);

	drawlines();
	drawxo();

	if (checkifwin() == true)
	{
		if (playerturn == 1)
		{
			gameover = true; // game is won game is over
			final_result = 2; // winner
		}
		else
		{
			gameover = true;
			final_result = 1;
		}
	}
	else if (checkifdraw() == true)
	{
		gameover = true; // if draw happens
		final_result = 0; // no one wins
	}

	if (gameover == true)
	{
		glColor3f(1, 0, 0);
		drawString(GLUT_BITMAP_HELVETICA_18, "Game Over", 100, 160);

		if (final_result == 0)
			drawString(GLUT_BITMAP_HELVETICA_18, "Its a draw", 110, 185);

		if (final_result == 1)
			drawString(GLUT_BITMAP_HELVETICA_18, "Player1 wins", 95, 185);

		if (final_result == 2)
			drawString(GLUT_BITMAP_HELVETICA_18, "Player2 wins", 95, 185);

		drawString(GLUT_BITMAP_HELVETICA_18, "Do you want to continue (y/n)", 40, 210);
	}

	glutSwapBuffers();
}

void reshape(int x, int y)
{
	glViewport(0, 0, x, y); // clipping window set
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	glOrtho(0, x, y, 0, 0, 1); 
}
int main(int argc, char** argv)
{
	initialize_game(); // starts the game
	glutInit(&argc, argv); // takes arguments from command line
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // sets display mode to colored
	glutInitWindowPosition(100, 100); // initial window size
	glutInitWindowSize(300, 350); // size of window
	glutCreateWindow("Tic Tac Toe"); // window heading
	glutReshapeFunc(reshape); // reshape func
	glutDisplayFunc(display); // calls display function
	glutKeyboardFunc(play_again); // keyboard function to restart and end game
	glutMouseFunc(click); // mouse clicking function
	glutIdleFunc(display); // calls display function when no event is there.
	initGL(); // initialize stactic parameter
	glutMainLoop(); // loops the main
}