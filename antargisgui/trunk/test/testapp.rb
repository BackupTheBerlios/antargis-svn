#!/usr/bin/ruby

require 'libantargisruby'
require 'sdl'
require 'theme.rb'

include Libantargisruby

class TestApp <AGApplication
	def initialize(autoexit=true)
		initTheme
		@count=0
		@autoexit=autoexit
		super()
	end
	def eventQuit(event)
		puts "Quitting"
		super(event)
	end
	def eventIdle
		#puts "idle count:"+@count.to_s
		@count+=1
		if @count>=20 then
			if @autoexit then
				puts "correct quit after 20 idles"
				tryQuit
			end
		end
	end
end
