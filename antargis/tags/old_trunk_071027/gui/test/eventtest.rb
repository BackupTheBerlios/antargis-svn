#!/usr/bin/env ruby

require 'antargisgui.rb'

class MyWidget<AGWidget
	#attr_reader :sigMuh
	def initialize(p,r)
		super(p,r)
		b=AGButton.new(self,AGRect.new(10,10,100,20),"hupe")
		addChild(b)
		addHandler(b,:sigClick,:sigMuh)
		addSignal("sigMuh")
	end
	def draw(p)
		p.fillRect(getRect.origin,AGColor.new(0,0,0xFF))
	end
end

class MyApp<AGApplication
	def initialize
		super
		w=MyWidget.new(nil,AGRect.new(0,0,300,300))
		puts "SIGMUH:",w.sigMuh
		setMainWidget(w)

		puts w.sigMuh
		#exit

		addHandler(w,:sigMuh,:sigBla)
	end
	def sigBla
		puts "sigBla"
	end
end

$main=AGMain.new(640,480,32,false,true)

app=MyApp.new
app.run