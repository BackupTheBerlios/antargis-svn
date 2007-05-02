#!/usr/bin/env ruby

require 'ruby/antargislib.rb'
require 'ruby/ant_models.rb'

class MMap
	def getScene
		$scene
	end
end

def getMap
	MMap.new
end

class MyApp<GLApp
	@@types=[]
	AntModels.getMeshMap.each{|k,v|
		puts "#{k}(#{k.class}):#{v} #{v.class}"
		v.each{|x,y|
			assert{!(k.is_a?(Hash)||x.is_a?(Hash))}
			@@types<<[k,x[0]]
			puts "K:#{k}(#{k.class}) Y:#{x}(#{x.class})"
		}
	}
	puts @@types.inspect
	
	def initialize(w,h)
		super
		$scene=getScene

		tex=nil
		@@types.each{|t,t2|
			puts "TRY CREATE #{t}(#{t.class}) #{t2}(#{t2.class})"
			node=AntModels.createModel(t,t2)
			puts "#{t}:#{t2} failed creation" if node.nil?
			$scene.addNode(node)
			imp=AntImpostorData.new(node,512,512)
	
			tex=imp.getTexture
			name=t.to_s
			if t2
				name+="_"+(t2.to_s)	
			end
			name+=".png"
			imp.getSurface.save(name)
			getScene.removeNode(node)
		}
			
		image=AGImage.new(nil,AGRect2.new(0,0,tex.width,tex.height),tex,false)
		setMainWidget(image)
		
		getScene.removeNode(node)
	end

	def eventFrame(t)
		delay(100)
	puts t
		return true
	end
end

app=MyApp.new(1024,768)

app.run
