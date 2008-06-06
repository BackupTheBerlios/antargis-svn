#
# antargislib.rb does the following:
# - load libantargis.so (the c++-lib for anything that's not ruby)
# - init video-mode
# - process config-file and parameter input
#
require 'pp'


module MyAntargisLib
  @@antargislibinited||=false


   if not @@antargislibinited
    @@programDir=File.expand_path(File.split(__FILE__)[0])
    $:.push(@@programDir)
    #$:.push(File.join(@@programDir,"entities"))
     # try suspending arts
    if File.exists?("/usr/bin/artsshell")
      unless ENV["PATH"] =~ /^[A-Za-z]:\\.*/
        File.popen("/usr/bin/artsshell suspend 2>&1").close
      end
    end
   end
end

if $demoMode.nil?
  $demoMode=false
  $enableLogging=true
end

module AntMyEventHandler
  def getNewEvent
    filename=File.join(getWriteDir,"events.txt")
    if $demoMode
      if hardwareCursor
        setCursor(getTextureCache.get("blue_cursor.png"))
      end

      @@eventDebugging||=File.open(filename,"r")
      @@nextLine||=@@eventDebugging.readline
      puts "NEXTLINE:#{@@nextLine}"
      if @@nextLine=~/T:.*/
        time=@@nextLine.scan(/..(.*)/)[0][0].to_f
        puts "TIME:#{time}"
        @@nextLine=nil
        setDemoTime(time)
        
        return toSDLEvent("")
      else
        s=@@nextLine
        @@nextLine=nil
        return toSDLEvent(s)
      end
    else
      e=super
      if not e.nil?
        if eventOk(e)
          s=toString(e)
          if $enableLogging
            @@eventDebugging||=File.open(filename,"w")
            @@eventDebugging.puts s
          end
        end
      end
      return e
    end
  end


  def eventPrepareFrame(t)
    if $enableLogging and not $demoMode
      @@eventDebugging||=File.open("events.txt","w")
      @@eventDebugging.puts "T: #{t}"
      #puts "TIME #{t}"
    end
    return false
  end

end

begin
  if $antargisStarterLoaded!=true
    puts "Please run wih ./starter antargis" 
    exit
  end
rescue
  puts "Please run wih ./starter antargis"
  exit
end


begin
  include Antargis
  puts "Antargis-module included"
rescue
  puts "Antargis-module not found - trying to load .so"
  begin
    if File.exists?("ext/antargis.so")
      require 'ext/antargis'
    elsif File.exists?("antargis.bundle")
      require 'antargis.bundle'
    else
      puts "Please run 'rake' before starting this program!"
      require 'antargis'
    end
    include Antargis
  rescue
    puts "I'll try to run rake for you. Please try again yourself when it doesn't work."
    #system "rake"
    puts "Try starting again..."
    if File.exists?("ext/antargis.so")
      require 'ext/antargis'
    else
      puts "Please run 'rake' before starting this program!"
      require 'antargis'
    end
    include Antargis
    puts "I'll try to run rake for you. Please try again yourself when it doesn't work."
  
  end
end
puts "extension loaded"

  
require 'ruby/gui/ag_tools.rb'
require 'ant_tools'
require 'ant_debug.rb'


# add path
@basePath=File.split(File.split(__FILE__)[0])[0]
addPath(@basePath)
addPath(File.join(@basePath,"data"))
addPath(File.join(@basePath,"data","fonts"))

    
module MyAntargislib
  @@antargislibinited||=false
  if not @@antargislibinited
    @@antargislibinited=true
    @@fullscreen=false
    @@antProfiling=false
    @@opengl=true

    setDebugLevel(1000)
  
    # check options
    ARGV.each{|arg|
      if arg=~/--.*/ or arg=~/-.*/
        a=arg[1..1000]
        if arg[1..1]=="-"
          a=arg[2..1000]
        end
        case a
          when "no-sound"
            getConfig.set("soundEnabled","false")
          when "sound"
            getConfig.set("soundEnabled","true")
          when "profile"
            require 'profile'
            @@antProfiling=true
          when "gc-check"
            GC.stress=true
          when "intro"
            getConfig.set("intro","true")
          when "no-intro"
            getConfig.set("intro","false")
          when "fullscreen"
            @@fullscreen=true
          when "window"
            @@fullscreen=false
          when /debug-level/
            level=a.split("=")[1].to_i
            setDebugLevel(level)
          when "nogl"
            @@opengl=false
          when "demo"
            $demoMode=true
          when "gui-test"
            require 'ruby/tests/gui_tests.rb'
            @@cursorEnabled=true
          when "ruby-raise"
            setRubyRaising(true)
          when "no-3d-textures"
            setUsing3dTextures(false)
          when "help","h"
            STDERR.puts "Possible options:
  --help         show this help message
  
  --fullscreen   enable fullscreen
  --window       start in windows mode
  
  --profile      enables profiling output of BoA's ruby-part
  --gc-check     enables debug mode for gc-check (GC.stress=true)
  
  --intro        enables the intro
  --no-intro     disables the intro
  
  --sound        enables sound for now and future uses
  --no-sound     disables sound for now and future uses
  
  --gui-test     tells BoA to use events.txt to produce GUI-events

  --demo         enable demo mode - reading events from events.txt

  --debug-level=x

  --nogl         disable GL-mode (3d-acceleration) - THIS IS NOT YET FULLY SUPPORTED!!
  --ruby-raise   raise exceptions as ruby-exceptions

  --no-3d-textures disables 3d-textures - they're bad on some graphics chipsets (esp. Intel)
  "
  
            exit
        end
      end
    }
  
  
    @@main=getMain #AGMain.new
    xres=1024
    if getConfig.get("xRes")!=""
      xres=getConfig.get("xRes").to_i
    end
    yres=768
    if getConfig.get("yRes")!=""
      yres=getConfig.get("yRes").to_i
    end
  
  
  
    if not @@fullscreen
      if getConfig.get("fullscreen")=="true"
        @@fullscreen=true
      end
    end
    

    @@noVideo||=nil  
    if @@noVideo.nil?
      puts "initVideo..."
      getVideo.initVideo(xres,yres,32,@@fullscreen,@@opengl,1024,768)
      puts "initVideo ok"
    
      getConfig.set("xRes",xres.to_s)
      getConfig.set("yRes",yres.to_s)
      getConfig.set("fullscreen",@@fullscreen.to_s)
    
      getVideo.setCaption("Antargis")
      getVideo.setIcon("data/gui/editor/tower1.png")
    end
  end
  def MyAntargislib.startGC
    GC.enable
    GC.start
  end

  def MyAntargislib.demoMode
    $demoMove
  end
  def MyAntargislib.opengl
    @@opengl
  end
end

def demoMode
  MyAntargislib.demoMode
end

#include Libantargis
def startGC
  MyAntargislib.startGC
end

class AGStringUtf8
  alias  :oldIndex :[]
  def [](range)
    if range.is_a?(Range)
      ma=range.max
      mi=range.min
      if self.length>0
        ma%=self.length
        mi%=self.length
      end
      
      return self.substr(mi,ma-mi+1)
    end
    #puts range,range.class
    super
  end
end

class Logger
  filename=File.join(getWriteDir,"log.txt")
  @@log=File.open(filename,"w")
  def self.log(*s)
    @@log.print("[")
    @@log.print(Time.new)
    @@log.print("] ")
    @@log.puts(*s)
  end
end

def log(*s)
  Logger.log(*s)
end

class AntApplication<AGApplication
  include AntMyEventHandler

# # Here you can bypass some functions - if you might want to -
#   def delay(ms)
#     puts "DELAY  #{ms}"
#     return 
#     if not demoMode
#       super
#     end
#   end
end



