//
//  Space.cpp
//  FinalProject
//
//  Created by Adam Barson on 4/19/17.
//  Copyright © 2017 Adam Barson. All rights reserved.
//

#include "Space.hpp"
#include "GamePiece.hpp"
#include <iostream>
#include <fstream>
//#include <irrklang/irrKlang.h>

enum screen_state {menu, game_play, paused, game_over};
enum type {SHIP, ASTEROID,POWERUP};

GLdouble screen_width, screen_height;

int wd;

vector<Asteroid> asteroids;

vector<Circle_Coord> thrustFire;

vector<Circle_Coord> explosionFire;

Ship ship;
vector<Bullet> clip;
Powerup PU1(color{0, 0, 1});
Powerup PU2(color{0, 1, 0});

screen_state screen;

vector<Triangle_Coord>lives;

int mouse_x, mouse_y = 0;

int newNumLives;
int newNumLevel;

int start_ast;

bool keys[256];

bool respawning = false;

bool power_up = false;
bool power_up1 = false;
bool power_up2 = false; 
bool magazinetime = false;


ofstream write_discovered;

bool can_load = false;
bool loaded = false;

int level_change;

int counter = 0;
int power_counter = 0;
int level;
int destroyed;

int score = 0;

int gameOverWait = 0;

vector<int> stars_x;
vector<int> stars_y;



//winmm.lib
//conio.h

/*ALCdevice *device;

ALsizei size, freq;
ALenum format;
ALvoid *data;
ALboolean loop = AL_FALSE;




void test(){
    device = alcOpenDevice(NULL);
    if (!device){
        cout << "Oh no..." << endl;
    } else {
        cout << "Heck yeah." << endl;
    }
    
    
}
*/

void display_menu() {
    // draw a string message
    
    string message = "Click anywhere to begin";
    glColor3f(1, 1, 1);
    glRasterPos2i(170, 250);
    for (int i = 0; i < message.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, message[i]);
    }
    string asteroid_message = "Welcome to Asteroids!";
    glColor3f(1,1,1);
    glRasterPos2i(180, 150);
    for (int i = 0; i < asteroid_message.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, asteroid_message[i]);
    }
    if (can_load){
    string load_message = "Press 'L' to load";
    glColor3f(1, 1, 1);
    glRasterPos2i(210, 450);
    for (int i = 0; i < load_message.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, load_message[i]);
    }
    }
}


void display_paused(){
    string pause_message = "Paused";
    glColor3f(1, 1, 1);
    glRasterPos2i(250, 250);
    for (int i = 0; i < pause_message.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, pause_message[i]);
    }
    
    string resume_message = "'r' to resume";
    glColor3f(1, 1, 1);
    glRasterPos2i(220, 300);
    for (int i = 0; i < resume_message.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, resume_message[i]);
    }
    string save_message = "'s' to save";
    glColor3f(1, 1, 1);
    glRasterPos2i(220, 200);
    for (int i = 0; i < save_message.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, save_message[i]);
    }
}

void display_game_over(){
    string game_over_message = "GAME OVER";
    glColor3f(1, 0, 0);
    glRasterPos2i(230, 250);
    for (int i = 0; i < game_over_message.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, game_over_message[i]);
    }
    
    string new_game_message = "'n' for new game";
    glColor3f(1, 0, 0);
    glRasterPos2i(210, 300);
    for (int i = 0; i < new_game_message.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, new_game_message[i]);
    }
}

void display_level(int x, int y){
    char buffer[10]={'\0'};
    sprintf(buffer, "%d", level);
    string level_message = "Level: ";
    glColor3f(1, 1, 1);
    glRasterPos2i(x, y);
    for (int i = 0; i < level_message.length(); ++i) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, level_message[i]);
    }
    string level_message1 = buffer;
    glColor3f(1, 1, 1);
    glRasterPos2i(x + 65, y);
    for (int j = 0; j < level_message1.length(); ++j){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, level_message1[j]);
    }
}

void display_score(){
    char buffer[10]={'\0'};
    sprintf(buffer, "%d", score);
    string score_message = "Score: ";
    glColor3f(1, 1, 1);
    glRasterPos2i(460, 20);
    for (int i = 0; i < score_message.length(); i++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_message[i]);
    }
    string score_message1 = buffer;
    glColor3f(1,1,1);
    glRasterPos2i(525, 22);
    for (int j = 0; j < score_message1.length(); j++){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_message1[j]);
    }
}

void display_lives(){
    for (int i = 0; i < lives.size(); ++i){
        if (lives.size() > 0){
            lives[i].draw();
        }
    }
}

void display_stars(){
    
    string star = ".";
    glColor3f(1, 1, 1);
    for(int s = 0; s < stars_x.size(); s++){
        glRasterPos2i(stars_x[s], stars_y[s]);
        for(int i = 0; i < star.length(); ++i){
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, star[i]);
        }
    
    }
}

void star_coords(){
    for (int i = 0; i < 300; i++){
        stars_x.push_back(rand() % 600 + 1);
        stars_y.push_back(rand() % 600 + 1);
    }
}

void remove_life(){
    lives.erase(lives.begin() + lives.size() - 1);
}

void add_life(){
    lives.push_back(Triangle_Coord(lives.size() * 20 + 110, 5, 5));
}


void start(){
    ifstream in_file("save_state.txt");
    if (in_file){
        cout << "Would you like to load your previous game state? (y/n)" << endl;
        string user_input;
        while ((!(cin >> user_input) || (user_input != "y" && user_input != "n"))){
            cin.clear();
            string junk;
            getline(cin, junk);
            cout << "Enter a y/n. " << endl;
        }
        if (user_input == "y"){
            in_file >> start_ast;
            write_discovered.open("save_state.txt");
        } else{
            start_ast = 1;
            write_discovered.open("save_state.txt");
        }
     }    else{
            write_discovered.open("save_state.txt");
            //write_discovered("save_state.txt");
            cout << "No save file found. Making one for you now... " << endl;
            start_ast = 1;
            start_ast = 1;
        }
    }
void saveGame(){
    ofstream out_file;
    out_file.open("lives.txt");
    out_file<<ship.getNumLives()<<endl;
    out_file.close();
    out_file.open("levels.txt");
    out_file<<level<<endl;
    out_file.close();
}
void loadGame(){
    newNumLives = -1;
    newNumLevel = -1;
    ifstream in_file("lives.txt");
    if (in_file){
        in_file >> newNumLives;
        cout << newNumLives << endl;
        can_load = true;
    } else {
        can_load = false;
    }
    in_file.close();
    
    ifstream in_file2("levels.txt");
    if (in_file2){
        in_file2 >> newNumLevel;
        cout << newNumLevel << endl;
        can_load = true;
    } else {
        can_load = false;
    }
    in_file.close();


    
}

void explosion(Point2D loc, double size, type t){
    for (int i = 0; i < 20 + (int)size; ++i){
        Circle_Coord c(rand() % ((int)size / 6));
        if (t == SHIP){
            c.set_outside_color(1, 1, 0);
            c.set_color(1, 1, 0);
        }else if (t==POWERUP){
            c.set_outside_color(0.0/255, 100.0/255, 255.0/255);
            c.set_color(1,1,1);
        }
        else {
            c.set_outside_color(100.0/255, 100.0/255, 100.0/255);
            c.set_color(150.0/255, 150.0/255, 150.0/255);
        }
        c.set_center(loc.get_x() + rand() % ((int)size - (-(int)size) + 1) + (-(int)size), loc.get_y() + rand() % ((int)size - (-(int)size) + 1) + (-(int)size));
        explosionFire.push_back(c);
        
    }
}

void collisions(){
    if (asteroids.size() > 0 && clip.size() > 0){
    for (int i = 0; i < asteroids.size(); ++i){
        for (int j = 0; j < clip.size(); ++j){
            if (i > asteroids.size() || i < 0){
                break;
            }
            if (asteroids[i].detectCollision(clip[j])){
                explosion(asteroids[i].getLocation(), asteroids[i].getCircle().get_radius(), ASTEROID);
                asteroids.erase(asteroids.begin() + i);
                clip.erase(clip.begin() + j);
                i--;
                j--;
                destroyed++;
                cout << destroyed << endl;
                score += 100;
            }
        }
    }
    }
    if (!respawning && screen != game_over){
        for (int i = 0; i < asteroids.size(); ++i){
            if (asteroids[i].detectCollision(ship) && !respawning){
                explosion(asteroids[i].getLocation(), asteroids[i].getCircle().get_radius(), ASTEROID);
                explosion(ship.getLocation(), 30, SHIP);
                ship.regenerate();
                ship.setNumLives(ship.getNumLives()-1);
                if (ship.getNumLives() == 0){
                    screen = game_over;
                }
                remove_life();
                respawning = true;
                asteroids.erase(asteroids.begin() + i);
                i--;
                magazinetime=false;
            }
        }
    }
    if(power_up1){
        if (PU1.detectCollision(ship)){
            explosion(PU1.getLocation(), PU1.getCircle().get_radius(), POWERUP);
            power_up1 = false;
            magazinetime = true;
        }
    }
    if (power_up2){
        if (PU2.detectCollision(ship)){
            explosion(PU2.getLocation(), PU2.getCircle().get_radius(), POWERUP);
            power_up2 = false;
            add_life();
            ship.setNumLives(ship.getNumLives() + 1);
        }
   
    }}
void spawnThrustFire(){
    Circle_Coord c1(2);
    c1.set_center(Point2D(ship.getLocation().get_x() - ship.getDirection().get_x() * 40 - rand() % 8 + 1, ship.getLocation().get_y() -ship.getDirection().get_y() * 40));
    c1.set_color(1, 0, 0);
    c1.set_outside_color(1, 0, 0);
    
    Circle_Coord c2(2);
    c2.set_center(Point2D(ship.getLocation().get_x() - ship.getDirection().get_x() * 40 + rand() % 8 + 1, ship.getLocation().get_y() -ship.getDirection().get_y() * 40));
    c2.set_color(1, 0, 0);
    c2.set_outside_color(1, 0, 0);
    
    Circle_Coord c3(2);
    c3.set_center(Point2D(ship.getLocation().get_x() - ship.getDirection().get_x() * 40, ship.getLocation().get_y() -ship.getDirection().get_y() * 40 + rand() % 8 + 1));
    c3.set_color(1, 0, 0);
    c3.set_outside_color(1, 0, 0);
    
    Circle_Coord c4(2);
    c4.set_center(Point2D(ship.getLocation().get_x() - ship.getDirection().get_x() * 40, ship.getLocation().get_y() -ship.getDirection().get_y() * 40  - rand() % 8 + 1));
    c4.set_color(1, 0, 0);
    c1.set_outside_color(1, 0, 0);
    
    Circle_Coord c5(2);
    c5.set_center(Point2D(ship.getLocation().get_x() - ship.getDirection().get_x() * 40, ship.getLocation().get_y() -ship.getDirection().get_y() * 40));
    c5.set_color(1, 0, 0);
    //c5.set_outside_color(1, 0, 0);
    thrustFire.push_back(c1);
    thrustFire.push_back(c2);
    thrustFire.push_back(c3);
    thrustFire.push_back(c4);
    thrustFire.push_back(c5);
    
}

void reduceFire(){
    for (int i = 0; i < thrustFire.size(); ++i){
        thrustFire[i].set_center(Point2D(thrustFire[i].get_center_x() + rand() % (5 - (-5) + 1) + (-5), thrustFire[i].get_center_y() + rand() % (5 - (-5) + 1) + (-5)));
        thrustFire[i].set_radius(thrustFire[i].get_radius()-0.20);
        if (thrustFire[i].get_radius() < 0.01){
            thrustFire[i].set_radius(0);
            thrustFire.erase(thrustFire.begin() + i);
            i--;
        }
    }
    for (int i = 0; i < explosionFire.size(); ++i){
        explosionFire[i].set_center(Point2D(explosionFire[i].get_center_x() + rand() % (5 - (-5) + 1) + (-5), explosionFire[i].get_center_y() + rand() % (5 - (-5) + 1) + (-5)));
        explosionFire[i].set_radius(explosionFire[i].get_radius()-0.20);
        if (explosionFire[i].get_radius() < 0.01){
            explosionFire[i].set_radius(0);
            explosionFire.erase(explosionFire.begin() + i);
            i--;
        }
    }
}

void drawAllAsteroids(){
    for (int i = 0; i < asteroids.size(); ++i){
        asteroids[i].drawShape();
    }
}

void moveAllAsteroids(){
    for (int i = 0; i < asteroids.size(); ++i){
        asteroids[i].move();
    }
}
void drawBullets(){
    for(int i=0; i<clip.size();++i){
        clip[i].drawShape();
    }
}
void moveBullets(){
    for(int i=0; i<clip.size();++i){
        clip[i].move();
        if (clip[i].getLifeTime() > 50){
            clip.erase(clip.begin() + i);
            i--;
        }
    }
}



void generateBullet(){
    if (!respawning){
        if (ship.getShotDelay() == 0){
            if (!magazinetime){
                clip.push_back(Bullet(Point2D(ship.getDirection().get_x(),ship.getDirection().get_y()), Point2D(ship.getLocation().get_x(), ship.getLocation().get_y())));
                ship.shoot();
            } else {
                clip.push_back(Bullet(Point2D(ship.getDirection().get_x(),ship.getDirection().get_y()), Point2D(ship.getLocation().get_x() + ship.getDirection().get_y() * 15, ship.getLocation().get_y() - ship.getDirection().get_x() * 15 )));
                clip.push_back(Bullet(Point2D(ship.getDirection().get_x(),ship.getDirection().get_y()), Point2D(ship.getLocation().get_x() - ship.getDirection().get_y() * 15, ship.getLocation().get_y() + ship.getDirection().get_x() * 15)));
                ship.shoot();
            }
        } 
    }
}



void animation(){
    display_stars();
    if (gameOverWait < 100){
        if (screen != game_over){
            ship.drawShape();
        }
        display_lives();
        drawAllAsteroids();
        display_score();
        display_level(20,20);
        for (int i = 0; i < thrustFire.size(); ++i){
            thrustFire[i].draw();
        }
    
        for (int i = 0; i < explosionFire.size(); ++i){
            explosionFire[i].draw();
        }
        drawBullets();
        if (power_up1){
            PU1.drawShape();
        }
        if (power_up2){
            PU2.drawShape();
        }
    
        if (level_change!=0){
            display_level(200, 300);
        }
        
    }
}

void startGame(){
    
    loadGame();
    asteroids.clear();
    clip.clear();
    score = 0;
    destroyed = 0;
    gameOverWait = 0;
    screen = menu;
    ship.regenerate();
    ship.setNumLives(3);
    level = 1;
    respawning = true;
    
    
    for (int i = 0; i < ship.getNumLives(); ++i){
        add_life();
    }
    
    power_up1 = false;
    power_up2 = false;
    cout << "Number of asteroids to start:" << start_ast << endl;
}

void init() {
    
   // test();
    //start();
    ship = Ship();
    startGame();
    screen_width = 600;
    screen_height = 600;
    star_coords();
}


/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glColor3f(0.0f, 0.0f, 1.0f);
}

void levelHandler(int l){
    if (level_change == 0){
        counter++;
        int levelTime = 80 / level + 20;
        if (levelTime < 30){
            levelTime = 30;
        }
        double levelAmount = 8 * level;
        double cap = level;
        if (cap > 8){
            cap = 8;
        }
        while (counter % levelTime == 0 && asteroids.size() < cap && levelAmount - destroyed - asteroids.size() > 0){
            asteroids.push_back(Asteroid());
        }
        if (destroyed == levelAmount){
            level++;
            level_change = 1;
            destroyed = 0;
            counter = 0;
            if (level % 3 == 0){
                power_up1 = true;
            } else if (level % 4 == 0){
                power_up2 = true;
            }
        }
    } else {
        level_change++;
        if (level_change > 100){
            level_change = 0;
            for (int i = 0; i < level + 2; ++i){
                asteroids.push_back(Asteroid());
            }
        }
    }
    
}

void play(){
    
    if (screen == game_play || (screen == game_over && gameOverWait < 100)){
        if (keys[GLUT_KEY_LEFT]){
            ship.rotateL();
        }
        if (keys[GLUT_KEY_RIGHT]){
            ship.rotateR();
        }
        if (keys[GLUT_KEY_UP]){
            if (screen != game_over){
                ship.move();
                spawnThrustFire();
            }
            
        }
        if (keys[32]){
            generateBullet();
        }
        if (power_counter==1000){
            power_up=true;
            power_counter-=1000;
        }
        else{
            power_counter++;

        }
        moveAllAsteroids();
        reduceFire();
        collisions();
        moveBullets();
        
        if (power_up1){
            PU1.move();
        }
        if (power_up2){
            PU2.move();
        }
        ship.update();
        levelHandler(level);
        if (respawning){
            ship.setRespawning(ship.getRespawning() + 1);
            ship.blink();
            if (ship.getRespawning() == 80){
                ship.setRespawning(0);
                respawning = false;
            }
        }
    }
}

/* Handler for window-repaint event. Call back when the window first appears and
 whenever the window needs to be re-painted. */
void display() {
    // tell OpenGL to use the whole window for drawing
    glViewport(0, 0, screen_width, screen_height);
    
    // do an orthographic parallel projection with the coordinate
    // system set to first quadrant, limited by screen/window size
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, screen_width, screen_height, 0.0, -1.f, 1.f);
    
    
    
    glClear(GL_COLOR_BUFFER_BIT);   // Clear the color buffer with current clearing color
    
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    animation();
    
    switch(screen) {
        case menu:
            display_menu();
            break;
        case game_play:
            
            //play();
            break;
        case game_over:
            if (gameOverWait > 100){
                display_game_over();
            } else {
                gameOverWait++;
            }
            
            break;
        case paused:
            display_paused();
    }
    
    glFlush();
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbd(unsigned char key, int x, int y)
{
    // escape
    if (key == 27) {
        write_discovered << asteroids.size();
        glutDestroyWindow(wd);
        exit(0);
    }
    if (key == 32){
        keys[32] = true;
    }
    
    if (key == 'G') {
        glColor3f(0.0f, 1.0f, 0.0f);
    }
    
    if (key == 'R') {
        glColor3f(1.0f, 0.0f, 0.0f);
    }
    
    if (key == 'p' && screen == game_play) {
        screen = paused;
    }
    
    if (key == 'r' && screen == paused){
        screen = game_play;
        
    }
    if (screen == paused && key == 115){
        saveGame();
    
    }
    if (screen == menu && key == 108){
        if (can_load){
            ship.setNumLives(newNumLives);
            level = newNumLevel;
            for (int i = 0; i < 3; ++i){
                remove_life();
            }
            for (int i = 0; i < newNumLives; ++i){
                add_life();
            }
            screen = game_play;
            loaded = true;
            level_change = 1;
        } 
        
    }

    
    if (screen == game_over && key == 'n'){
        startGame();
    }
    
    glutPostRedisplay();
    
    return;
}

// http://www.theasciicode.com.ar/ascii-control-characters/escape-ascii-code-27.html
void kbU(unsigned char key, int x, int y)
{
    
    if (key == 32){
        keys[32] = false;
    }
    
    
    glutPostRedisplay();
    
    return;
}

void kbdS(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            //maybe break in the future?
            break;
        case GLUT_KEY_LEFT:
            keys[GLUT_KEY_LEFT] = true;
            break;
        case GLUT_KEY_RIGHT:
            keys[GLUT_KEY_RIGHT] = true;
            break;
        case GLUT_KEY_UP:
            keys[GLUT_KEY_UP] = true;
            break;
            
    }
    glutPostRedisplay();
    return;
}

void kbdU(int key, int x, int y) {
    switch(key) {
        case GLUT_KEY_DOWN:
            
            break;
        case GLUT_KEY_LEFT:
            keys[GLUT_KEY_LEFT] = false;
            break;
        case GLUT_KEY_RIGHT:
            keys[GLUT_KEY_RIGHT] = false;
            break;
        case GLUT_KEY_UP:
            keys[GLUT_KEY_UP] = false;
            break;
            
    }
    
    glutPostRedisplay();
    
    return;
}

void cursor(int x, int y) {
    mouse_x = x;
    mouse_y = y;
    glutPostRedisplay();
}

// button will be GLUT_LEFT_BUTTON or GLUT_RIGHT_BUTTON
// state will be GLUT_UP or GLUT_DOWN
void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && screen == menu) {
        screen = game_play;
        level_change = 1;
    }
    glutPostRedisplay();
}


void timer(int extra) {
    play();
    
    glutTimerFunc(30, timer, 0);
    glutPostRedisplay();
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    
    init();
    
    glutInit(&argc, argv);          // Initialize GLUT
    
    glutInitDisplayMode(GLUT_RGBA);
    
    glutInitWindowSize((int)screen_width, (int)screen_height);
    glutInitWindowPosition(100, 100); // Position the window's initial top-left corner
    /* create the window and store the handle to it */
    wd = glutCreateWindow("Asteroids!" /* title */ );
    
    // Register callback handler for window re-paint event
    glutDisplayFunc(display);
    
    // Our own OpenGL initialization
    initGL();
    
    // register keyboard press event processing function
    // works for numbers, letters, spacebar, etc.
    glutKeyboardFunc(kbd);
    
    glutKeyboardUpFunc(kbU);
    
    // register special event: function keys, arrows, etc.
    glutSpecialFunc(kbdS);
    
    glutSpecialUpFunc(kbdU);
    
   // glutIgnoreKeyRepeat(true);
    
    // handles mouse movement
    glutPassiveMotionFunc(cursor);
    
    // handles mouse click
    glutMouseFunc(mouse);
    
    // handles timer
    glutTimerFunc(0, timer, 0);
    
    // Enter the event-processing loop
    glutMainLoop();
    return 0;
}
