module TestModule
	attr_reader :quitCounter
	def step
		run
	end
	def eventFrame(t)
		puts "FRAME"
		super
		tryQuit
		true
	end
	def tryQuit
		super
		@quitCounter||=0
		@quitCounter+=1
	end
end

def makeTestAppClass(pClass)
	mClass=Class.new(pClass)
	mClass.class_eval{||include TestModule}
	mClass
end
# class TestApp<AntEditorApp
# 	def step
# 		run
# 	end
# 	def eventFrame(t)
# 		puts "FRAME"
# 		super
# 		tryQuit
# 		true
# 	end
# end

class TestWidget
	attr_reader :widget
	def initialize(widget)
		@widget=widget
	end
	def click
# 		r=@widget.getScreenRect
# 		x=r.x+r.w/2
# 		y=r.y+r.h/2
# 		pp r,x,y
# 
# 		sdlEvent=toSDLEvent("SDL_MOUSEBUTTONDOWN:0:1:1:#{x.to_i}:#{y.to_i}")
# 		@widget.eventMouseButtonDown(newEvent(nil,"",sdlEvent))
# 		sdlEvent=toSDLEvent("SDL_MOUSEBUTTONUP:0:1:1:#{x.to_i}:#{y.to_i}")
# 		event=newEvent(nil,"",sdlEvent)
# 		@widget.eventMouseButtonUp(event)

# # 		@widget.eventMouseButtonDown(AGEvent.new(@widget,@widget.getName))
# # 		@widget.eventMouseButtonDown(AGEvent.new(@widget,@widget.getName))
# 		@widget.sigClick.signal(AGEvent.new(@widget,@widget.getName))
 		@widget.doClick(AGEvent.new(@widget,@widget.getName))
	end
	def child(widgetName)
		return TestWidget.new(nil) if @widget.nil?
		TestWidget.new(@widget.getChild(widgetName))
	end
	def visible
		return false if @widget.nil?
		@widget.visible
	end
	def valid
		(not @widget.nil?)
	end
	def is_a?(type)
		@widget.is_a?(type)
	end
	def method_missing(*s)
		@widget.send(*s)
	end
	def printVisible
		widget=@widget
		puts "--"
		while widget
			puts "#{widget} #{widget.getName} #{widget.visible}"
			widget=widget.getParent
		end
		puts "--"
	end
	def kind_of?(type)
		@widget.kind_of?(type)
	end
	def instance_of?(type)
		@widget.instance_of?(type)
	end
	def visible?
		@widget.visible
	end
	def nil?
		@widget.nil?
	end
	def checked?
		@widget.isChecked
	end
end


module GuiTest
protected
	def widget(widgetName)
		TestWidget.new(@app.getMainWidget.getChild(widgetName))
	end
	def clickScreen(x,y)
		@app.eventMouseButtonDown(newEvent(@app,"",toSDLEvent("SDL_MOUSEBUTTONDOWN:0:1:1:#{x.to_i}:#{y.to_i}")))
		@app.eventMouseButtonUp(newEvent(@app,"",toSDLEvent("SDL_MOUSEBUTTONUP:0:1:1:#{x.to_i}:#{y.to_i}")))
	end
end
