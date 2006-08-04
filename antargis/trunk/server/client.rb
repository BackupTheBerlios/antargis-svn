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
		# FIXME: currently does only connect to localhost
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
		d=AntMarshal.dump(m)
		Thread.critical=true
		begin
			@socket.puts d
		rescue Errno::EPIPE => e
			Thread.critical=false
			connectLost
		rescue Errno::ECONNRESET => e
			Thread.critical=false
			connectLost
		end
		puts "message sent    #{d.length}"
	end

	def connectLost
			puts "CONNECTION LOST"	
			exit
	end
private
	def runThread
		while true
			c=@socket.gets
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

