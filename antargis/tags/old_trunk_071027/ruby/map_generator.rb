#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# map_generator.rb
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

class GeneratorParameters
	attr_accessor :size,:water,:hills,:population,:enemies
	attr_reader :params
	def initialize
		@params=[:size,:water,:hills,:population,:enemies]
		@size=128
		@water=0.4
		@hills=1
		@population=40
		@enemies=2
	end
	def readFromDialog(d)
		@params.each{|p|
			v=d.edits[p].getText.to_s
			case p	
				when :water
					v=v.to_f
				else
					v=v.to_i
			end
			eval("self.#{p}=v")
		}
	end
end

def addFlatOnMap(map,x,y,amount,size)
	w=map.getW
	h=map.getH
	s2=size/2
	x0=[0,w-1,x-s2].sort[1].to_i
	x1=[0,w-1,x+s2].sort[1].to_i
	y0=[0,w-1,y-s2].sort[1].to_i
	y1=[0,w-1,y+s2].sort[1].to_i
	(x0..x1).each{|mx|
		(y0..y1).each{|my|
			if (mx-x)**2+(my-y)**2<s2**2
				h=map.get(mx,my)
				a=[amount+h,rand*2+6].min # not too high
				map.set(mx,my,a)
			end
		}
	}
end

def addFlatOnMapTerrain(map,x,y,amount,size,tt)
	w=map.getW
	h=map.getH
	s2=size/2
	x0=[0,w-1,x-s2].sort[1].to_i
	x1=[0,w-1,x+s2].sort[1].to_i
	y0=[0,w-1,y-s2].sort[1].to_i
	y1=[0,w-1,y+s2].sort[1].to_i
	(x0..x1).each{|mx|
		(y0..y1).each{|my|
			if (mx-x)**2+(my-y)**2<s2**2
				h=map.getTerrain(mx,my,tt)
				a=[amount+h,rand*2+6].min # not too high
				map.setTerrain(mx,my,tt,a)
			end
		}
	}
end

def blurMap(map)
	vps=[[0,0],[1,0],[0,1],[1,1]]
	(0..(map.getW-2)).each{|x|
		(0..(map.getH-2)).each{|y|
			c=0
			vps.each{|v|
				c+=map.get(x+v[0],y+v[1])
			}
			c/=vps.length
			map.set(x,y,c)
		}
	}
end

# ensure that beaches are steep enough, so that water doesn't run too far
def checkBeachSteepness(map)
	thres=0.1
	(0..(map.getW-1)).each{|x|
		(0..(map.getH-1)).each{|y|
			h=map.get(x,y)
			if h>-thres and h<0
				h=-thres
			elsif h>=0 and h<thres
				h=thres
			end
			map.set(x,y,h)
		}
	}
end

def addWaterTerrain(map)
	tt=WATER
	(0..(map.getW-1)).each{|x|
		(0..(map.getH-1)).each{|y|
			if map.get(x,y)<0
				map.setTerrain(x,y,tt,0.7)
			end
		}
	}
end

def blurTerrain(map,tt)
	vps=[[0,0],[1,0],[0,1],[1,1]]
	(0..(map.getW-2)).each{|x|
		(0..(map.getH-2)).each{|y|
			c=0
			vps.each{|v|
				c+=map.getTerrain(x+v[0],y+v[1],tt)
			}
			c/=vps.length
			map.setTerrain(x,y,tt,c)
		}
	}
end

def addGrass(map)
	tt=GRASS
	thres=0.3
	(0..(map.getW-1)).each{|x|
		(0..(map.getH-1)).each{|y|
			if map.get(x,y)>thres
				map.setTerrain(x,y,tt,rand)
			end
		}
	}
	
	tt=GRASS2
	thres=1.3
	(0..(map.getW-1)).each{|x|
		(0..(map.getH-1)).each{|y|
			if map.get(x,y)>thres
#				map.setTerrain(x,y,tt,rand)
				r=rand
				map.setTerrain(x,y,tt,r)
				if r>0.97
					# add tree
					t=AntGrass.new
					t.setPos(AGVector2.new(x,y))
					map.insertEntity(t)
				end
			end
		}
	}
end

def addRock(map)
	w=map.getW
	h=map.getH
	flat=[[5,10],[4,10],[3,20],[2,20]]
	flat.each{|add|
		for i in 1..add[1]
			rad=add[0]
			x=rand*(w-2*rad)+rad
			y=rand*(h-2*rad)+rad
			#map.addFlat(x,y,30,rad)
			if map.get(x.to_i,y.to_i)>1
				addFlatOnMapTerrain(map,x,y,1,rad,ROCK)
			end
		end
	}
end

def addForest(map)
	tt=FOREST
	thres=1.0
	(0..(map.getW-1)).each{|x|
		(0..(map.getH-1)).each{|y|
			if map.get(x,y)>thres
				r=rand
				map.setTerrain(x,y,tt,r)
				if r>0.97 and map.getTerrain(x,y,ROCK)<0.3
					# add tree
					t=AntTree.new
					t.setPos(AGVector2.new(x,y))
					map.insertEntity(t)
				end
			end
		}
	}
end


def generateMap(map,params)
	w=params.size
	h=params.size

	map.newMap(w,h)
	getMap.setHeight(2) # everything deep water

 		flat=[[30,10],[15,20],[10,8],[5,40],[4,10],[3,20],[2,20],[1,70]]
 		flat.each{|add|
 			for i in 1..add[1]
 				rad=add[0]
 				x=rand*(w-2*rad)+rad
 				y=rand*(h-2*rad)+rad
 				#map.addFlat(x,y,30,rad)
				addFlatOnMap(map,x,y,2,rad)
 			end
 		}

	blurMap(map)
	blurMap(map)
	blurMap(map)
	
	checkBeachSteepness(map)

	addWaterTerrain(map)
	blurTerrain(map,WATER)

	addGrass(map)
	addRock(map)
	addForest(map)

	map.mapChanged
end