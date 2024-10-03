//Header file for the Level class..... the class that powers this entire game

#define MOVE_RIGHT		3
#define MOVE_LEFT		2
#define MOVE_UP			1
#define	FACING_RIGHT	1
#define FACING_LEFT		2

class Level {
public:
	Level(); /*get level 1*/
	Level(int n); /* get level n */
	~Level(); //... deconstructor. if you didnt know that, you need to learn c++
	void move(int dir); /* try to move in direction dir (left, right, up) */
	void pickup(); //try to pickup or put down a brick
	void draw(); /* to be used in display() */
	void drawTitleScreen(int which);
	void reset(); /*reset level*/
	// debug purposes only
	void printLevel(); //used when i created the class.. not useful anymore
	void newLevel(int n); //start level n.... called when level (n-1) is beat
	void resetLevel(); //reset the current level(place blocks back and move triangle back
	float block_color[3];
	int animIntro(int count); //the opening scene
private:
	int **layout;
	int mylevel;
	int h;
	int w;
	char *password;
	char *levelfile;
	int x;
	int y;
	int current;
	int facing;
	bool moving;
	int total;
	char pieces[5];
	bool holding;
	void zero(char arr[], int len); //zero len elements of array arr[]
	bool is_a_comment(char first); //check to see if the line read in is a comment (starts with #)
	bool cmpStringToNumber(char arr[], int n); //check to see if "n" is equal to m
	int numberFromSubstring(char arr[], int begin, int end); //name says it all
	void makeSubstring(char arr[], int begin, int end, char sub[]); //name says it all
	
};