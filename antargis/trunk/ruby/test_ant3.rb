#!/usr/bin/env ruby

# small (example) application to test animations and view animated meshes

require 'ruby/antargislib.rb'

class App<GLApp
  def initialize(w,h)
    super
    
    data=AnimMeshData.new("data/models/sheep.anim")
    data.setTransform(AGMatrix4.new(Math::PI,AGVector3.new(0,0,1))*AGMatrix4.new(Math::PI/2,AGVector3.new(1,0,0)))
    
    puts "YAY"
    @n=AnimMesh.new(getScene,data)
    
    @n.setAnimation("go")
    getScene.addNode(@n)
    @rot=0.0
  end
  
  def eventFrame(s)
    super(s)
    @rot+=1
    @n.setRotation(AGVector3.new(0,0,1),@rot)
    if @rot>180
      @n.setAnimation("eat")
    end
  end
end

a=App.new(800,600)
a.run
