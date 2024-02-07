//////////////////////////////////////////////////////////////////////////////
//
//	  Functions modified Angel 6th Edition
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __ANGEL_H__
#define __ANGEL_H__

//----------------------------------------------------------------------------
// 
// --- Include system headers ---
//

#include <iostream>
#include <cstdio>
#include <stdlib.h>

#define BUFFER_OFFSET( offset )   ((GLvoid*) (offset))

#ifdef _WIN32
#include "u8names.h"
#endif //_WIN32

static char*
readShaderSource(const char* shaderFile)
{
#ifdef _WIN32
  std::wstring wcfn;
  if (u8names_towc(shaderFile, wcfn) != 0)
    return NULL;
  FILE* fp = _wfopen(wcfn.c_str(), L"rb");
#else
  FILE* fp = fopen(shaderFile, "rb");
#endif //_WIN32
  
  if ( fp == NULL ) { return NULL; }
  
  fseek(fp, 0L, SEEK_END);
  long size = ftell(fp);
  
  fseek(fp, 0L, SEEK_SET);
  char* buf = new char[size + 1];
  fread(buf, 1, size, fp);
  
  buf[size] = '\0';
  fclose(fp);
  
  return buf;
}

static void check_shader_compilation(std::string shader_file, GLuint shader){
  GLint  compiled;
  glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
  if ( !compiled ) {
    std::cerr << shader_file.c_str() << " failed to compile:" << std::endl;
    GLint  logSize;
    glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
    char* logMsg = new char[logSize];
    glGetShaderInfoLog( shader, logSize, NULL, logMsg );
    std::cerr << logMsg << std::endl;
    delete [] logMsg;
    
    exit( EXIT_FAILURE );
  }
}

static void check_program_link(GLuint program){
  
  GLint  linked;
  glGetProgramiv( program, GL_LINK_STATUS, &linked );
  if ( !linked ) {
    std::cerr << "Shader program failed to link" << std::endl;
    GLint  logSize;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
    char* logMsg = new char[logSize];
    glGetProgramInfoLog( program, logSize, NULL, logMsg );
    std::cerr << logMsg << std::endl;
    delete [] logMsg;
    
    exit( EXIT_FAILURE );
  }
  
}


#endif // __ANGEL_H__
