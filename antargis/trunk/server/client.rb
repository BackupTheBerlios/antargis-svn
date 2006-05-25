#!/usr/bin/env ruby

require 'config.rb'
require 'socket'
require 'messages.rb'
require 'thread.rb'
require 'md5.rb'

class Client
	def initialize
		@port=$PORT.to_i
		@socket = TCPSocket.new('localhost', @port)
		@thread=Thread.new {
			runThread
		}
		@instr=""
		@queue=Queue.new
	end
	def finish
		@thread.join
	end
	def type
		:server
	end
	def processMessage(m)
		puts m
		if m.class==TimeMessage
			puts "TIME:#{m.time}"
		end
	end
	def sendMessage(m)
		@socket.puts(Marshal.dump(m))
		puts "message sent"
	end
private
	def runThread
		while true
			c=@socket.gets
			if c.nil?
				break
			end
			puts @instr.class
			@instr+=c
			begin
				m=Marshal.load(@instr)
				l=Marshal.dump(m).length
				@instr=@instr[(l+1)..(@instr.length-1)]
				processMessage(m)
			rescue TypeError => e
				puts "crappy data"
				@instr=@instr[1..(@instr.length-1)] # ignore one char
				puts e
			rescue ArgumentError => e
				#puts e.class
				#puts e
			end
			puts "INSTR: #{@instr}"
			if @instr.nil?
				@instr=""
			end
		end
		puts "LOST CONNECTION"
	end
end

class AntClient<Client
	def initialize(user,pw)
		super()
		@user=user
		@pw=pw
	end
	def processMessage(m)
		puts "PROC"
		puts m
		if m.class==ChallengeMessage
			sendMessage(LoginMessage.new(@user,myhash(m.challenge+myhash(@user+@pw))))
		else
			return super(m)
		end
		return true
	end
end
client=AntClient.new("muh","puh")
client.finish

