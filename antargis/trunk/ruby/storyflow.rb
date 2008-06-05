#--
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_hljobs.rb
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
#++
#
# This file contains the class StoryFlow only. Move on for more information.

#
# The class StoryFlow contains a single dialog, that's displayed in-game.
# * A dialog has a name and
# * consists of several "sentences", which
#   * themselves are spoken by a person
#
# a simple example on how to create a story-flow:
#      start=StoryFlow.new("recruit")
#      start.push("Tutorial","Ok, you have recruited some men.")
#      start.push("Tutorial","If you want to recruit all men select the button with the three swords. ....")
#      start.push("Tutorial","When you're done with this go north and defeat your enemies. You .......")
#      tellStory(start)
#
class StoryFlow
  attr_reader :name, :s

  # create a story-flow with identification "name"
  def initialize(name)
    @s=[]
    @pos=0
    @name=name
  end

  # push a text with title "name" into the story-flow
  def push(name,text)
    @s.push([name,text])
  end

  # return current text
  def getCurrent
    return nil if @s.length<=@pos
    c=@s[@pos]
    return c
  end
    

  # you won't need this unless you're implementing within the actual application-object
  def get
    return nil if @s.length<=@pos
    c=@s[@pos]
    @pos+=1
    return c
  end
  # you won't need this unless you're implementing within the actual application-object
  def append(flow)
    @s+=flow.s
    @name=flow.name
  end
  # you won't need this unless you're implementing within the actual application-object
  def back
    if @pos>0
      @pos-=1
    end
  end  
end
