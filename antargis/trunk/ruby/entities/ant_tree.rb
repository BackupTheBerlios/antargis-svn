#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_tree.rb
# by David Kamphausen (david.kamphausen@web.de)
#
# The "Antargis" project, including all files needed to compile it,
# is free software; you can redistribute it and/or use it and/or modify it
# under the terms of the GNU General Public License as published
# by the Free Software Foundation; either version 2 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# You should have received a copy of the GNU General Public
# License along with this program.
#

require 'ant_grass.rb'


class AntTree<AntRubyEntity
  def initialize(map,typeID=nil)
    super(map)
    @typeID=typeID
    #@typeID||=(getRand*getTreeTypes.length).to_i
    @typeID||=(getRand*(AntModels.getMeshCount(:tree)-1)).to_i
    setProvide("wood",true)
    setProvide("fruit",true)
    @angle=getRand*360
    
    @maxApples=100
    setMinimapColor(AGColor.new(0,0x77,0))
  
    resource.set("fruit",5)
    resource.set("wood",20)
    resourceChanged
  end
  def resourceChanged
    if resource.get("wood")<=0
      @typeID=-1
      setProvide("fruit",false)
      setProvide("wood",false)
    end
    setupMesh
    super
  end
  
  def setTreeType(t)
    @typeID=t
  end
  def saveXML(node)
    super(node)
    node.set("typeID",@typeID.to_s)
  end
  def loadXML(node)
    super(node)
    @typeID=node.get("typeID").to_i
    setupMesh
  end
  
  def eventNoJob
    # grow apples
    resource.set("fruit",[resource.get("fruit")+1.7,@maxApples].min)
    resourceChanged
    newRestJob(20)
  end
  
  private
  def setupMesh
    typeId="stub"
    if @typeID>=0
      typeId=@typeID%10
    end
    mesh=setMesh(typeId)
    
    mesh.setRotation(@angle) if mesh
  end

  # an old function for display apples - this is too costly
  def setupApples
    @crownMiddle=AGVector3.new(0,0,3)
    @crownRadius=1.3
    @applePos=[]
    for i in 1..@maxApples
      va=(getRand-0.5)*Math::PI
      ha=(getRand+1)*Math::PI  # only on front side
      z=@crownRadius*Math::sin(va)
      rest=Math::sqrt(@crownRadius**2-z**2)
      y=Math::sin(ha)*rest
      x=Math::cos(ha)*rest
      @applePos.push(AGVector3.new(x,y,z)+@crownMiddle)
    end
  end
end

