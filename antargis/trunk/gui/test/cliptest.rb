#!/usr/bin/env ruby

require 'antargisgui.rb'

class MyWidget<AGWidget
	def initialize(p,r)
		super
		@f=AGSurface.load("flag.png")
		@t=AGTexture.new(@f)
		@x=0
		@y=0
		@time=0
	end
	def draw(p)
		#p.fillRect(AGRect2.new(0,0,128,128),AGColor.new(0xFF,0xFF,0xFF))
		p.pushMatrix
		p.clip(AGRect2.new(@x,@y,32,32))
		p.blit(@t,AGRect2.new(0,0,64,64))
		p.popMatrix
	end
	def proceed(t)
		@time+=t
		if @time>0.4
			@time-=0.4
			if @y==1
				@y=0
				@x=0
			else	
				@y=1
				@x=1
			end
		end
		queryRedraw
	end
end

class MyApp<AGApplication
	def initialize
		super
		@w=MyWidget.new(nil,AGRect2.new(10,10,128,128))
		setMainWidget(@w)
	end
	def eventFrame(t)
		@w.proceed(t)
	end
end

main=AGMain.new(640,480,32,false,false)

app=MyApp.new
app.run