#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_hero.rb
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

class AntLocalizer<AGLocalizer
	def initialize(l="de")
		super()
		@table={}
		@lang=l
		read
	end
	def process(x)
		if @table.member?(x)
			if @table[x]!="" and @table[x]!=nil
				return @table[x]
			end
		end
		@table[x]=""
		write
		
		return x
	end
	def find(x)
		process(x)
	end

private
	def read
		c=loadFile("data/local_#{@lang}.txt")
		c.split("\n").each{|l|
			langc,langm=l.split(";;")
			if langc and langm
				@table[langc]=langm
			end
		}
	end
	def write
		o=""
		@table.each{|n,v|
			o+=n+";;"+v+"\n"
		}
		saveFile("data/local_#{@lang}.txt",o)
	end
end

# set localizer, so that it can be accessed from c++ too
setLocalizer($localizer=AntLocalizer.new) # set global due to GC

def translate(a)
	# FIXME:lookup
	$localizer.process(a)
end

def _(a,*args)
	a=translate(a)
	(1..args.length).each{|i|
		a.sub!("{#{i}}",args[i-1].to_s)
	}
	return a
end