#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

class App<GLApp
	def initialize(w,h,file,tex,zoom=0.1)
		super(w,h)
		@anim=false
		if file==nil
			require 'gen_tree.rb'
			data=genTree
			@n=Mesh.new(getScene,data,AGVector4.new(0,0,0),-30)
		elsif file=~/.*anim/
			data=AnimMeshData.new("data/models/sheep.anim")
			data.setTransform(AGMatrix4.new(Math::PI,AGVector3.new(0,0,1))*AGMatrix4.new(Math::PI/2,AGVector3.new(1,0,0)))
		
			puts "YAY"
			@n=AnimMesh.new(getScene,data)
			@anim=true
		else
			zoom||=0.1
			zoom=zoom.to_f
			data=MeshData.new(file,zoom,tex)
			@n=Mesh.new(getScene,data,AGVector4.new(0,0,0),-30)
		end
			
		if @anim
			@n.setAnimation("go")
		end
		getScene.addNode(@n)
		@rot=0.0
	end
	
	def eventFrame(s)
		super(s)
		delay(10)
		if not @anim
			return
		end
		@rot+=2
		@n.setRotation(AGVector3.new(0,0,1),@rot)
		if @rot>180
			@n.setAnimation("eat")
		end
	end
end

a=nil
if ARGV.length<2
	a=App.new(800,600,nil,nil)
else
	a=App.new(800,600,ARGV[0],ARGV[1],ARGV[2])
end
a.run