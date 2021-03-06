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

#
#
#
#
#
#


$ENV=ENV
def getLanguage
	if $ENV["LANG"]
		$ENV["LANG"].sub(/_.*/,"")
	else
		# windows - try estimating thruogh "Program Files"-dir name
		known={"Programme"=>"de",
		 "Archivos de programa"=>"es"}
		if $ENV.has_key?("CommonProgramFiles")
			n=$ENV["CommonProgramFiles"].split("\\")[1]
			if known[n]
				return known[n]
			end
		end
		return "en"
	end
end

class AntLocalizer<AGLocalizer
	def initialize(l=getLanguage)
		super()
		@table={}
		@lang=l
		read
	end
	def process(x)
		r=myprocess(x)
		#puts "TRANSLATE: #{x} #{r.class}"
		return r
	end
	def myprocess(x)
		if x.is_a?(AGStringUtf8)
			x=x.to_s
		end

		if @table.member?(x)
			if (not @table[x].nil?) and @table[x].length>0
				return AGStringUtf8.new(@table[x].gsub("\\n","\n"))
			end
		end
		x||=""
			
		@table[x]=""
		#write
		return AGStringUtf8.new(x.to_s)
	end
	def find(x)
		process(x)
	end

	def finalize
		write
	end

private
	def read
		c=loadFile("data/local/local_#{@lang}.txt")
		c.split("\n").each{|l|
			langc,langm=l.split(";;")
			if langc
				if not langm
					langm=""
				end
				@table[langc]=langm
			end
		}
	end
	def write
		o=""
		# save incomplete at first
		@table.each{|n,v|
			if v.nil? or v.length==0
				#puts "98: #{n} -- #{v}"
				o+=n.to_s+";;"+v.to_s+"\n"
			end
		}
		@table.each{|n,v|
			if (not v.nil?) or v.length>0
				o+=n.to_s+";;"+v.to_s+"\n"
			end
		}
		saveFile("data/local/local_#{@lang}.txt",o)
		o=""
		#startGC # run GC because of much overhead
	end
end

module MyLocalizer
	# set localizer, so that it can be accessed from c++ too
	setLocalizer(@@localizer=AntLocalizer.new) # set global due to GC
	
	def MyLocalizer.translate(a)
		# FIXME:lookup
		@@localizer.process(a)
	end
	def MyLocalizer.finalize
		@@localizer.finalize
	end
end
def _(a,*args)
	a=MyLocalizer.translate(a).to_s
	(1..args.length).each{|i|
		a.sub!("{#{i}}",args[i-1].to_s)
	}
	a=AGStringUtf8.new(a)
	return a
end