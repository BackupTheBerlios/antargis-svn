#!/usr/bin/env ruby

require 'ruby/antargislib.rb'
require 'ant_local.rb'

# *BlackFade* fades the screen to black. This Widget is laid over other widgets and
# draws a black rectangle with varying alpha-channel. This way you
# can fade in and out pictures and text-displays.
class BlackFade<AGWidget
	# alpha-value is set to 0 by default
	def initialize(p,r)
		super(p,r)
		@r=r
		@alpha=0.0
	end
	# set the alpha-value *a* in range from 0.0 to 1.0
	# the default-value is 0
	def setAlpha(a)
		@alpha=a
		queryRedraw
	end

	# draws the black rectangle with set pre-set alpha-value
	def draw(painter)
		#painter.fillRect(@r,AGColor.new(0,0,0,0x1F))
		painter.fillRect(@r,AGColor.new(0,0,0,(0xFF*@alpha).to_i))
	end
end


# This application-class handles the display and event-handling of BoA's intro
# It's able to display images and text-elements (that are drawn using the intro.font-Font from the theme)
# Each image/text-element is faded-in and faded-out. While the intro runs some music-track is played
class IntroApp<AntApplication
	include AGHandler
	def initialize
		puts "introApp..."
		super
		puts "introApp!"
		
		@pics=["data/gui/rough_lands.png","data/gui/strong_heroes.png","data/gui/logoGold.png","data/gui/your_mission.png","data/gui/unify_the_people.png","data/gui/castle_small.png"]

		@pics=["rough lands","strong heroes","data/gui/logoGold.png","your mission","unify the people","data/gui/castle_small.png"]

		font=getTheme.getFont("intro.font")
		@surfaces=@pics.collect{|x|
			r=nil
			if x=~/\.png/ or x=~/\.jpg/
				r=AGSurface.load(x)
			else
				r=getFontEngine.renderText(0,0,_(x),font)
			end
			r
		}
		@s=0
		@img=AGImage.new(nil,AGRect.new(0,0,getScreen.getWidth,getScreen.getHeight),@surfaces[0],false)
		@black=BlackFade.new(@img,AGRect.new(0,0,getScreen.getWidth,getScreen.getHeight))
		@img.addChild(@black)
		setMainWidget(@img)
		@time=0
		
		@blendin=0.9
		@blendout=0.6
		@show=1.2
		@hide=0.3
		@phase=0
		@phases=[2,2,2,1]
		getSoundManager.playMp3("data/music/battle1.ogg")
		
		addHandler(getSoundManager,:sigMp3Finished,:musicEnd)
	end
	

	# if a key was pressed, we cancel the intro playing
	def eventKeyDown(e)
		cancel
		return super
	end
	
	# cancel the intro
	# resets the *@phases*
	def cancel
		@canceled=true
		@phases=[0.2,0.2,2,10]
	end	

	# if the music has finished, we cancel the intro
	def musicEnd
		cancel
	end
	

	# this function is called, when you press F10 to quit the application
	# the first call cancels the intro (thus it will still fade out)
	# the second call will quit the intro directly
	def tryQuit
		if @canceled
			#getSoundManager.stopMp3
			#getSoundManager.fadeOutMusic(1000)
			super
		else
			cancel
		end
	end

	# this function does:
	# * increase the phase - set alpha for BlackFade-Widget
	# * switch to newer textures/text-elements
	# * quit the intro, when its finished
	def checkPhase
		#puts "#{@time}>#{@phases[@phase]}"
		if @time>@phases[@phase]
			@time-=@phases[@phase]
			@phase+=1
			if @phase==4
				@phase=0
			elsif @phase==3 # switch surface early, so there won't be a timing problem
				@s=@s+1
				if @s==@surfaces.length
					@s=0
					@canceled=true
				end
				if @surfaces[@s].is_a?(AGSurface)
					@img.setSurface(@surfaces[@s])
				else
					@img.setTexture(@surfaces[@s])
				end
			end
		end
		case @phase
			when 0
				@black.setAlpha(1-@time/@phases[@phase])
			when 2
				@black.setAlpha(@time/@phases[@phase])
			when 3
				if @canceled
					tryQuit
				end
		end
		@img.redraw
	end
	
	# gets called in each frame (and sets a delay of 10ms between frames)
	def eventFrame(t)
		@time+=t
		checkPhase
		delay(10)
		return true
	end

	# when a mouse-button is clicked the intro is canceled
	def eventMouseButtonDown(e)
		tryQuit
		return super(e)
	end
	
end
# 
# app=IntroApp.new
# 
# 
# if getConfig.get("intro")!="false"
#   app.run
# end
# require 'ruby/mainmenu.rb'
