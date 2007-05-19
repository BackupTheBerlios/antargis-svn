#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

getMain.initVideo(640,480,32,false,false)

setDebugLevel(0)

class MyWidget<AGWidget
	def initialize(p,r)
		super
	end
	def draw(p)
		c=AGColor.new(0xFF,0,0)#(rand*0xFF).to_i,(rand*0xFF).to_i,(rand*0xFF).to_i)
		p.fillRect(getRect.origin,c)
	end
end

class MyWidget2<AGWidget
	def initialize(p,r)
		super
		@x=@y=0
		eventFrame(0)
		queryRedraw
	end
	def draw(p)
		c=AGColor.new((rand*0xFF).to_i,(rand*0xFF).to_i,(rand*0xFF).to_i)
		p.fillRect(getRect.origin,c) #AGColor.new(0,0,0))
		c=AGColor.new(0,0xFF,0) #(rand*0xFF).to_i,(rand*0xFF).to_i,(rand*0xFF).to_i)
		p.fillRect(@r,c)
	end
	def eventFrame(t)
		if @inited
		else
			pushChangeRect(getRect.origin) #queryRedraw
			@inited=true
		end
		@x+=1
		@y+=3
		@x%=width
		@y%=height
		@r=AGRect2.new(@x,@y,10,10)
		pushChangeRect(@r.grow(3))
		#queryRedraw
		true
	end
end

class MyApp<AGApplication
	def initialize
		super
		@ws=[]
		@m=MyWidget2.new(nil,AGRect2.new(50,50,200,200))
		setMainWidget(@m)
		@ws << MyWidget.new(@m,AGRect2.new(5,5,110,70))
		@ws << AGButton.new(@m,AGRect2.new(10,10,100,50),AGStringUtf8.new("halo"))
		puts "adddd..."
		#@ws.each{|w|@m.addChild(w)}
		

	end
	def eventFrame(t)
		delay(10)
		@m.eventFrame(t)
		true
	end
end

a=MyApp.new
a.run