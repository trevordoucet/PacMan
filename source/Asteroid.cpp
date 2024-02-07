//
//  Asteroid.cpp
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#include "Game.h"

#define _MAX_SPEED 0.05f
#define _DAMPING 0.98f
#define _ACC 0.008f


vec2 randvec(float min, float max){
  float p1 = (max-min)*((float)rand()/(float)RAND_MAX) - ((max-min)/2.0);
  float p2 = (max-min)*((float)rand()/(float)RAND_MAX) - ((max-min)/2.0);
  return vec2(p1,p2);
}

Asteroid::Asteroid(unsigned int index){
  state.cur_location = randvec(-1.0, 1.0);
  state.velocity = randvec(-1.0, 1.0);
  state.velocity = normalize(state.velocity)*_ACC;
  state.angle = 0.0;
  
  angle_increment = (rand() > RAND_MAX/2)? M_PI/32 : -M_PI/32 ;
  
  Asteroid_vert.push_back(vec2(-0.15, -0.15)); Asteroid_uv.push_back(vec2(0.0,0.0));
  Asteroid_vert.push_back(vec2(-0.15,  0.15)); Asteroid_uv.push_back(vec2(0.0,1.0));
  Asteroid_vert.push_back(vec2(0.15,  -0.15)); Asteroid_uv.push_back(vec2(1.0,0.0));
  Asteroid_vert.push_back(vec2(0.15,   0.15)); Asteroid_uv.push_back(vec2(1.0,1.0));
  
  asteroid_bbox[0] = vec2(-0.15, -0.15);
  asteroid_bbox[1] = vec2(0.15,   0.15);
  
  if(index == 1){
    std::string file_location = source_path + "sprites/asteroid_1.png";
    unsigned error = lodepng::decode(asteroid_im, im_width, im_height, file_location.c_str());
  }
  if(index == 2){
    std::string file_location = source_path + "sprites/asteroid_2.png";
    unsigned error = lodepng::decode(asteroid_im, im_width, im_height, file_location.c_str());
  }
  std::cout << im_width << " X " << im_height << " image loaded\n";
  
};




void Asteroid::update_state(vec4 extents){

  state.cur_location+=state.velocity;
  state.angle += angle_increment;

  if(state.cur_location.x < extents[0] || state.cur_location.x > extents[1]){
    state.cur_location.x = -state.cur_location.x;
  }
  if(state.cur_location.y < extents[2] ||state.cur_location.y > extents[3]){
    state.cur_location.y = -state.cur_location.y;
  }

}


void Asteroid::gl_init(){

  unsigned int Asteroid_vert_size = Asteroid_vert.size()*sizeof(vec2);
  unsigned int Asteroid_uv_size = Asteroid_uv.size()*sizeof(vec2);

  std::string vshader = source_path + "shaders/vshader_Texture.glsl";
  std::string fshader = source_path + "shaders/fshader_Texture.glsl";

  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

  GLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(GLvars.vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(GLvars.vertex_shader);
  check_shader_compilation(vshader, GLvars.vertex_shader);

  GLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(GLvars.fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(GLvars.fragment_shader);
  check_shader_compilation(fshader, GLvars.fragment_shader);

  GLvars.program = glCreateProgram();
  glAttachShader(GLvars.program, GLvars.vertex_shader);
  glAttachShader(GLvars.program, GLvars.fragment_shader);

  glLinkProgram(GLvars.program);
  check_program_link(GLvars.program);
  
  glGenTextures( 1, &GLvars.asteroid_texture );

  glBindTexture( GL_TEXTURE_2D, GLvars.asteroid_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, im_width, im_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &asteroid_im[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );


  glBindFragDataLocation(GLvars.program, 0, "fragColor");

  GLvars.vpos_location   = glGetAttribLocation(GLvars.program, "vPos");
  GLvars.vcolor_location = glGetAttribLocation(GLvars.program, "vTexCoord" );
  GLvars.M_location = glGetUniformLocation(GLvars.program, "M" );

  // Create a vertex array object
  glGenVertexArrays( 1, &GLvars.vao );
  //Set GL state to use vertex array object
  glBindVertexArray( GLvars.vao );

  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &GLvars.buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, GLvars.buffer );

  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, Asteroid_vert_size + Asteroid_uv_size, NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, Asteroid_vert_size, &Asteroid_vert[0] );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, Asteroid_vert_size, Asteroid_uv_size, &Asteroid_uv[0] );

  glEnableVertexAttribArray(GLvars.vpos_location);
  glEnableVertexAttribArray(GLvars.vcolor_location );

  glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( GLvars.vcolor_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(Asteroid_vert_size) );

  glBindVertexArray(0);
  
}

void Asteroid::draw(mat4 Projection){
  glUseProgram( GLvars.program );
  glBindVertexArray( GLvars.vao );
  
                 
  mat4  ModelView = Translate(state.cur_location.x, state.cur_location.y, 0.0)
                    * RotateZ(state.angle);
  
  glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, Projection*ModelView);
  
  
  glLineWidth(1.2);
  
  glBindTexture( GL_TEXTURE_2D, GLvars.asteroid_texture );
  glDrawArrays( GL_TRIANGLE_STRIP, 0, Asteroid_vert.size() );
  
  
  glBindVertexArray(0);
  glUseProgram(0);
  
}
