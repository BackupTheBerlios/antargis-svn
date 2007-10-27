#!/usr/bin/env ruby

require 'ruby/antargislib.rb'
require 'ant_local.rb'

class Black<AGWidget
	def initialize(p,r)
		super(p,r)
		@r=r
		@alpha=0.0
	end
	def setAlpha(a)
		@alpha=a
		queryRedraw
	end
	def draw(painter)
		#painter.fillRect(@r,AGColor.new(0,0,0,0x1F))
		painter.fillRect(@r,AGColor.new(0,0,0,(0xFF*@alpha).to_i))
	end
end

class IntroApp<AntApplication
	include AGHandler
	def initialize
		super
		
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
		@black=Black.new(@img,AGRect.new(0,0,getScreen.getWidth,getScreen.getHeight))
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
	
	def eventKeyDown(e)
		cancel
		return super
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
	
	def eventFrame(t)
		@time+=t
		checkPhase
		delay(10)
		return true
	end

	def eventMouseButtonDown(e)
		tryQuit
		return super(e)
	end
	
end

app=IntroApp.new


if getConfig.get("intro")!="false"
  app.run
end
require 'ruby/mainmenu.rb'
