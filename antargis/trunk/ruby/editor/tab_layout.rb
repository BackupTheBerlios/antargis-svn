#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# tab_layout.rb
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

class AntTabLayout<AGTable
  def initialize(p,r)
    super
    @s=AGSurface.load("data/gui/buttontest3.png")
    @sw=@s.width/3
    @sh=@s.height/3
    @ts={}
    (0..2).each{|x|
      (0..2).each{|y|
        @ts[[x,y]]=AGTexture.new(@s.getSubSurface(x*@s.width/3,y*@s.height/3))
      }
    }
  end
  def draw(p)
    p.blit(@ts[[0,0]],AGRect2.new(0,0,@sw,@sh))
  end
end


class AntTabLayoutCreator<AGLayoutCreator
  def initialize()
    super("antTabLayout")
  end
  def create(parent,rect,node)
    e=AntTabLayout.new(parent,rect)
    e.setName(node.get("name"))
    puts node.get("name")
    #raise 1
    return e
  end
end


$antTabLayoutCreator=AntTabLayoutCreator.new
