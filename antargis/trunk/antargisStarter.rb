#!/usr/bin/env ruby

# run intro - mainmenu will be called automatically

def puts(*s)
  $debugFile||=File.open("antargisSt.log","w")
  $debugFile.puts(*s)
  $debugFile.flush
end
puts "MY LOG"
puts "MY LOG2"

Dir.chdir(File.split(__FILE__)[0])
puts "MY LOG2"
puts Dir.pwd
puts $:
puts "--"
require File.join('ruby','intro.rb')
puts "MY LOG2"
require 'ruby/mainmenu.rb'
puts "MY LOG2"



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