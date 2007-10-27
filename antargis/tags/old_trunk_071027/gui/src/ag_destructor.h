#ifndef AG_DESTRUCTOR
#define AG_DESTRUCTOR

#include <string>
#include <set>

/** These two classes provide some functionality for getting an event
    when an object is destroyed by ruby's GC.

    For this you can give an object (derived from *Sender) a name. You have
    a central Listening-class, derived from AGDestructionHandler.
    And each time an AGDestructionSender-object is killed, AGDestructionHandler::destroyed(.) is called.
    You can override this one. :-)

    Hope this make things clear.

    Here's a simple ruby-example:
    
    class Listener<AGDestructionHandler
      def destroyed(sender,name)
        puts name
      end
    end
    b=Listener.new
    a=AGDestructionSender.new(b,"sth")
    a=nil
    GC.start
    # "sth" is printed
    
*/

class AGDestructionSender;

class AGDestructionHandler
{
 public:
  virtual ~AGDestructionHandler();
  virtual void destroyed(AGDestructionSender *s, const std::string &pName);
 private:
  
  void addSender(AGDestructionSender*s);
  void subSender(AGDestructionSender*s);
  
  std::set<AGDestructionSender*> mSenders;

  friend class AGDestructionSender;
};

class AGDestructionSender
{
 public:
  AGDestructionSender(AGDestructionHandler *pHandler,const std::string &pName="");
  virtual ~AGDestructionSender();

 private:

  void resetHandler();

  std::string mName;
  AGDestructionHandler *mHandler;

  friend class AGDestructionHandler;
};


#endif
