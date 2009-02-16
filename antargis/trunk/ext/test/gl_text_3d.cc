#include <ag_gl.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <stdexcept>
#include <iostream>
#include <sstream> 


void assertGL(std::string s)
  {
      {
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
          std::ostringstream msg;
          msg <<s<< ": OpenGLError "
          << gluErrorString(error);

          std::cerr<<msg.str()<<std::endl;
          throw std::runtime_error(msg.str());
        }
      }
  }

int main(int argc,char *argv[])
  {
    SDL_Init(SDL_INIT_VIDEO);

    int w=1024;
    int h=768;

    SDL_SetVideoMode(w,h,32,SDL_OPENGL);

    // load file
    //SDL_Surface *surface=IMG_Load(IMAGE_FILE);

    GLuint tex;
    // generate texture
    glGenTextures(1,&tex);
    glBindTexture(GL_TEXTURE_3D,tex);
    glEnable(GL_TEXTURE_3D);

    size_t redSize=128*128*4096*4;
    char *red=new char[redSize];
    for(size_t i=0;i<redSize;i++)
      red[i]=(((i%4)==0 || (i%4)==3)?255:0);
    char *blue=new char[redSize];
    for(size_t i=0;i<redSize;i++)
      blue[i]=(((i%4)==2 || (i%4)==3)?255:0);


    glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA, 128, 128, 4096/128, 0, GL_RGBA,
        GL_UNSIGNED_BYTE,red);

    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    assertGL("");
    glPixelStorei(GL_UNPACK_ROW_LENGTH,
        128); //blue->pitch / blue->format->BytesPerPixel);


    glTexSubImage3D(GL_TEXTURE_3D,0,0,0,0,
        128,128,32,GL_RGBA,GL_UNSIGNED_BYTE,blue);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_3D,tex);
    glEnable(GL_TEXTURE_3D);

    glDisable(GL_TEXTURE_2D);

    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    GLfloat ratio;

    ratio = ( float )w / ( float )h;

    gluOrtho2D(0,w,0,h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor4f(1,1,1,1);
    glBegin(GL_QUADS);

    for(int i=0;i<32;i++)
      {
        float x=int(i%6);
        float y=int(i/6);

        float d=float(i)/32.0;

        x*=110;
        y*=110;

        float w=100;
        float x2=x+w;
        float y2=y+w;



        glTexCoord3f(0,0,d);
        glVertex2f(x,y);

        glTexCoord3f(1,0,d);
        glVertex2f(x2,y);

        glTexCoord3f(1,1,d);
        glVertex2f(x2,y2);

        glTexCoord3f(0,1,d);
        glVertex2f(x,y2);

      }

    glEnd();

    SDL_GL_SwapBuffers();




    assertGL("");

    SDL_Delay(10000);

    return 0;
  }
