//
//  Ship.cpp
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#include "Game.h"

#define _MAX_SPEED 0.05f
#define _DAMPING 0.98f
#define _ACC 0.008f

Ship::Ship(){
  
  state.cur_location = vec2(0.0f,0.0f);
  state.pointing = vec4(0.0f,-1.0f,0.0f,0.0f);
  state.velocity = vec2(0.0f,0.0f);
  state.thruster_on = false;
  state.turning = _NO_TURN;
  state.angle = 0.0;
  
  max_speed = 0.05;
  damping_fact = 0.98;
  accel = 0.008;
  
  ship_pos.resize(16);
  ship_uv.resize(16);
    
  //Ship
  ship_pos[0] = vec2(-0.075, -0.075);     ship_uv[0] = vec2(0.0,0.0);
  ship_pos[1] = vec2(-0.075,  0.075);     ship_uv[1] = vec2(0.0,1.0);
  ship_pos[2] = vec2(0.075,  -0.075);     ship_uv[2] = vec2(1.0,0.0);
  ship_pos[3] = vec2(0.075,   0.075);     ship_uv[3] = vec2(1.0,1.0);
  
  //Min and max coordinates
  ship_bbox[0] = ship_pos[0];
  ship_bbox[1] = ship_pos[3];
  
  //Ship with thruster (because the image gets bigger 39px -> 43px)
  ship_pos[4] = vec2(-0.075, -0.075);     ship_uv[4] = vec2(0.0,0.0);
  ship_pos[5] = vec2(-0.075,  0.0903846);     ship_uv[5] = vec2(0.0,1.0);
  ship_pos[6] = vec2(0.075,  -0.075);     ship_uv[6] = vec2(1.0,0.0);
  ship_pos[7] = vec2(0.075,   0.0903846);     ship_uv[7] = vec2(1.0,1.0);

  //left or right turn (skinnier)
  ship_pos[8] = vec2(-0.057692, -0.075);     ship_uv[8] = vec2(0.0,0.0);
  ship_pos[9] = vec2(-0.057692,  0.075);     ship_uv[9] = vec2(0.0,1.0);
  ship_pos[10] = vec2(0.057692,  -0.075);     ship_uv[10] = vec2(1.0,0.0);
  ship_pos[11] = vec2(0.057692,   0.075);     ship_uv[11] = vec2(1.0,1.0);
  
  left_bbox[0] = ship_pos[8];
  left_bbox[1] = ship_pos[11];

  //left or right turn with thruster
  ship_pos[12] = vec2(-0.057692, -0.075);     ship_uv[12] = vec2(0.0,0.0);
  ship_pos[13] = vec2(-0.057692,  0.0903846);     ship_uv[13] = vec2(0.0,1.0);
  ship_pos[14] = vec2(0.057692,  -0.075);     ship_uv[14] = vec2(1.0,0.0);
  ship_pos[15] = vec2(0.057692,   0.0903846);     ship_uv[15] = vec2(1.0,1.0);

  right_bbox[0] = ship_pos[12];
  right_bbox[1] = ship_pos[15];

  std::string file_location = source_path + "sprites/ship.png";
  unsigned error = lodepng::decode(ship_im, ship_im_width, ship_im_height, file_location.c_str());
  std::cout << ship_im_width << " X " << ship_im_height << " image loaded\n";
  
  for(unsigned int i=0; i < ship_im_width*ship_im_height; i++){
    unsigned int x = i%ship_im_width;
    unsigned int y = i/ship_im_width;
    
    bool neighbor_alpha = false;
    if(ship_im[4*i+3] != 0){
      if(x > 0){
        if(ship_im[4*(i-1)+3] == 0){
          neighbor_alpha = true;
        }
      }
      
      if(x < ship_im_width-1){
        if(ship_im[4*(i+1)+3] == 0){
          neighbor_alpha = true;
        }
      }
      
      if(y > 0){
        if(ship_im[4*(i-ship_im_width)+3] == 0){
          neighbor_alpha = true;
        }
      }
      
      if(y < ship_im_height-1){
        if(ship_im[4*(i+ship_im_width)+3] == 0){
          neighbor_alpha = true;
        }
      }
    }
    
    if(neighbor_alpha){
      outline.push_back(vec2((float)x/(float)ship_im_width,(float)y/(float)ship_im_height));
    }
  }
  
  for(unsigned int i=0; i < outline.size(); i++){
    outline[i].x =  ship_bbox[0].x + outline[i].x*(ship_bbox[1].x-ship_bbox[0].x);
    outline[i].y =  ship_bbox[0].y + outline[i].y*(ship_bbox[1].y-ship_bbox[0].y);
  }
  
  file_location = source_path + "sprites/ship_thrust.png";
  error = lodepng::decode(ship_t_im, ship_t_im_width, ship_t_im_height, file_location.c_str());
  std::cout << ship_t_im_width << " X " << ship_t_im_height << " image loaded\n";

  file_location = source_path + "sprites/ship_right.png";
  error = lodepng::decode(ship_left, s_left_width, s_left_height, file_location.c_str());
  std::cout << s_left_width << " X " << s_left_height << " image loaded\n";
  
  for(unsigned int i=0; i < s_left_width*s_left_height; i++){
    unsigned int x = i%s_left_width;
    unsigned int y = i/s_left_width;
    
    bool neighbor_alpha = false;
    if(ship_left[4*i+3] != 0){
      if(x > 0){
        if(ship_left[4*(i-1)+3] == 0){
          neighbor_alpha = true;
        }
      }
      
      if(x < s_left_width-1){
        if(ship_left[4*(i+1)+3] == 0){
          neighbor_alpha = true;
        }
      }
      
      if(y > 0){
        if(ship_left[4*(i-s_left_width)+3] == 0){
          neighbor_alpha = true;
        }
      }
      
      if(y < s_left_height-1){
        if(ship_left[4*(i+s_left_width)+3] == 0){
          neighbor_alpha = true;
        }
      }
    }
    
    if(neighbor_alpha){
      outline_left.push_back(vec2((float)x/(float)s_left_width,(float)y/(float)s_left_height));
    }
  }
  for(unsigned int i=0; i < outline_left.size(); i++){
    outline_left[i].x =  left_bbox[0].x + outline_left[i].x*(left_bbox[1].x-left_bbox[0].x);
    outline_left[i].y =  left_bbox[0].y + outline_left[i].y*(left_bbox[1].y-left_bbox[0].y);
  }

  file_location = source_path + "sprites/ship_left.png";
  error = lodepng::decode(ship_right, s_right_width, s_right_height, file_location.c_str());
  std::cout << s_right_width << " X " << s_right_height << " image loaded\n";
  
  for(unsigned int i=0; i < s_right_width*s_right_height; i++){
    unsigned int x = i%s_right_width;
    unsigned int y = i/s_right_width;
    
    bool neighbor_alpha = false;
    if(ship_right[4*i+3] != 0){
      if(x > 0){
        if(ship_right[4*(i-1)+3] == 0){
          neighbor_alpha = true;
        }
      }
      
      if(x < s_right_width-1){
        if(ship_right[4*(i+1)+3] == 0){
          neighbor_alpha = true;
        }
      }
      
      if(y > 0){
        if(ship_right[4*(i-s_right_width)+3] == 0){
          neighbor_alpha = true;
        }
      }
      
      if(y < s_right_height-1){
        if(ship_right[4*(i+s_right_width)+3] == 0){
          neighbor_alpha = true;
        }
      }
    }
    
    if(neighbor_alpha){
      outline_right.push_back(vec2((float)x/(float)s_right_width,(float)y/(float)s_right_height));
    }
  }

  for(unsigned int i=0; i < outline_right.size(); i++){
    outline_right[i].x =  right_bbox[0].x + outline_right[i].x*(right_bbox[1].x-right_bbox[0].x);
    outline_right[i].y =  right_bbox[0].y + outline_right[i].y*(right_bbox[1].y-right_bbox[0].y);
  }

  file_location = source_path + "sprites/ship_right_thrust.png";
  error = lodepng::decode(ship_left_t, s_left_t_width, s_left_t_height, file_location.c_str());
  std::cout << s_left_t_width << " X " << s_left_t_height << " image loaded\n";

  file_location = source_path + "sprites/ship_left_thrust.png";
  error = lodepng::decode(ship_right_t, s_right_t_width, s_right_t_height, file_location.c_str());
  std::cout << s_right_t_width << " X " << s_right_t_height << " image loaded\n";


};


void Ship::update_state(vec4 extents){
  if(state.thruster_on){
    state.velocity += accel* vec2(state.pointing.x, state.pointing.y);
    if(length(state.velocity) > max_speed){
      state.velocity = normalize(state.velocity);
      state.velocity*=max_speed;
    }
  }
  
  state.velocity*= damping_fact;
  state.cur_location+=state.velocity;
  

  if(state.cur_location.x < extents[0] || state.cur_location.x > extents[1]){
    state.cur_location.x = -state.cur_location.x;
  }
  if(state.cur_location.y < extents[2] ||state.cur_location.y > extents[3]){
    state.cur_location.y = -state.cur_location.y;
  }

}


void Ship::gl_init(){

  unsigned int ship_vert_size = ship_pos.size()*sizeof(vec2);
  unsigned int ship_uv_size = ship_uv.size()*sizeof(vec2);

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
  glUseProgram(GLvars.program);
  
  glBindFragDataLocation(GLvars.program, 0, "fragColor");

  GLvars.vpos_location   = glGetAttribLocation(GLvars.program, "vPos");
  GLvars.vtex_location = glGetAttribLocation(GLvars.program, "vTexCoord" );
  GLvars.M_location = glGetUniformLocation(GLvars.program, "M" );
  
  glGenTextures( 1, &GLvars.ship_texture );
  glGenTextures( 1, &GLvars.ship_t_texture );
  glGenTextures( 1, &GLvars.left_texture );
  glGenTextures( 1, &GLvars.right_texture );
  glGenTextures( 1, &GLvars.left_t_texture );
  glGenTextures( 1, &GLvars.right_t_texture );

  glBindTexture( GL_TEXTURE_2D, GLvars.ship_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, ship_im_width, ship_im_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &ship_im[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, GLvars.ship_t_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, ship_t_im_width, ship_t_im_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &ship_t_im[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  
  glBindTexture( GL_TEXTURE_2D, GLvars.left_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s_left_width, s_left_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &ship_left[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, GLvars.right_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s_right_width, s_right_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &ship_right[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  
  glBindTexture( GL_TEXTURE_2D, GLvars.left_t_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s_left_t_width, s_left_t_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &ship_left_t[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  glBindTexture( GL_TEXTURE_2D, GLvars.right_t_texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, s_right_t_width, s_right_t_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &ship_right_t[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

  // Create a vertex array object
  glGenVertexArrays( 1, &GLvars.vao );
  //Set GL state to use vertex array object
  glBindVertexArray( GLvars.vao );

  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &GLvars.buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, GLvars.buffer );

  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, ship_vert_size + ship_uv_size, NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, ship_vert_size, &ship_pos[0] );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, ship_vert_size, ship_uv_size, &ship_uv[0] );

  glEnableVertexAttribArray(GLvars.vpos_location);
  glEnableVertexAttribArray(GLvars.vtex_location );

  glVertexAttribPointer( GLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( GLvars.vtex_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(ship_vert_size) );

  glBindVertexArray(0);
  
}


void Ship::draw(mat4 Projection){
  glUseProgram( GLvars.program );
  glBindVertexArray( GLvars.vao );
    
  mat4 ModelView = Translate(state.cur_location.x, state.cur_location.y, 0)
                   * RotateZ(state.angle);
  

  glUniformMatrix4fv( GLvars.M_location, 1, GL_TRUE, Projection*ModelView);
  
  if(!state.thruster_on && state.turning == _NO_TURN){
    glBindTexture( GL_TEXTURE_2D, GLvars.ship_texture );
    glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
  }
  
  if(state.thruster_on && state.turning == _NO_TURN){
    glBindTexture( GL_TEXTURE_2D, GLvars.ship_t_texture );
    glDrawArrays( GL_TRIANGLE_STRIP, 4, 4 );
  }
  
  if(!state.thruster_on && state.turning == _TURN_LEFT){
    glBindTexture( GL_TEXTURE_2D, GLvars.left_texture );
    glDrawArrays( GL_TRIANGLE_STRIP, 8, 4 );
  }
  
  if(!state.thruster_on && state.turning == _TURN_RIGHT){
    glBindTexture( GL_TEXTURE_2D, GLvars.right_texture  );
    glDrawArrays( GL_TRIANGLE_STRIP, 8, 4 );
  }
  
  if(state.thruster_on && state.turning == _TURN_LEFT){
    glBindTexture( GL_TEXTURE_2D, GLvars.left_t_texture );
    glDrawArrays( GL_TRIANGLE_STRIP, 12, 4 );
  }

  if(state.thruster_on && state.turning == _TURN_RIGHT){
    glBindTexture( GL_TEXTURE_2D, GLvars.right_t_texture  );
    glDrawArrays( GL_TRIANGLE_STRIP, 12, 4 );
  }
  
  glBindVertexArray(0);
  glUseProgram(0);
  
}
