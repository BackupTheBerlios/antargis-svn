#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# view.rb
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

#!/usr/bin/ruby


class AntRubyView <EditIsoView
	def initialize(p,rect,pos,map)
		super(p,rect,pos,map)
		puts "ANTRUBYVIEW"
		puts p
		puts rect.to_s
		puts pos
		puts getMap
		@updated=false
	end
	def clickMap2(pos)
		puts "CLICKMAP:"
		puts pos.to_s
	end
	def draw(p)
		if not @updated then
			mapUpdate
			@updated=true
		end
		super(p)
		puts "DRAW"
	end
end

class AntRubyViewCreator<AGLayoutCreator
	def initialize()
		super("antRubyView")
		puts "REGISTER ANTRUBYVIEW"
	end
	def create(parent,rect,node)
		w=AntRubyView.new(parent,rect,Pos3D.new(0,0,0),$map)
		puts "RECT:"
		puts rect.to_s
		puts w
		puts w.class
		x=w.class
		while x!=nil do
			puts x
			x=x.superclass
		end
#		exit
		return w
	end
end
$antRubyViewCreator=AntRubyViewCreator.new
