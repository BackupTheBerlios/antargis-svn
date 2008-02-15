require 'ruby/spec_helper.rb'
require 'ruby/gui/testing.rb'
require 'ruby/ant_application.rb'

module LevelTesting
	class TestEntity
		class ClickNode
			attr_accessor :node
		end

		def initialize(entity,app)
			@entity=entity
			@app=app
		end
		def click(button=1)
			n=ClickNode.new
			n.node=@entity.getMesh[0]
			@app.eventEntitiesClicked([n],button)
		end
		def method_missing(*s)
			@entity.send(*s)
		end
	end
	
	def entities(type)
		map.getByType(type).collect{|e|TestEntity.new(e,@app)}
	end
	
	include GuiTest
	def getTestApp(testLevelName)
		case testLevelName
			when :tutorial0
				file="data/levels/tutorial/tutorial0.antlvl"
			when :tutorial3
				file="data/levels/tutorial/tutorial3.antlvl"
	    end
	    clientConnection=nil
	    @app=makeTestAppClass(AntGameApp).new(loadFile(file),getVideo.width,getVideo.height,nil,clientConnection)
	end
	
	def runUntilLowLevelJobToFinish(entity)
		observe(entity,:eventJobFinished) do |observer|
			while observer.isNotCalled
				advance
			end
		end
	end
	
    def clickAwayStory
        telltaleWidget=widget("TellTale")
		raise if telltaleWidget.nil?
        return if telltaleWidget.nil?

        telltaleWidget.should_not be_nil
        telltaleWidget.should be_visible
        okButton=telltaleWidget.child("ok")
        okButton.should_not be_nil
        okButton.click
        trials=0
		
        while telltaleWidget.visible?
            okButton.click
            @app.step
            trials+=1
            trials.should <10
        end

    end
	
	
	def advance(time=nil)
		step=0.3
		time||=0
		begin
			@app.getMap.move(step)
			@app.step
			time-=step
		end while time>step
	end	

    def hero(name)
    	h=map.getByName(name)
		h.should_not be_nil
		h.should be_a_kind_of(AntHero)
		h
    end
	def building(name)
		b=map.getByName(name)
		b.should_not be_nil
		b.should be_a_kind_of(AntHouse)
		b
	end
	def map
		@app.getMap
	end
	
	def clickMap(pos2d,button=1)
		@app.eventMapClicked(pos2d,button)
	end
	
	def waitForStory
		while not (widget("TellTale").valid and widget("TellTale").visible?)
			advance
		 end
	end		
end