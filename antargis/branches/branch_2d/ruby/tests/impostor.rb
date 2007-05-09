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
			x.each{|xi|
				@@types<<[k,xi]
				puts "K:#{k}(#{k.class}) Y:#{x}(#{x.class})"
			}
		}
	}
	puts @@types.inspect
	
	def initialize(w,h)
		super
		$scene=getScene

		hotpoints={}

		#@@types=[[:man,"stand"]]

		tex=nil
		@@types.each{|t,t2|
			puts "TRY CREATE #{t}(#{t.class}) #{t2}(#{t2.class})"
			node=AntModels.createModel(t,t2)
			puts "#{t}:#{t2} failed creation" if node.nil?
			if node.is_a?(AnimMesh)
				anims=node.getData.getAnimations
				puts "# anims:#{anims.length}"
				anims.collect!{|s|s.to_s+""}
				anim=anims[0]
				if anims.member?(t2.to_s)
					anim=t2
# 				else
# 					anims.each{|a|puts "#{a}(#{a.class})"}
# 					puts "t2:#{t2} (#{t2.class})"
# 					raise 1
				end
				puts "anim:#{anim}"
				node.setAnimation(anim.to_s)
				node.advance(0.1)
				node.setRotation(30)
			end
			$scene.addNode(node)
			imp=AntImpostorData.new(node,512,512)
	
			tex=imp.getTexture
			name=t.to_s
			if t2
				name+="_"+(t2.to_s)	
			end
			name+=".png"
			surface=imp.getSurface
			p=surface.shrink2Fit
	
			hotpoints[name]=p
			surface.save(name)
			getScene.removeNode(node)
		}

		text=""
		hotpoints.each{|k,v|text+=k+":"+(v.to_s)+"\n"}
		saveFile("hotspots.txt",text)
			
# 		image=AGImage.new(nil,AGRect2.new(0,0,tex.width,tex.height),tex,false)
# 		setMainWidget(image)
		
		#getScene.removeNode(node)
	end

	def eventFrame(t)
# 		delay(100)
	puts t
		return true
	end
end

#getMain.initVideo(512,512,32,false,true)

app=MyApp.new(1024,768)

app.run
