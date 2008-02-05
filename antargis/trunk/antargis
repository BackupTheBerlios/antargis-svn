#!/usr/bin/env ruby

# run intro - mainmenu will be called automatically

Dir.chdir(File.split(__FILE__)[0])

require 'ruby/intro.rb'
require 'ruby/mainmenu.rb'



# code for starting a level directly from command-line like this:
# ./ruby/run_game.rb levels/birth1
# or
# ./ruby/run_game.rb savegames/savegame0
$useMenu||=false
if true
  savegame=""
  ARGV.each{|arg|
    if arg=~/levels.*/ or arg=~/savegames.*/
      savegame=arg+".antlvl"
    end
  }
  if savegame!=""
    startGame(savegame) 
    exit
  end
end

if getConfig.get("intro")!="false"
  app=IntroApp.new
  app.run
end



getConfig.set("ok","ok")



# run menu
app=AntMenuApp.new
app.run


MyLocalizer.finalize