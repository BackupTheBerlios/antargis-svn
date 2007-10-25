#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# layout.rb
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
# == File description
# This file contains some helper functions to provide easier event-Handling in ruby.
# Have a look at AGHandler for more information.


# checks if libantargis is already loaded
def hasLibantargis
	begin
		x=eval("Antargis")
	rescue
		return false
	end
	return true
	#puts x
	#x!=nil
end

# if libantargis was not yet loaded, try loading libantargisgui alone
if not hasLibantargis
	require 'antargisgui'
	include Antargisgui
end

# some old abbrevs
AGPoint=AGVector2
AGRect=AGRect2

#
# EVENT_HANDLERS
#
# this module adds simpler event handling to your ruby code
# each slot should have for parameters: (event)
# add a slot with this:
# * addHandler(object,:signalName,:slotName)
# and also include this module into each class in which you want to use this :-)
# this is not needed if you're using an AGWidget or AGApplication object
module AGHandler
	# clear all handlers
	def clearHandlers
		@handlers={}
	end
	# add Event Handler - this function should go into AGRubyApp
	def addHandler(object,event,func)
		if not defined? @handlers then
			@handlers={}
		end
		if object.nil? then
			raise "AGHandler.addHandler: object does not exist!"
		else
			object.send(event).connect(self)
			hname=makeHandlerName(object,event)
			#hname=object.getName+":"+event.to_s
			@handlers[hname]||=[]
			@handlers[hname].push(func)
		end
	end
	# event dispatcher
	def signal(e)
		@handlers||={}
		mcaller=e.getCaller
		evName=makeHandlerName(mcaller,e.getName)
		#callerName=e.getCaller.getName
		#evName=callerName+":"+e.getName
		#puts "#{callerName} #{evName}"
		if @handlers.has_key?(evName) then
			value=false
			@handlers[evName].each{|handler|
				m=method(handler)
				if m.arity==1 || m.arity==-1
					# ok
					if self.send(handler,e)
						value=true
					end
				elsif m.arity==0
					if self.send(handler)
						value=true
					end
				else
					puts "AR:",m.arity
					raise "SLOT is invalid! event:"+evName+" slotname:"+handler.to_s+" in class:"+(self.class).to_s
				end
			}
			return value
		end
		return super(e)
	end

	private
	def makeHandlerName(object,event)
		if object.respond_to?(:getName)
			return object.getName+":"+event.to_s
		else
			return object.object_id.to_s+":"+event.to_s
		end
	end
end

# this function adds a signal to the current object
# you can call this with signalName(e) afterwards
def addSignal(name)
	eval <<EOT
class #{self.class}
	def #{name}(e=nil)
		if e
			e.setCaller(self)
			return @#{name}.signal(e)
		else
			return @#{name}
		end
	end
end
EOT
	eval("@#{name}=AGSignal.new(self,'#{name}')\n")
end

module Antargisgui
	class AGApplication
		include AGHandler
	end
	class AGWidget
		include AGHandler
	end
end

class Array
	# extend Array-class with a shuffle function
	def shuffle
		sort{0.5 <=> rand}
	end
end
# just in case it isn't defined
class AGString
end
# class String
# 	alias oldadd +
# 	def +(p)
# 		#puts "puh"
# 		if p.is_a?(AGString)
# 			p=p.to_s
# 		end
# 		oldadd(p)
# 	end
# end

module Antargisbasic
	class AGVector3
		def x=(p)
			setX(p)
		end
		def y=(p)
			setY(p)
		end
		def z=(p)
			setZ(p)
		end
	end
	class AGVector2
		def x=(p)
			setX(p)
		end
		def y=(p)
			setY(p)
		end
		def _dump(depth)
			[x,y].pack("gg")
		end
		def AGVector2._load(s)
			a,b=s.unpack("gg")
			AGVector2.new(a,b)
		end
	end
end
