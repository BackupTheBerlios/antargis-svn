#ifndef __CG_H
#define __CG_H

#ifdef USE_CG

#include <cg.h>  
#include <cgGL.h> 

#include <string>
#include <ag_debug.h>

class CgProgram
{
 public:
  CgProgram(CGprogram p,CGprofile prof):
    cgProgram(p),cgVertexProfile(prof)
    {
      // Get Handles To Each Of Our Parameters So That
      // We Can Change Them At Will Within Our Code
      position        = cgGetNamedParameter(cgProgram, "IN.position");
      color           = cgGetNamedParameter(cgProgram, "IN.color");
      wave            = cgGetNamedParameter(cgProgram, "IN.wave");
      modelViewMatrix = cgGetNamedParameter(cgProgram, "ModelViewProj");
    }
  void bind()
  {
    cgGLSetStateMatrixParameter(modelViewMatrix, CG_GL_MODELVIEW_PROJECTION_MATRIX, CG_GL_MATRIX_IDENTITY);

    
    cgGLEnableProfile(cgVertexProfile);                                     // Enable Our Vertex Shader Profile
    
    // Bind Our Vertex Program To The Current State
    cgGLBindProgram(cgProgram);
    
    
    // Set The Drawing Color To Light Green (Can Be Changed By Shader, Etc...)
    cgGLSetParameter4f(color, 0.5f, 1.0f, 0.5f, 1.0f);
    

  }
  
  CGprogram getID() const
  {
    return cgProgram;
  }

 private:
  CGprogram cgProgram;
  CGprofile cgVertexProfile;

  CGparameter     position, color, modelViewMatrix, wave;
};

class Cg
{
 public:
  Cg()
    {
      // Setup Cg
      cgContext = cgCreateContext();
      
      // Validate Our Context Generation Was Successful
      if (cgContext == NULL)
        {
	  cdebug("ERROR creating cgContext");
	  return;
        }
      cgVertexProfile = cgGLGetLatestProfile(CG_GL_VERTEX);                           // Get The Latest GL Vertex Profile
      
      // Validate Our Profile Determination Was Successful
      if (cgVertexProfile == CG_PROFILE_UNKNOWN)
        {
	  cdebug("Invalid profile type");
	  return;                                                           // We Cannot Continue
        }
      cgGLSetOptimalOptions(cgVertexProfile);  
    }
  ~Cg()
    {
       cgDestroyContext(cgContext);    
    }
  

  CgProgram *loadProgram(const std::string &pFile="CG/Wave.cg")
    {
      // Load And Compile The Vertex Shader From File
      CGprogram cgProgram = cgCreateProgramFromFile(cgContext, CG_SOURCE, pFile.c_str(), cgVertexProfile, "main", 0);
      
      // Validate Success
      if (cgProgram == NULL)
        {
	  // We Need To Determine What Went Wrong
	  CGerror Error = cgGetError();
	  
	  // Show A Message Box Explaining What Went Wrong
	  cdebug("ERROR:"<<cgGetErrorString(Error));
	  return 0;
        }
      // Load The Program
      cgGLLoadProgram(cgProgram);


      return new CgProgram(cgProgram,cgVertexProfile);
    }

 private:
  CGcontext cgContext;
  CGprofile cgVertexProfile;
};

Cg *getCG();

#endif // USE_CG
#endif
