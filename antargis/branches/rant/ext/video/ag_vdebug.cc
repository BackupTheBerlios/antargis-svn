#include <ag_vdebug.h>
#include <ag_video.h>

void agAssertGL(std::string s)
{
  if(glMode())
    {
      GLenum error = glGetError();
      if(error != GL_NO_ERROR) {
	std::ostringstream msg;
	msg <<s<< ": OpenGLError "
	    << gluErrorString(error);
	
	std::cerr<<msg.str()<<std::endl;
	throw std::runtime_error(msg.str());
	agRaise(msg.str());
      }
    }

  // asserting SDL_errors
  char *se=SDL_GetError();
  if(se)
    {
      if(strlen(se)>0)
	{
	  std::cerr<<"SDL_Error:"<<s<<":"<<se<<std::endl;
	  if(std::string(se).substr(0,37)=="Failed loading glXGetSwapIntervalMESA")
	    std::cerr<<"IGNORING THIS ERROR!"<<std::endl;
	  else
	    agRaise(se);
	  SDL_ClearError();
	}
    }
  else
    {
      std::cerr<<"MUH"<<std::endl;
    }

}


