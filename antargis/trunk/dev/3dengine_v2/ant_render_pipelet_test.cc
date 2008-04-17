#include "ant_render_pipelet_test.h"
#include "ant_gl.h"

AntRenderPipeletTest::AntRenderPipeletTest(AntRenderTarget *pRenderTarget):
  AntRenderPipelet(pRenderTarget)
        {

        }


void AntRenderPipeletTest::render()
  {

    if(false)
      {
        /* Enable smooth shading */
        glShadeModel( GL_SMOOTH );

        /* Set the background black */
        glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

        /* Depth buffer setup */
        glClearDepth( 1.0f );

        /* Enables Depth Testing */
        glEnable( GL_DEPTH_TEST );

        /* The Type Of Depth Test To Do */
        glDepthFunc( GL_LEQUAL );

        /* Really Nice Perspective Calculations */
        glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
      }

    int width=800;
    int height=600;
    float ratio=float(width)/height;

    /* Setup our viewport. */
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.1f, 100.0f );

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity( );



    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear The Screen And The Depth Buffer
    glLoadIdentity();                                               // Reset The View
    glTranslatef(-1.5f,0.0f,-6.0f);                                 // Move Left 1.5 Units And Into The Screen 6.0
    glColor3f(1,0,0);
    glBegin(GL_TRIANGLES);                                              // Drawing Using Triangles

    glVertex3f( 0.0f, 1.0f, 0.0f);                          // Top
    glVertex3f(-1.0f,-1.0f, 0.0f);                          // Bottom Left
    glVertex3f( 1.0f,-1.0f, 0.0f);                          // Bottom Right
    glEnd();        
  }

