#!/usr/bin/env ruby

require 'libantargisruby'
include Libantargisruby

main=AGMain.new

class MyWidget< AGWidget
	def initialize(p,r)
		super(p,r)
		@surface=getScreen().loadSurface("win_border.png")
	end
	def draw(r)
		getScreen().tile(@surface,AGRect.new(10,10,50,8),AGRect.new(8,0,8,8))
	end
end

app=AGApplication.new

w=MyWidget.new(nil,AGRect.new(0,0,0,0))

app.setMainWidget(w)

app.run