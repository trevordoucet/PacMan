//
//  Ship.h
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#ifndef __Asteroids__Ship__
#define __Asteroids__Ship__

#include "Game.h"

using namespace tcg;

class Ship{

  friend class Game;

  std::vector < vec2 > ship_pos;
  std::vector < vec2 > ship_uv;
  
  std::vector<unsigned char> ship_im;
  unsigned ship_im_width, ship_im_height;
  
  vec2 ship_bbox[2];
  std::vector < vec2 > outline;

  std::vector<unsigned char> ship_t_im;
  unsigned ship_t_im_width, ship_t_im_height;
  
  std::vector<unsigned char> ship_left;
  unsigned s_left_width, s_left_height;
  
  vec2 left_bbox[2];
  std::vector < vec2 > outline_left;

  std::vector<unsigned char> ship_right;
  unsigned s_right_width, s_right_height;
  
  vec2 right_bbox[2];
  std::vector < vec2 > outline_right;

  std::vector<unsigned char> ship_left_t;
  unsigned s_left_t_width, s_left_t_height;
  
  std::vector<unsigned char> ship_right_t;
  unsigned s_right_t_width, s_right_t_height;
  
  //Initialized in constructor
  float max_speed;
  float damping_fact;
  float accel;

  enum{ _NO_TURN, _TURN_LEFT, _TURN_RIGHT};
  
  //Ship State
  struct {
    vec2 cur_location;
    vec4 pointing;
    vec2 velocity;
    bool thruster_on;
    uint8_t turning;
    float angle;
  } state;
  
  //OpenGL variables for a ship
  struct {
    GLuint vao;           //Vertex array object
    GLuint program;       //shader program
    GLuint buffer;        //Vertex buffer objects
    GLuint vertex_shader, fragment_shader;  //Shaders
    GLint vpos_location, vtex_location;   //reference to pos and color in shaders
    GLint M_location;     //Reference to matrix in shader
    GLuint ship_texture;
    GLuint ship_t_texture;
    GLuint left_texture;
    GLuint right_texture;
    GLuint left_t_texture;
    GLuint right_t_texture;
  } GLvars;
  
  
public:
  
  

  Ship();
  
  inline void start_thruster(){ state.thruster_on= true;}
  inline void stop_thruster() { state.thruster_on= false;}
  
  inline void rotateLeft() {
    state.turning = _TURN_LEFT;
    state.angle+= M_PI/16;
    mat4 R = RotateZ(state.angle);
    state.pointing = R*vec4(0.0f, -1.0f, 0.0f, 0.0f);
  }
  
  inline void stopTurn(){
    state.turning = _NO_TURN;
  }
  
  inline void rotateRight(){
    state.turning = _TURN_RIGHT;
    state.angle-= M_PI/16;
    mat4 R = RotateZ(state.angle);
    state.pointing = R*vec4(0.0f, -1.0f, 0.0f, 0.0f);
  }
  
  Bullet pew_pew(){
    return Bullet(state.cur_location,  state.velocity + 5.0f * accel * vec2(state.pointing.x, state.pointing.y));
  }
  
  void update_state(vec4 extents);
  
  void gl_init();
  void gl_init_Debug();
  
  void draw(mat4 proj);
  void draw_Debug(mat4 proj);
  
};


#endif /* defined(__Asteroids__Ship__) */
