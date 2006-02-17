#!/usr/bin/env ruby

class StoryFlow
	attr_reader :name
	def initialize(name)
		@s=[]
		@pos=0
		@name=name
	end
	def push(name,text)
		@s.push([name,text])
	end
	def get
		return nil if @s.length<=@pos
		c=@s[@pos]
		@pos+=1
		return c
	end
end