#include "ant_gl.h"
#include "ant_render_pipeline.h"
#include "ant_screen_render_target.h"
#include "ant_render_scene_pipelet.h"
#include "ant_render_mode.h"
#include "ant_simple_scene_graph.h"
#include "ant_perspective_camera.h" 
#include "ant_sprite.h"
#include "ant_static_mesh_data.h"
#include "ant_mesh_va_data.h"
#include "ant_mesh_direct_data.h"
#include "ant_lighting.h"
#include "ant_example_cube.h"
#include "ant_example_triangle.h"
#include "ant_render_pipelet_test.h"

#include <SDL.h>
#include <iostream>
#include <math.h>
#include "ag_glscreen.h"

int main(int argc,char *argv[])
  {
    AntGL gl;
    
    gl.initVideoMode(800,600);
    gl.clearBuffer();
    setScreen(new AGGLScreen(800,600,800,600));
    
    AntScreenRenderTarget screen(AGRect2(0,0,800,600));
    AntRenderPipeline pline;
    AntRenderMode mode;
    AntSimpleSceneGraph scene;
    
    AntStaticMeshData *treeMesh(AntStaticMeshData::load("../data/models/tower.ant2",1));
    AntMeshVAData tree(treeMesh);
    //AntMeshDirectData tree(treeMesh);
    AntTexture texture("../data/textures/models/tower.png");
    
    AntSprite sprite(AGVector3(0,0,0),AntQuaternion(AGVector3(0,0,1),-M_PI/4.0),&tree,&texture);
    scene.add(&sprite);
    
    /*
    AntExampleTriangle tri;
    scene.add(&tri);
    AntExampleCube cube;
      scene.add(&cube);
      */
    AntRigidTransform rigidTransform(AGVector3(0,-10,10),AGVector3(0,0,0),AGVector3(0,0,1));
    AntFrustrum frustrum(45.0,800.0/600.0,0.1,100);
    AntPerspectiveCamera camera(frustrum,rigidTransform);
    AntCameraView view(&scene,&camera);
    
    AntLighting lighting;
    
    AntRenderPipelet *pipelet=new AntRenderScenePipelet(&screen,&view,mode,&lighting); 
    
    pline.insert(pipelet);
    
    /*gl.clearBuffer();
    glColor4f(1,1,1,1);*/
    pline.render();
    
    gl.swapBuffers();
    SDL_Delay(1000);
    gl.swapBuffers();
    SDL_Delay(1000);
    gl.swapBuffers();
    SDL_Delay(1000);
    return 0;
  }

int main2(int argc,char *argv[])
  {
    AntGL gl;
    gl.initVideoMode(800,600);
    gl.clearBuffer();
    
    AntScreenRenderTarget screen(AGRect2(0,0,800,600));
    AntRenderPipeline pline;
    
    AntRenderPipelet *pipelet=new AntRenderPipeletTest(&screen); 
    
    pline.insert(pipelet);
    
    pline.render();
    
    gl.swapBuffers();
    SDL_Delay(1000);
    return 0;
  }
 
