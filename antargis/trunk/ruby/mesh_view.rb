#!/usr/bin/env ruby

puts "a"
require 'ruby/antargislib.rb'
puts "a"



class App<GLApp
	def initialize(w,h,file)
		super(w,h)
		@anim=false
		if file=~/.*anim/
			data=AnimMeshData.new("data/models/sheep.anim")
			data.setTransform(AGMatrix4.new(Math::PI,AGVector3.new(0,0,1))*AGMatrix4.new(Math::PI/2,AGVector3.new(1,0,0)))
		
			puts "YAY"
			@n=AnimMesh.new(data)
			@anim=true
		else
			data=MeshData.new("data/models/ant_coach.ant2",3,"data/textures/models/ant_coach.png")
			@n=Mesh.new(data,AGVector4.new(0,0,0),-30)
		end
			
		if @anim
			@n.setAnimation("go")
		end
		getScene.addNode(@n)
		@rot=0.0
	end
	
	def eventFrame(s)
		super(s)
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

if ARGV.length<1
	throw "no file given"
end

m=AGMain.new(800,600,32,false,true)
a=App.new(800,600,ARGV[0])
a.run