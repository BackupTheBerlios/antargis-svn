#!/usr/bin/env ruby

puts "a"
require 'ruby/antargislib.rb'
puts "a"

class App<GLApp
	def initialize(w,h)
		super
		
		data=AnimMeshData.new("data/models/sheep.anim")
		data.setTransform(AGMatrix4.new(Math::PI,AGVector3.new(0,0,1))*AGMatrix4.new(Math::PI/2,AGVector3.new(1,0,0)))
		
		puts "YAY"
		@n=AnimMesh.new(data)
		
		@n.setAnimation("go")
		getScene.addNode(@n)
		@rot=0.0
	end
	
	def eventFrame(s)
		super(s)
		@rot+=2
		@n.setRotation(AGVector3.new(0,0,1),@rot)
		if @rot>180
			@n.setAnimation("eat")
		end
	end
end

m=AGMain.new(800,600,32,false,true)
a=App.new(800,600)
a.run