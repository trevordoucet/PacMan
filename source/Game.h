//
//  Game.h
//  Asteroids
//
//  Created by Brian Summa on 9/7/15.
//
//

#ifndef Asteroids_Game_h
#define Asteroids_Game_h

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#include <list>
#include <vector>
#include <utils/shader_process.h>
#include <path/path.h>

#include <utils/vec.h>
#include <utils/mat.h>
#include <utils/lodepng.h>
#include <utils/loadTexture.h>


#include "Bullets.h"
#include "Ship.h"
#include "Asteroid.h"

class Game{

public:
  Ship *ship;
  std::vector <Asteroid *> asteroids;
  Bullets bullets;

  //left right bottom top of window
  vec4 screen_extents;
  
private:
  
  bool game_over;
  
  std::vector<unsigned char> game_over_im;
  unsigned go_width, go_height;

  struct {
    GLuint vao;           //Vertex array object
    GLuint program;       //shader program
    GLuint buffer;        //Vertex buffer objects
    GLuint vertex_shader, fragment_shader;  //Shaders
    GLint vpos_location, vtex_location;   //reference to pos and color in shaders
    GLint M_location;     //Reference to matrix in shader
    GLuint texture;
  } GOGLvars;

  
public:
  Game();
  
  ~Game(){
    delete ship;
    asteroids.clear();
  }
  
  void init(){
    for(unsigned int i=0; i < asteroids.size(); i++){
      asteroids[i]->gl_init();
    }
    ship->gl_init();
    bullets.gl_init();
    gl_init();
  }
  
  void draw(mat4 proj){
    ship->draw(proj);
    bullets.draw(proj);
    for(unsigned int i=0; i < asteroids.size(); i++){
      asteroids[i]->draw(proj);
    }
    
    if(game_over){
      draw_game_over(proj);
    }
  }
  
  void update(){
    for(unsigned int i=0; i < asteroids.size(); i++){
      asteroids[i]->update_state(screen_extents);
    }
    ship->update_state(screen_extents);
    bullets.update_state(screen_extents);
    
    game_over = testIntersections();
  }
  
private:
  
  void gl_init();
  void draw_game_over(mat4 proj);
  
  bool testIntersections();
  
};


#endif
