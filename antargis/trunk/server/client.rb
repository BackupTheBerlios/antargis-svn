#!/usr/bin/env ruby

$programDir=Dir.pwd+"/ruby"
# add programdir to path
$:.push($programDir)

$serverDir=Dir.pwd+"/server"
# add programdir to path
$:.push($serverDir)

require 'antargislib.rb'

require 'server/config.rb'
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
		if m.class==WelcomeMessage
			puts uncompress(m.level)
		end
		@queue.push(m)
	end
	def sendMessage(m)
		@socket.puts(AntMarshal.dump(m))
		#@socket.flush
		puts "message sent"
	end
private
	def runThread
		while true
			c=@socket.gets #recv(10000000)
			if c.nil? or c==""
				break
			end
			#puts @instr.class
			#puts c.class
			@instr+=c
			#puts "next try"
			begin
				m=AntMarshal.load(@instr)
				l=AntMarshal.dump(m).length
				@instr=@instr[(l+2)..-1] #(@instr.length-1)]
				processMessage(m)
			rescue TypeError => e
				puts "crappy data"
				@instr=@instr[1..(@instr.length-1)] # ignore one char
				puts e
			rescue ArgumentError => e
				#puts "too few"
				#puts e.class
				#puts e
			end

			if @instr.nil?
				@instr=""
			end
			puts "INSTR: #{@instr.length}"
		end
		puts "LOST CONNECTION"
	end
end

class AntClient<Client
	def initialize(user,pw)
		super()
		@user=user
		@pw=pw
		@state=:login
	end
	def processMessage(m)
		#puts "PROC"
		#puts m
		if m.class==ChallengeMessage
			sendMessage(LoginMessage.new(@user,myhash(m.challenge+myhash(@user+@pw)),getProtocolVersion))
			puts "try login"
		else
			if m.class==WelcomeMessage
				@state=:loggedin
			end
			return super(m)
		end
		return true
	end

	def state
	end
end


client=AntClient.new("muh","puh")

client.finish

