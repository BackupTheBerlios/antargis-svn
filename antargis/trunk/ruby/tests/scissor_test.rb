#!/usr/bin/env ruby


require 'ruby/antargislib.rb'

class MyWidget<AGWidget
	def draw(p)
		super
		p.fillRect(AGRect2.new(-10,-10,30,30),AGColor.new(0,0xFF,0xFF))
		p.fillRect(AGRect2.new(1,1,2,2),AGColor.new(0xFF,0xFF,0xFF))
	end
end

class MyApp<AGApplication
	def initialize
		super
		setMainWidget(MyWidget.new(nil,AGRect2.new(0,0,4,4)))
	end
	def eventFrame(t)
		#puts "eventFram"
		delay(20)
		return true
	end

end


app=MyApp.new
app.run



