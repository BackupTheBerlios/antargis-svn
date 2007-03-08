#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_formations.rb
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

class AntFormation
	def initialize(boss)
		@boss=boss
		@cache={}
		@sorted=nil
		@inited=false
		calcFormation
	end
	def calcFormation
	end
	def getPosition(man,pos)
		if pos.nil?
			raise "pos nil"
		end
		if @cache.member?(man)
			return @cache[man]+pos
		end
		r=getPositionReal(man)
		if r.nil?
			calcFormation # try to recalc formation
			r=getPositionReal(man)
			if r.nil?
				if not @boss.getMen.member?(man)
					puts "/// #{man} #{@boss}"
					puts "MAN:",man.uid," BOSS:",@boss.uid
					puts "MEN:",@boss.getMen.collect{|m|m.uid.to_s}.join(" ")
					raise "Man #{man} is not member of #{@boss}'s men!"
				end
				raise "Problem in AntFormation!"
			end
		end
		@cache[man]=r
		return r+pos
	end
	def getSortedMen
		if @sorted.nil?
			@sorted=sort(@boss.getMen)
		end
		@sorted
	end
	def sort(men)
		raise "not implemented"
	end
end

def betterWeapons(a,b)
	weapons=["bow","sword"]
	weapons.each{|w|
		r=(a.resource.get(w)<=>b.resource.get(w))
		if r!=0
			return r
		end
	}
	return 0
end


class AntFormationRest<AntFormation
	def initialize(boss)
		super(boss)
	end

	def calcFormation
		@rpos={} # real positions as map from man to AGVector2
		vpos={}  # virtual positions as map from man to pair of [row,line (circle)]

		men=sort(@boss.getMen)
		@men=men

		linesizes={}
		line=1
		row=0
		men.delete(@boss)
		men.each{|m|
			vpos[m]=[row,line]
			linesizes[line]||=0
			linesizes[line]+=1
			row+=1
			if row>getRowsOfLine(line)  # add check for new weapon group here
				row-=getRowsOfLine(line)
				line+=1
			end
		}
		men.each{|m|
			row,line=vpos[m]
			radius=line*1.2
			angle=row.to_f/linesizes[line]*Math::PI*2
			@rpos[m]=AGVector2.new(Math::cos(angle)*radius,Math::sin(angle)*radius)
			#puts "SET:#{m}"
		}
		@rpos[@boss]=AGVector2.new(0,0)
		if @men.length>0
			@inited=true
		end
	end
	def getPositionReal(man)
		if (not @inited)
			calcFormation
		end
		return @rpos[man]
	end
	private
	def getRowsOfLine(line)
		{1=>10,2=>14,3=>20,4=>40}[line]
	end

	def sort(men)
		men.sort{|a,b|betterWeapons(a,b)}
	end

end

class AntFormationBlock<AntFormation
	def initialize(boss,dir)
		@dir=dir
		super(boss)
	end
	def calcFormation
		boss=@boss
		dir=@dir
		# precalc formation and store into @rpos
	
		men=boss.getMen
		# sort from back to front (lines)
		men=sort(men)

		vpos={}
		@rpos={}
		linesizes={}
		line=0
		row=0

		rowsize=5

		normal=dir.normal
		men.each{|m|
			vpos[m]=[row,line]
			
			linesizes[line]||=0
			linesizes[line]+=1
			row+=1
			if row>rowsize-1 # or weapon-group changed
				row=0
				line+=1
			end
		}
		men.each{|m|
			row,line=vpos[m]
			linesize=linesizes[line]
			@rpos[m]=normal*(row-(linesize/2.0))*0.5 + dir*line*0.7
		}


	end
	def getPositionReal(man)
		@rpos[man]
	end
	def sort(men)
		men=men.sort{|a,b|betterWeapons(a,b)}
		men.delete(@boss)
		a=men[0..1]
		b=[@boss]
		c=men[2..-1]
		a||=[]
		b||=[]
		c||=[]
		a+b+c
	end
end

# FIXME: not yet implemented
#
# class AntFormationV
# end
# 
# class AntFormationCircle
# end