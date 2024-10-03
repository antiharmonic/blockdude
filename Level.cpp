// Function definitions for the class Level
// I know a lot of it is messy... but i was too lazy to clean much of it up
// it needs to be made so that a person can type in a password and continue their game...
// but that should be easy enough for any of you reading this to implement.
// if you find any major bugs, email me: [redacted]



#include "Level.h"
#include <cstdio>
#include <cassert>
//mac files... if your on windows comment these out and uncomment the windows files :p
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
//windows files
//#include "glut.h"
//#include "GL/glu.h"
//#include "GL/gl.h"
//#include "windows.h"
#include <cstring>
#include "modes.h"

#define LEVEL_FILE		"./original_levels.txt"
#define MAX_WIDTH		50
#define BUFFER_LENGTH	1024
#define MODE_NUM		1
#define MODE_PASS		2
#define MODE_FILE		3
#define MODE_HEIGHT		4
#define MODE_WIDTH		5
#define MODE_X			6
#define MODE_Y			7
#define EMPTY			0
#define BLOCK			1
#define BRICK			2
#define DOOR			3
#define BLOCKDUDE		4

void myTimer(int);

Level::Level() {
	Level(1);
}

void Level::printLevel() {
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			printf("%c", pieces[layout[i][j]]);
		}
		printf("\n");
	}
}

void Level::zero(char arr[], int len) {
	for (len -= 1; len >= 0; len--) {
		arr[len] = 0;
	}
}

bool Level::is_a_comment(char first) {
	
	bool result = false;
	if (first == '#')
		result = true;
	//printf("is_a_comment was called. Received %c. Returning %i\n", first, (int)result);
	return result;
}

Level::Level(int n) {
	//open LEVEL_FILE
	char line[BUFFER_LENGTH];
	bool found = false;
	FILE *fp;
	//parse it to find level n
	printf("Trying to open %s.\n", LEVEL_FILE);
	assert(fp = fopen(LEVEL_FILE, "r"));
	//printf("Entered Constructor\n");
	holding = false;
	pieces[EMPTY] = ' ';
	pieces[BLOCK] = 'o';
	pieces[BRICK] = '#';
	pieces[DOOR]  = '|';
	block_color[0] = 0.0;
	block_color[1] = 0.0;
	block_color[2] = 1.0;
	moving = false;
	pieces[BLOCKDUDE] = BLOCKDUDE;
	while (fgets(line, BUFFER_LENGTH, fp)) {
		//make sure it got an entire line.. if not print error and die
		//where_newline();
		if (!is_a_comment(line[0])) {
			// if it doesnt start with # parse it
			if (cmpStringToNumber(line, n)) {
				//if we found the line containing information about level n
				found = true;
				break;
			}
		}
		zero(line, BUFFER_LENGTH);
	}
	fclose(fp);
	if (found == true) {
		//parse level n line for information and set it :)
		int i = 0;
		int mode = MODE_NUM;
		int subbegin;
		int subend;
		char substr[BUFFER_LENGTH / 2]; // why 2? why not?
		
		facing = FACING_LEFT;
		//printf("Level::Level(int) - Beginning to process string.\n");
		while (line[i] != '\0') {
			//printf("Level::Level(int) - While loop. mode = %i\n", mode);
			subbegin = subend = i;
			while (line[i] != ':' && line[i] != '\n') {
				i++;
				subend++;
			}
			if (mode == MODE_NUM) {
				mylevel = numberFromSubstring(line, subbegin, subend);
			} else if (mode == MODE_PASS) {
				password = new char [(subend - subbegin + 2)]; // (one for newline, one for begin-end equaling 0)
				makeSubstring(line, subbegin, subend, password); //alters password
			} else if (mode == MODE_FILE) {
				levelfile = new char [(subend - subbegin + 2)]; // (one for newline, one for begin-end equaling 0)
				makeSubstring(line, subbegin, subend, levelfile); //alters password
			} else if (mode == MODE_HEIGHT) {
				h = numberFromSubstring(line, subbegin, subend);
			} else if (mode == MODE_WIDTH) {
				w = numberFromSubstring(line, subbegin, subend);
			} else if (mode == MODE_X) {
				x = numberFromSubstring(line, subbegin, subend);
			} else if (mode == MODE_Y) {
				y = numberFromSubstring(line, subbegin, subend);
			} else {
				//die
				break;
			}
			i++; // get away from the semicolon
			mode++; // get to new mode
		}
		//printf("Level %i... %i x %i, located in %s\n", mylevel, h, w, levelfile);
		//open level n file
		fp = fopen(levelfile, "rt");
		assert(fp);
		//set layout to that file
		layout = new int* [h];
		for (int i = 0; i < h; i++) {
			layout[i] = new int [w];
		}
		{
		int c;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				c = getc(fp);
				assert(c != EOF);
				if (c == '\n')
					c = getc(fp);
				layout[i][j] = c - '0';
			}
		}
		}
		//close file
		fclose(fp);
	} else {
		//won game
		newLevel(1);
	}
}

void Level::newLevel(int n) {
	delete [] password;
	delete [] levelfile;
	for (int i = 0; i < h; i++) {
		delete [] layout[i];
	}
	delete [] layout;
	char line[BUFFER_LENGTH];
	bool found = false;
	FILE *fp; fp = fopen(LEVEL_FILE, "r");
	//parse it to find level n
	assert(fp);
	//printf("Entered Constructor\n");
	holding = false;
	pieces[EMPTY] = ' ';
	pieces[BLOCK] = 'o';
	pieces[BRICK] = '#';
	pieces[DOOR]  = '|';
	pieces[BLOCKDUDE] = BLOCKDUDE;
	moving = false;
	while (fgets(line, BUFFER_LENGTH, fp)) {
		//make sure it got an entire line.. if not print error and die
		//where_newline();
		if (!is_a_comment(line[0])) {
			// if it doesnt start with # parse it
			if (cmpStringToNumber(line, n)) {
				//if we found the line containing information about level n
				found = true;
				break;
			}
		}
		zero(line, BUFFER_LENGTH);
	}
	fclose(fp);
	if (found == true) {
		//parse level n line for information and set it :)
		int i = 0;
		int mode = MODE_NUM;
		int subbegin;
		int subend;
		char substr[BUFFER_LENGTH / 2]; // why 2? why not?
		
		facing = FACING_LEFT;
		//printf("Level::Level(int) - Beginning to process string.\n");
		while (line[i] != '\0') {
		//	printf("Level::Level(int) - While loop. mode = %i\n", mode);
			subbegin = subend = i;
			while (line[i] != ':' && line[i] != '\n') {
				i++;
				subend++;
			}
			if (mode == MODE_NUM) {
				//mylevel = numberFromSubstring(line, subbegin, subend);
				mylevel = n;
			} else if (mode == MODE_PASS) {
				password = new char [(subend - subbegin + 2)]; // (one for newline, one for begin-end equaling 0)
				makeSubstring(line, subbegin, subend, password); //alters password
			} else if (mode == MODE_FILE) {
				levelfile = new char [(subend - subbegin + 2)]; // (one for newline, one for begin-end equaling 0)
				makeSubstring(line, subbegin, subend, levelfile); //alters password
			} else if (mode == MODE_HEIGHT) {
				h = numberFromSubstring(line, subbegin, subend);
			} else if (mode == MODE_WIDTH) {
				w = numberFromSubstring(line, subbegin, subend);
			} else if (mode == MODE_X) {
				x = numberFromSubstring(line, subbegin, subend);
			} else if (mode == MODE_Y) {
				y = numberFromSubstring(line, subbegin, subend);
			} else {
				//die
				break;
			}
			i++; // get away from the semicolon
			mode++; // get to new mode
		}
		//printf("Level %i... %i x %i, located in %s\n", mylevel, h, w, levelfile);
		//open level n file
		fp = fopen(levelfile, "rt");
		//set layout to that file
		layout = new int* [h];
		for (int i = 0; i < h; i++) {
			layout[i] = new int [w];
		}
		{
		int c;
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				c = getc(fp);
				assert(c != EOF);
				if (c == '\n')
					c = getc(fp);
				layout[i][j] = c - '0';
			}
		}
		}
		//close file
		fclose(fp);
	} else {
		// level not found... terminate
		//printf("Did not load correctly. Exiting...\n");
		exit(EXIT_FAILURE);
	}
}

void Level::pickup() {
	int xway;
	if (facing == FACING_LEFT)
		xway = -1;
	else
		xway = 1;
	if (holding == false) {
		if (layout[h-y-1][x+xway] == BLOCK) {
			layout[h-y-1][x+xway] = EMPTY;
			holding = true;
		}
	} else {
		if (layout[h-y-2][x+xway] == EMPTY) {
			int i;
			layout[h-y-2][x+xway] = BLOCK;
			i = 1;
			while (layout[h-y-i][x+xway] == EMPTY) {
				layout[h-y-i][x+xway] = BLOCK;
				layout[h-y-i-1][x+xway] = EMPTY;
				i--;
			}
			holding = false;
		}
	}
}

void Level::move(int dir) {
	//printf("move being called: %i\n", x);
	if (dir == MOVE_RIGHT || dir == MOVE_LEFT) {
		int way;
		if (dir == MOVE_LEFT)
			way = -1;
		else
			way = 1;
		
		if (layout[h - y - 1][x + way] == EMPTY || layout[h - y -1][x + way] == DOOR) {
			int i = 2;
			
				x += way;
			moving = true;
	
			while (layout[h-y][x] == EMPTY || layout[h-y][x] == DOOR) {
				y--;
			}
					
		}
		if (dir == MOVE_LEFT) {
			facing = FACING_LEFT;
		} else {
			facing = FACING_RIGHT;
		}
	} else if (dir == MOVE_UP) {
		int xway;
		if (facing == FACING_LEFT) {
			xway = -1; 	
		} else if (facing == FACING_RIGHT) {
			xway = 1;
		}
		//printf("y: %i\t h-y: %i\t h-y-2: %i\n", y, h-y, h-y+2);
		if (layout[h - y - 1][x + xway] == BRICK || layout[h - y - 1][x + xway] == BLOCK) {
			if (layout[h - y - 2][x + xway] == EMPTY || layout[h - y - 2][x + xway] == DOOR) {
			x += xway;
			y += 1;
		}}
	} else {
		//kill program
	}
	if (layout[h-y-1][x] == DOOR) {
		//go to next level
		newLevel(++mylevel);	
	}
	//printf("move exiting: %i\n", x);
	//redisplay... animate walking
}

void Level::resetLevel() {
	newLevel(mylevel);
}

void Level::draw() {
	int level_top, level_bottom, level_right, level_left;
	// draw viewable part of screen
	// figure out what part of the screen needs to be drawn
	level_top = y + 3;
	level_bottom = y - 3;
	level_left = x - 6;
	level_right = x + 5;
	if (level_left < 0) {
		int offset = -level_left;
		level_left = 0;
		level_right += offset;
	}
	
	if (level_right >= w) {
		int offset = (level_right - w) + 1;
		level_right = w - 1;
		level_left -= offset;
	}
	
	if (level_bottom < 0) {
		int offset = -level_bottom;
		level_bottom = 0;
		level_top += offset;
	}
	
	if (level_top >= h) {
		int offset = (level_top - h) + 1;
		level_top = h - 1;
		level_bottom -= offset;
	}
		
	assert((level_top - level_bottom) == 6);
	assert((level_right - level_left) == 11);
	
	int screen_x = 0, screen_y = 0;

	for (int i = level_left; i <= level_right; i++) {
		for (int j = level_bottom; j <= level_top; j++) {
			int array_bottom = /*level_top*/ h - j - 1;
			if (layout[array_bottom][i] == BRICK) {
				glColor3f(0.25, 0.0, 0.0);
				glBegin(GL_QUADS);
					glVertex2f((float)screen_x, (float)screen_y);
					glVertex2f((float)screen_x + 1, (float)screen_y);
					glVertex2f((float)screen_x + 1, (float)screen_y + 1);
					glVertex2f((float)screen_x, (float)screen_y + 1);
				glEnd();
				
			} else if (layout[array_bottom][i] == BLOCK) {
				glColor3f(block_color[0], block_color[1], block_color[2]);
				glBegin(GL_QUADS);
					glVertex2f((float)screen_x, (float)screen_y);
					glVertex2f((float)screen_x + 1, (float)screen_y);
					glVertex2f((float)screen_x + 1, (float)screen_y + 1);
					glVertex2f((float)screen_x, (float)screen_y + 1);
				glEnd();
			} else if (layout[array_bottom][i] == DOOR) {
				glColor3f(1.0, 1.0, 0.0);
				glBegin(GL_QUADS);
					glVertex2f((float)screen_x, (float)screen_y);
					glVertex2f((float)screen_x + 1, (float)screen_y);
					glVertex2f((float)screen_x + 1, (float)screen_y + 1);
					glVertex2f((float)screen_x, (float)screen_y + 1);
				glEnd();
			}
			screen_y++;
		}
		screen_y = 0;
		screen_x++;
	}
		glColor3f(0.0f, 0.25f, 0.0f);
		
		{
		float offset = 0;
		/*if (moving == true)
			offset = .5;
		*/
		if (facing == FACING_LEFT) {
		offset = -offset;
		glBegin(GL_TRIANGLES);
			glVertex2f((float)(x - level_left) + offset, (float)((y - level_bottom) + 0.5));
			glVertex2f((float)((x - level_left) + 1.0) + offset, (float)((y - level_bottom) + 1));
			glVertex2f((float)((x - level_left) + 1.0) + offset, (float)(y - level_bottom));
		glEnd();
		} else {
		
		glBegin(GL_TRIANGLES);
			glVertex2f((float)((x - level_left) + 1.0) + offset, (float)((y - level_bottom) + 0.5));
			glVertex2f((float)(x - level_left) + offset, (float)((y - level_bottom) + 1));
			glVertex2f((float)(x - level_left) + offset, (float)(y - level_bottom));
		glEnd();
		}
		
		
		if (holding == true) {
			glColor3f(block_color[0], block_color[1], block_color[2]);
			glBegin(GL_QUADS);
				glVertex2f((float)(x - level_left)+offset, (float)(y - level_bottom + 1));
				glVertex2f((float)(x - level_left)+offset, (float)(y - level_bottom + 2));
				glVertex2f((float)(x - level_left + 1)+offset, (float)(y - level_bottom + 2));
				glVertex2f((float)(x - level_left + 1)+offset, (float)(y - level_bottom + 1));
			glEnd();
		}
		
		/*if (moving == true) {
			if (total == 1) {
				total = 0;
				moving = false;
			} else {
				total++;
			}
			glutPostRedisplay();
		}*/
		}
	//printf("level_ top: %i\tbottom: %i\tleft: %i\tright: %i\n", level_top, level_bottom, level_left, level_right);
}

bool Level::cmpStringToNumber(char arr[], int n) {
	int m;
	int i = 1;
	m = arr[0] - '0';
	while (arr[i] != ':') {
		m *= 10; // move digit over one place
		m += arr[i] - '0'; //tack on next digit
		i++;
	}
	//printf("cmpStringToNumber Received %i, Generated %i and is returning %i", n, m, (int)(n==m));
	return (m == n);
}

int Level::numberFromSubstring(char arr[], int begin, int end) {
	int m = 0;
	
	for (int i = begin; i < end; i++) {
		if (i > begin && !(begin == end)) {
			m *= 10;
		}
		if (i > begin+5 || i < 0) break;
		m += arr[i] - '0';
		printf("m: %i, arr[i] = %c\n", m, arr[i]);
	}
	//printf("Level::numberFromSubstring() - Received %i, %i. Returning %i\n", begin, end, m);
	return m;
}

void Level::makeSubstring(char arr[], int begin, int end, char sub[]) {
	int j = 0;
	for (int i = begin; i < end; i++) {
		sub[j] = arr[i];
		j++;
	}
	sub[j] = '\0';
	//printf("Level::makeSubstring received %i, %i and is passing back %s\n", begin, end, sub);
}

Level::~Level() {
	delete [] password;
	delete [] levelfile;
	for (int i = 0; i < h; i++) {
		delete [] layout[i];
	}
	delete [] layout;
}

void Level::drawTitleScreen(int which) {
	// three things responding to hover and click events...
	glColor3f(0.4f, 0.6f, 1.0f);
	glBegin(GL_QUADS);
		glVertex2f(4.0f, 6.0f);
		glVertex2f(4.0f, 5.0f);
		glVertex2f(8.0f, 5.0f);
		glVertex2f(8.0f, 6.0f);
		
		glVertex2f(4.0f, 4.0f);
		glVertex2f(4.0f, 3.0f);
		glVertex2f(8.0f, 3.0f);
		glVertex2f(8.0f, 4.0f);

		glVertex2f(4.0f, 2.0f);
		glVertex2f(4.0f, 1.0f);
		glVertex2f(8.0f, 1.0f);
		glVertex2f(8.0f, 2.0f);
	glEnd();
	{
		float newBlue = 0.5;
		float passBlue = 0.5f;
		float quitBlue = 0.5f;
		if (which == NEW_SELECTED)
			newBlue = 1.0f;
		else if (which == PASS_SELECTED)
			passBlue = 1.0f;
		else if (which == QUIT_SELECTED)
			quitBlue = 1.0f;
		
		glBegin(GL_QUADS);
			glColor3f(0.0f, 0.0f, newBlue);
			glVertex2f(4.2f, 5.8f);
			glVertex2f(4.2f, 5.2f);
			glVertex2f(7.8f, 5.2f);
			glVertex2f(7.8f, 5.8f);
			
			glColor3f(0.0f, 0.0f, passBlue);
			glVertex2f(4.2f, 3.8f);
			glVertex2f(4.2f, 3.2f);
			glVertex2f(7.8f, 3.2f);
			glVertex2f(7.8f, 3.8f);
	
			glColor3f(0.0f, 0.0f, quitBlue);
			glVertex2f(4.2f, 1.8f);
			glVertex2f(4.2f, 1.2f);
			glVertex2f(7.8f, 1.2f);
			glVertex2f(7.8f, 1.8f);
		glEnd();
	}
	
	
	glColor3f(1.0f, 1.0f, 1.0f);
	{
	char pStr[] = "New Game";
    int i, len;
    len = strlen(pStr);
    glRasterPos2f(5.0, 5.3);
    for(i = 0; i < len; ++i)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, pStr[i]);
	}
	
	{
	char pStr[] = "Password";
    int i, len;
    len = strlen(pStr);
    glRasterPos2f(5.0, 3.3);
    for(i = 0; i < len; ++i)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, pStr[i]);
	}
	
	{
	char pStr[] = "Quit";
    int i, len;
    len = strlen(pStr);
    glRasterPos2f(5.5, 1.3);
    for(i = 0; i < len; ++i)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, pStr[i]);
	}
}

int Level::animIntro(int count) {
	int result = 0;
	if (count >= 96) {
		//drawTitleScreen(NONE_SELECTED);
		result = 1;
	} else {
	float offset;
	glColor3f(0.4f, 0.6f, 1.0);
		if (count < 96 && count > 63) {
		offset = ((96.0 - count) / 32.0) * 4.0;
		
		glBegin(GL_QUADS);
			glVertex2f(4.0f, 6.0f);
			glVertex2f(4.0f, 5.0f);
			glVertex2f(8.0f, 5.0f);
			glVertex2f(8.0f, 6.0f);
		
			glVertex2f(4.0f, 4.0f);
			glVertex2f(4.0f, 3.0f);
			glVertex2f(8.0f, 3.0f);
			glVertex2f(8.0f, 4.0f);

			glVertex2f(4.0 - offset, 2.0f);
			glVertex2f(4.0 - offset, 1.0f);
			glVertex2f(8.0 - offset, 1.0f);
			glVertex2f(8.0 - offset, 2.0f);
		glEnd();
		
		
		//printf("count = %i && off = %f\n", count, offset);
		} else if (count < 64 && count > 31) {
			offset = ((64.0 - count) / 32.0) * 4.0;
			glBegin(GL_QUADS);
			glVertex2f(4.0f, 6.0f);
			glVertex2f(4.0f, 5.0f);
			glVertex2f(8.0f, 5.0f);
			glVertex2f(8.0f, 6.0f);
		
			glVertex2f(4+offset, 4.0f);
			glVertex2f(4+offset, 3.0f);
			glVertex2f(8+offset, 3.0f);
			glVertex2f(8+offset, 4.0f);
			glEnd();
		} else if (count < 32 && count > -1) {
			offset = ((32.0 - count) / 32.0) * 4.0;
			glBegin(GL_QUADS);
			glVertex2f(4.0f - offset, 6.0f);
			glVertex2f(4.0f - offset, 5.0f);
			glVertex2f(8.0f - offset, 5.0f);
			glVertex2f(8.0f - offset, 6.0f);
			glEnd();
		
		}
		
		//printf("being called %i\n", count);
	//	count++;
		glutPostRedisplay();
		//glutTimerFunc(500, myTimer, 2);
		
	}

	return result;
}
