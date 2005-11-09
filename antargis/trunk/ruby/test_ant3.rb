#!/usr/bin/env ruby

puts "a"
require 'antargislib.rb'
puts "a"

class App<GLApp
	def initialize(w,h)
		super
		
		data=AnimMeshData.new(File.open("yago.ant3").read)
		
		#n=AnimMesh.new(
		
		#getScene.addNode(n)
		
	end
end

m=AGMain.new(800,600,32,false,true)
a=App.new(800,600)
a.run