//
//  loadTexture.h
//  asteroids
//
//  Created by Brian Summa on 7/21/22.
//

#ifndef loadTexture_h
#define loadTexture_h

class OpenGLHelpers{
  
public:
  /* -------------------------------------------------------------------------- */
  static void loadImageTexture(const char* filename, GLuint textureID, GLuint GLtex){
    
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, filename);
    
    std::cout << width << " X " << height << " image loaded\n";
    
    GLint GL_format = GL_RGBA;
    
    glActiveTexture( GLtex );
    glBindTexture( GL_TEXTURE_2D, textureID );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, &image[0]);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  }
};


#endif /* loadTexture_h */
