//
//  Game.cpp
//  Asteroids
//
//  Created by Brian Summa on 6/5/15.
//
//

#include "Game.h"

bool Game::testIntersections(){
    vec2 ship_bbox_min, ship_bbox_max;
    if(ship->state.turning == Ship::_NO_TURN){
        ship_bbox_min = ship->ship_bbox[0];
        ship_bbox_max = ship->ship_bbox[1];
    } else if(ship->state.turning == Ship::_TURN_LEFT){
        ship_bbox_min = ship->left_bbox[0];
        ship_bbox_max = ship->left_bbox[1];
    } else if(ship->state.turning == Ship::_TURN_RIGHT){
        ship_bbox_min = ship->right_bbox[0];
        ship_bbox_max = ship->right_bbox[1];
    }
    
    ship_bbox_min += ship->state.cur_location;
    ship_bbox_max += ship->state.cur_location;

    for(unsigned int i=0; i < asteroids.size(); i++){
        vec2 asteroid_bbox_min = asteroids[i]->asteroid_bbox[0] + asteroids[i]->state.cur_location;
        vec2 asteroid_bbox_max = asteroids[i]->asteroid_bbox[1] + asteroids[i]->state.cur_location;

        if(!(ship_bbox_max.x < asteroid_bbox_min.x ||
             ship_bbox_min.x > asteroid_bbox_max.x ||
             ship_bbox_max.y < asteroid_bbox_min.y ||
             ship_bbox_min.y > asteroid_bbox_max.y)){
            
            game_over = true;
            return true;
        }
    }

    // No collision detected
    return false;
}



Game::Game(){
  ship = new Ship();
  asteroids.resize(2);
  asteroids[0] = new Asteroid(1);
  asteroids[1] = new Asteroid(2);
  game_over = false;
  
  std::string file_location = source_path + "sprites/game_over.png";
  unsigned error = lodepng::decode(game_over_im, go_width, go_height, file_location.c_str());
  std::cout << go_width << " X " << go_height << " game image loaded\n";
  
};


void Game::gl_init(){
  
  std::vector <vec2> pos(4);
  std::vector <vec2> uv(4);
  
  pos[0] = vec2(screen_extents[0],  screen_extents[3]);
  pos[1] = vec2(screen_extents[0],  screen_extents[2]);
  pos[2] = vec2(screen_extents[1],  screen_extents[3]);
  pos[3] = vec2(screen_extents[1],  screen_extents[2]);
  
  uv[0] = vec2(0.0,0.0);
  uv[1] = vec2(0.0,1.0);
  uv[2] = vec2(1.0,0.0);
  uv[3] = vec2(1.0,1.0);
  

  unsigned int vert_size = pos.size()*sizeof(vec2);
  unsigned int uv_size = uv.size()*sizeof(vec2);

  std::string vshader = source_path + "shaders/vshader_Texture.glsl";
  std::string fshader = source_path + "shaders/fshader_Texture.glsl";

  GLchar* vertex_shader_source = readShaderSource(vshader.c_str());
  GLchar* fragment_shader_source = readShaderSource(fshader.c_str());

  GOGLvars.vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(GOGLvars.vertex_shader, 1, (const GLchar**) &vertex_shader_source, NULL);
  glCompileShader(GOGLvars.vertex_shader);
  check_shader_compilation(vshader, GOGLvars.vertex_shader);

  GOGLvars.fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(GOGLvars.fragment_shader, 1, (const GLchar**) &fragment_shader_source, NULL);
  glCompileShader(GOGLvars.fragment_shader);
  check_shader_compilation(fshader, GOGLvars.fragment_shader);

  GOGLvars.program = glCreateProgram();
  glAttachShader(GOGLvars.program, GOGLvars.vertex_shader);
  glAttachShader(GOGLvars.program, GOGLvars.fragment_shader);

  glLinkProgram(GOGLvars.program);
  check_program_link(GOGLvars.program);
  glUseProgram(GOGLvars.program);
  
  glBindFragDataLocation(GOGLvars.program, 0, "fragColor");

  GOGLvars.vpos_location   = glGetAttribLocation(GOGLvars.program, "vPos");
  GOGLvars.vtex_location = glGetAttribLocation(GOGLvars.program, "vTexCoord" );
  GOGLvars.M_location = glGetUniformLocation(GOGLvars.program, "M" );
  
  glGenTextures( 1, &GOGLvars.texture );

  glBindTexture( GL_TEXTURE_2D, GOGLvars.texture );
  glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, go_width, go_height,
               0, GL_RGBA, GL_UNSIGNED_BYTE, &game_over_im[0]);
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );


  // Create a vertex array object
  glGenVertexArrays( 1, &GOGLvars.vao );
  //Set GL state to use vertex array object
  glBindVertexArray( GOGLvars.vao );

  //Generate buffer to hold our vertex data
  glGenBuffers( 1, &GOGLvars.buffer );
  //Set GL state to use this buffer
  glBindBuffer( GL_ARRAY_BUFFER, GOGLvars.buffer );

  //Create GPU buffer to hold vertices and color
  glBufferData( GL_ARRAY_BUFFER, vert_size + uv_size, NULL, GL_STATIC_DRAW );
  //First part of array holds vertices
  glBufferSubData( GL_ARRAY_BUFFER, 0, vert_size, &pos[0] );
  //Second part of array hold colors (offset by sizeof(triangle))
  glBufferSubData( GL_ARRAY_BUFFER, vert_size, uv_size, &uv[0] );

  glEnableVertexAttribArray(GOGLvars.vpos_location);
  glEnableVertexAttribArray(GOGLvars.vtex_location );

  glVertexAttribPointer( GOGLvars.vpos_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
  glVertexAttribPointer( GOGLvars.vtex_location, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vert_size) );

  glBindVertexArray(0);
  
}

void Game::draw_game_over(mat4 proj){
  
  glUseProgram(GOGLvars.program);
  glBindVertexArray( GOGLvars.vao );
  glBindBuffer( GL_ARRAY_BUFFER, GOGLvars.buffer );
  
  glUniformMatrix4fv( GOGLvars.M_location, 1, GL_TRUE, proj);
  
  glBindTexture( GL_TEXTURE_2D, GOGLvars.texture  );
  glDrawArrays( GL_TRIANGLE_STRIP, 0, 4 );
  
  glBindVertexArray(0);
  glBindBuffer( GL_ARRAY_BUFFER, 0);

  
}
