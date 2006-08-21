#!/usr/bin/env ruby

require 'antargisgui.rb'

class MyWidget<AGWidget
	def initialize(p,r)
		super
		@f=AGSurface.load("flag2.jpg")
		@t=AGTexture.new(@f)
		@x=0
		@y=0
		@time=0
	end
	def draw(p)
		puts "TILE1:"
		p.tile(@t,getRect.origin)
		#p.fillRect(getRect.origin,AGColor.new(0xFF,0,0))
		p.pushMatrix
		puts @x,@y
		p.clip(AGRect2.new(@x,@y,64,64))
		p.fillRect(getRect.origin,AGColor.new(0,0,0))
		getScreen.flip
		$app.delay(100)
		puts "TILE2:"
		p.tile(@t,getRect.origin)
		getScreen.flip
		$app.delay(400)

		p.popMatrix

		return
		#p.fillRect(AGRect2.new(0,0,128,128),AGColor.new(0xFF,0xFF,0xFF))
		p.pushMatrix
		p.clip(AGRect2.new(@x,@y,32,32))
		p.blit(@t,AGRect2.new(0,0,64,64))
		p.popMatrix
	end
	def proceed(t)
		@time+=t
		d=0.4
		if @time>d
			@time-=d
			@x+=1
			if @x==32
				@x=0
				@y+=1
				if @y==32
					@y=0
				end
			end
			@x=15
		end
		queryRedraw
	end
end

class MyApp<AGApplication
	def initialize
		super
		@w=MyWidget.new(nil,AGRect2.new(0,0,128,128))
		setMainWidget(@w)
		$app=self
	end
	def eventFrame(t)
		@w.proceed(t)
	end
end

main=AGMain.new(640,480,32,false,false)

app=MyApp.new
app.run