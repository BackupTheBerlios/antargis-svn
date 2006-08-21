#!/usr/bin/env ruby

# add directories to PATH
$programDir=Dir.pwd+"/ruby"
$:.push($programDir)
$serverDir=Dir.pwd+"/server"
$:.push($serverDir)

require 'antargislib.rb'
require 'server/config.rb'
require 'socket'
require 'messages.rb'
require 'thread.rb'
require 'md5.rb'
require 'antargis.rb'


# a very simple TCP-client with a recv-thread
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
		puts "PROCESS..."
		if m.class==TimeMessage
			puts "TIME:#{m.time}"
		end
		puts "push in queue"
		@queue.push(m)
		puts "pushed"
	end
	def getMessage
		if @queue.length>0
			return @queue.pop
		end
		return nil
	end
	def sendMessage(m)
		@socket.puts(AntMarshal.dump(m))
		#@socket.flush
		puts "message sent"
	end
private
	def runThread
		while true
			#puts "GETTING STH"
			c=@socket.gets
			#puts "READ STH"
			if c.nil? or c==""
				break
			end
			@instr+=c
			begin
				m=AntMarshal.load(@instr)
				l=AntMarshal.dump(m).length
				@instr=@instr[(l+2)..-1] #(@instr.length-1)]
				puts "Processing message"
				processMessage(m)
				puts "Processing message!"
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
		case m
			when ChallengeMessage
				sendMessage(LoginMessage.new(@user,myhash(m.challenge+myhash(@user+@pw)),getProtocolVersion))
				puts "try login"
				return true
			when WelcomeMessage
				@state=:loggedin
				super
				return true
		end
		return false
	end

	def state
	end
	def getName
		@user
	end
end

#login
client=AntClient.new("muh","puh")

welcomeMessage=nil
#wait for welcome message
while true
	welcomeMessage=client.getMessage
	if (not welcomeMessage.nil?)
		break
	end
	sleep 0.1
end

# decompress level-text
leveltext=uncompress(welcomeMessage.level)
# start level
startGame(leveltext,client)


