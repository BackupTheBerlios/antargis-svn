#!/usr/bin/ruby

require 'libantargisruby'
require 'sdl'

include Libantargisruby

class MyButton < AGButton
	def initialize(parent,rect,text)
		super(parent,rect,text)
	end
	def draw(r)
		super(r)
		font=AGFont.new("Arial.ttf")
		font.setColor(AGColor.new)
		#surface=loadAGSurface("test.png")
		AGFontEngine.renderText(getScreen,r.project(getRect),0,0,"Hupe",font)
		#surface.toScreen(150,150)
	end
end

main=AGMain.new

surface=getScreen().loadSurface("test.png")
#getScreen().blit(surface,AGRect.new(10,10,300,300))#.toScreen(10,10)

font=AGFont.new("Arial.ttf")

AGFontEngine.renderText(getScreen(),AGRect.new(0,0,100,100),30,30,"Text",font)

#surface.toScreen(30,30)

r=AGRect.new(0,0,50,20)
widget=MyButton.new(nil,r,"hallo")

#widget.draw(getScreen.getRect)

main.flip
SDL.delay(1000)
#
