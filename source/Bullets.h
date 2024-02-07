//
//  Bullet.H
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#ifndef __Asteroids__Bullets__
#define __Asteroids__Bullets__

#include "Game.h"

using namespace tcg;

class Bullet{
  friend class Bullets;
  
  //Bullet State
  struct {
    vec2 cur_location;
    vec2 move;
  } state;

public:
  
  Bullet(vec2 cur_location, vec2 move){
    state.cur_location = cur_location;
    state.move = move;
  }
  
  void update_state(){
    state.cur_location+=state.move;
  }
  
};

class Bullets{
  
  //OpenGL variables for a bullet
  struct {
    GLuint vao;           //Vertex array object
    GLuint program;       //shader program
    GLuint buffer;        //Vertex buffer objects
    GLuint vertex_shader, fragment_shader;  //Shaders
    GLint vpos_location, vcolor_location;   //reference to pos and color in shaders
    GLint M_location;     //Reference to matrix in shader
  } GLvars;
  
public:
  std::list < Bullet > bullets;
  std::vector < vec2 > bullets_vec;
  
  Bullets(){ }
  
  ~Bullets(){
    bullets.clear();
    bullets_vec.clear();
  }
  
  void gl_init();
  
  void update_state(vec4 extents);
  void draw(mat4 proj);
  
  void add(Bullet b){ bullets.push_back(b); }
  
};



#endif /* defined(__Asteroids__Bullet__) */
