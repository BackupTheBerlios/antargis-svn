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

#!/usr/bin/ruby

def hasLibantargis
	begin
		x=eval("Libantargis")
	rescue
		return false
	end
	return true
	#puts x
	#x!=nil
end

if not hasLibantargis
	puts "libant NOT found"
	require 'libantargisgui'
	include Libantargisgui
end

AGPoint=AGVector2
AGRect=AGRect2

#
# EVENT_HANDLERS
#
# this module adds simpler event handling to your ruby code
# each slot should have for parameters: (eventname,callerName,eventItself,callerHimself)
# add a slot by addHandler(object,:signalName,:slotName)
# and also include this module into each class in which you want to use this :-)
module AGHandler
	def clearHandlers
		@handlers={}
	end
	# add Event Handler - this function should go into AGRubyApp
	def addHandler(object,event,func)
		if not defined? @handlers then
			@handlers={}
		end
		#puts event
		if object==nil then
			raise "AGHandler.addHandler: object does not exist!"
		else
			#puts "MY CLASS:",self.class
			object.send(event).connect(self)
			if @handlers[object.getName+":"+event.to_s]==nil
				@handlers[object.getName+":"+event.to_s]=[]
			end
			@handlers[object.getName+":"+event.to_s].push(func)
		end
	end
	# event dispatcher
	
	def signal(e)
		@handlers||={}
		callerName=e.getCaller.getName
		evName=callerName+":"+e.getName
		#dputs "EVNAME",evName
		#dputs @handlers.keys.join(";")
		#puts "SIGNAL"
		#puts callerName,evName
		#puts @handlers.keys.join(",")
		
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
	
	def signalOld(name,event,caller)
		if not defined? @handlers then
			@handlers={}
		end
		callerName=toAGWidget(caller).getName
		evName=callerName+":"+name
		
		if @handlers.has_key?(evName) then
			value=false
			@handlers[evName].each{|handler|
				m=method(handler)
				if m.arity==4 then
					if self.send(handler,name,callerName,event,caller) then
						value=true
					end
				elsif m.arity==1 then
					if self.send(handler,callerName) then
						value=true
					end
				else
					if self.send(handler) then
						value=true
					end
				end
			}
			return value
		else
			puts "HANDLER NOT FOUND"
		end
		return super(name,event,caller)
	end
end

def addSignal(name)
	eval <<EOT
class #{self.class}
	def #{name}(e=nil)
		#puts "SIGNALLLLLLLLLLLLLLLLLLLLLL"
		#puts e
		if e
			e.setCaller(self)
			#x=AGSignal.new(self,"#{name}")
			return @#{name}.signal(e)
		else
			return @#{name}
		end
	end
end
EOT
	eval("@#{name}=AGSignal.new(self,'#{name}')\n")
end

class AGApplication
	include AGHandler
end
class AGWidget
	include AGHandler
end


class Array
	def shuffle
		sort{0.5 <=> rand}
	end
end
