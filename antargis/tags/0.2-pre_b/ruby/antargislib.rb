#
# antargislib.rb does the following:
# - load libantargis.so (the c++-lib for anything that's not ruby)
# - init video-mode
# - process config-file and parameter input
#


module MyAntargisLib
	@@antargislibinited||=false

	if not @@antargislibinited
		puts "MYTRY"
		# try suspending arts
		if File.exists?("/usr/bin/artsshell")
			File.popen("/usr/bin/artsshell suspend 2>&1").close
		end
		
		@@programDir=Dir.pwd+"/ruby"
		# add programdir to path
		$:.push(@@programDir)
	end
end

if $demoMode.nil?
	$demoMode=false
	$enableLogging=true
end

module AntMyEventHandler
	def getNewEvent
		if $demoMode
			if hardwareCursor
				setCursor(getTextureCache.get("blue_cursor.png"))
			end
			@@eventDebugging||=File.open("events.txt","r")
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
						@@eventDebugging||=File.open("events.txt","w")
						@@eventDebugging.puts s
						#puts "NEWEVENT: #{self}  #{s}"
					end
				end
			end
# 			if e.nil?
# 				return toSDLEvent("")
# 			end
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


if File.exists?("libantargis.so")
	require 'libantargis'
elsif File.exists?("../src/.libs/libantargis.so")
	require '../src/.libs/libantargis'
else
	puts "Please run 'rake' before starting this program!"
	require 'libantargis'
end
include Libantargis
	
require 'gui/data/ag_tools.rb'
require 'ant_tools'
require 'ant_debug.rb'
		
module MyAntargislib
	@@antargislibinited||=false
	if not @@antargislibinited
		@@antargislibinited=true
		@@fullscreen=false
		@@antProfiling=false

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
					when "demo"
						$demoMode=true
					when "gui-test"
						require 'ruby/tests/gui_tests.rb'
						@@cursorEnabled=true
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
	"
	
						exit
				end
			end
		}
	
	
		@@main=AGMain.new
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
		
		puts xres,yres,@@fullscreen
		#raise #1

		@@noVideo||=nil	
		if @@noVideo.nil?
			#if xres!=1024 || yres!=768 || $fullscreen
			dputs @@fullscreen
			@@main.initVideo(xres,yres,32,@@fullscreen,true,1024,768)
			#end
		
			getConfig.set("xRes",xres.to_s)
			getConfig.set("yRes",yres.to_s)
			getConfig.set("fullscreen",@@fullscreen.to_s)
		
			@@main.setCaption("Antargis")
			@@main.setIcon("data/gui/editor/tower1.png")
		end
	end
	def MyAntargislib.startGC
		if not @@antProfiling
			puts "starting GC"
			GC.enable
			GC.start
			#GC.disable
			puts "GC ok"
		else
			puts "starting GC"
			GC.enable
			GC.start
			#GC.disable
			puts "GC ok"
		end
	end

	def MyAntargislib.demoMode
		$demoMove
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
		puts range,range.class
		super
	end
end

class AntApplication<AGApplication
	include AntMyEventHandler

# # Here you can bypass some functions - if you might want to -
# 	def delay(ms)
# 		puts "DELAY  #{ms}"
# 		return 
# 		if not demoMode
# 			super
# 		end
# 	end
end

# def rand
# 	raise "This Function shouldn't be called at all!!!!!"
# end