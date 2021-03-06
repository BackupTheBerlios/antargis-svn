#!/usr/bin/env ruby

require 'ruby/antargislib.rb'


class Black<AGWidget
	def initialize(p,r)
		super
		@r=r
		@alpha=0.0
	end
	def setAlpha(a)
		@alpha=a
	end
	def draw(painter)
		painter.drawRect(@r,AGColor.new(0,0,0,(0xFF*@alpha).to_i))
	end
end

class IntroApp<AGApplication
	include AGHandler
	def initialize
		super
		
		@story=[["data/gui/map1.png","blabla"]]
		
		@surfaces=@story.collect{|x|getSurfaceManager.loadSurface(x[0])}
		@s=0
		
		@button=AGButton.new(nil,AGRect.new(0,0,640,480),"muh")
		@button.setTheme("story")
		@img=AGImage.new(nil,AGPoint.new(0,0),@surfaces[0],false,AGRect.new(0,0,1024,768))
		@black=Black.new(@img,AGRect.new(0,0,1024,768))
		@img.addChild(@black)
		@img.addChild(@button)
		setMainWidget(@img)
		@time=0
		
		@blendin=0.2
		@blendout=0.6
		@show=1.2
		@hide=0.3
		@phase=0
		@phases=[2,2,2,1]
		#getSoundManager.playMp3("data/music/ant2.ogg")
		
		addHandler(getSoundManager,:sigMp3Finished,:musicEnd)
	end
	
	def eventKeyDown(e)
		super
		cancel
	end
	
	def cancel
		@canceled=true
		@phases=[0.2,0.2,2,10]
	end	
	def musicEnd
		cancel
	end
	
	def tryQuit
		if @canceled
			#getSoundManager.stopMp3
			#getSoundManager.fadeOutMusic(1000)
			super
		else
			cancel
		end
	end
	
	def checkPhase
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
				@img.setSurface(@surfaces[@s])
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
	end
	
	def eventFrame(t)
		@time+=t
		checkPhase
		delay(10)
	end
	
end

app=IntroApp.new
app.run
require 'ruby/mainmenu.rb'
