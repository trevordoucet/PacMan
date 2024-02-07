//
//  Ship.cpp
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#include "Game.h"

void Bullets::gl_init(){
  
  std::string vshader = source_path + "shaders/vshader_Bullets.glsl";
  std::string fshader = source_path + "shaders/fshader_Bullets.glsl";
  
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
  
  glBindFragDataLocation(GLvars.program, 0, "fragColor");
  
  GLvars.vpos_location   = glGetAttribLocation(GLvars.program, "vPos");
  GLvars.M_location = glGetUniformLocation( GLvars.program, "M" );

  // Create a vertex array object
  glGenVertexArrays( 1, &GLvars.vao );
  //Set GL state to use vertex array object
  glBindVertexArray( GLvars.vao );
  
  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &GLvars.buffer );
  
  glEnableVertexAttribArray(GLvars.vpos_location);
  glEnableVertexAttribArray(GLvars.vcolor_location );
  
  glBindVertexArray(0);

  delete[] vertex_shader_source;
  delete[] fragment_shader_source;

}

void Bullets::draw(mat4 proj){
  
  glPointSize(10.0);
  glUseProgram(GLvars.program);
  glBindVertexArray( GLvars.vao );
  glBindBuffer( GL_ARRAY_BUFFER, GLvars.buffer );
  
  glBufferData( GL_ARRAY_BUFFER, bullets_vec.size()*sizeof(vec2), &bullets_vec[0], GL_STATIC_DRAW );
  glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

  glUniformMatrix4fv(GLvars.M_location, 1, GL_TRUE, proj);
  glDrawArrays(GL_POINTS, 0, bullets_vec.size());

  
  glBindVertexArray(0);
  glBindBuffer( GL_ARRAY_BUFFER, 0);
  glPointSize(1.0);

  
}

void Bullets::update_state(vec4 extents){
  bullets_vec.clear();
  //Also need retire bullets that are no longer needed;
  for (std::list<Bullet>::iterator it = bullets.begin(); it != bullets.end();){
    it->update_state();
    if(it->state.cur_location.x < extents[0] ||
       it->state.cur_location.x > extents[1] ||
       it->state.cur_location.y < extents[2] ||
       it->state.cur_location.y > extents[3]){
      it = bullets.erase(it);
    }else{
      bullets_vec.push_back(it->state.cur_location);
      it++;
    }
  }
  
}
