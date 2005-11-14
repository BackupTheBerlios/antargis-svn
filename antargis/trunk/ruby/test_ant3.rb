#!/usr/bin/env ruby

puts "a"
require 'antargislib.rb'
puts "a"

class App<GLApp
	def initialize(w,h)
		super
		
		data=AnimMeshData.new(File.open("yago.ant3").read,0.1,"yago.png")
		#data=AnimMeshData.new(File.open("bert.ant3").read,0.1,"yago.png")
		data.setTransform(AGMatrix4.new(Math::PI,AGVector3.new(0,0,1))*AGMatrix4.new(Math::PI/2,AGVector3.new(1,0,0)))
		#data.setTransform(AGMatrix4.new(Math::PI,AGVector3.new(0,0,1)))
		
		
		puts "YAY"
		@n=AnimMesh.new(data)
		
		#n.setRot(AGVector3.new(0,0,1),180)
		#n.setRot(AGVector3.new(1,0,0),90)
		
		getScene.addNode(@n)
		@rot=0.0
	end
	
	def eventIdle
		return
		#puts "h"
		@rot+=1
		@n.setRot(AGVector3.new(0,0,1),@rot)
	end
end

m=AGMain.new(800,600,32,false,true)
a=App.new(800,600)
a.run