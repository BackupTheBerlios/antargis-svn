#!/usr/bin/env ruby

puts "a"
require 'antargislib.rb'
puts "a"

class App<GLApp
	def initialize(w,h)
		super
		
		data=AnimMeshData.new(File.open("yago.ant3").read,0.1,"yago.png")
		#data=AnimMeshData.new(File.open("bert.ant3").read,0.1,"yago.png")
		data.apply(AGMatrix4.new(Math::PI,AGVector3.new(0,0,1))*AGMatrix4.new(Math::PI/2,AGVector3.new(1,0,0)))
		puts "YAY"
		n=AnimMesh.new(data)
		
		getScene.addNode(n)
		
	end
end

m=AGMain.new(800,600,32,false,true)
a=App.new(800,600)
a.run