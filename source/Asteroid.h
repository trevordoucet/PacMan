//
//  Asteroid.h
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#ifndef __Asteroids__Asteroid__
#define __Asteroids__Asteroid__

#include "Game.h"

using namespace tcg;

class Asteroid{
  
  friend class Game;

  std::vector < vec2 > Asteroid_vert;
  std::vector < vec2 > Asteroid_uv;
  
  vec2 asteroid_bbox[2];
  
  std::vector<unsigned char> asteroid_im;
  unsigned im_width, im_height;

  
  float angle_increment;
  
  //Asteroid State
  struct {
    vec2 cur_location;
    vec2 velocity;
    float angle;
  } state;
  
  //OpenGL variables for a Asteroid
  struct {
    GLuint vao;           //Vertex array object
    GLuint program;       //shader program
    GLuint buffer;        //Vertex buffer objects
    GLuint vertex_shader, fragment_shader;  //Shaders
    GLint vpos_location, vcolor_location;   //reference to pos and color in shaders
    GLint M_location;     //Reference to matrix in shader
    GLuint asteroid_texture;
  } GLvars;
  
public:

  Asteroid(unsigned int index);
  
  void update_state(vec4 extents);
  
  void gl_init();  
  
  void draw(mat4 proj);
  
};


#endif /* defined(__Asteroids__Asteroid__) */
