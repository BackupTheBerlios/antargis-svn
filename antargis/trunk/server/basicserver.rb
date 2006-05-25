#!/usr/bin/env ruby

require 'config.rb'
require 'thread.rb'
require 'socket'

# threaded server

class Connection
	attr_accessor :name
	def initialize(session,server)
		@session=session
		@server=server
		@instr=""
	end
	def runConnection
		puts "new Client.."
		@server.addConnection(self)
		run
		puts "lost Connection..."
		@server.removeConnection(self)
	end
	def run
		while @session
			c=@session.gets
			if c.nil?
				break
			end
			@instr+=c
			begin
				m=Marshal.load(@instr)
				l=Marshal.dump(m).length
				@instr=@instr[(l+1)..(@instr.length-1)]
				@server.pushMessage(self,m)
			rescue TypeError => e
				@instr=@instr[1..(@instr.length-1)] # ignore one char
				puts "crappy data"
				puts e
			rescue ArgumentError => e
				# ignore - short of data
			end
		end
	end

	def sendMessage(m)
		#puts "sent message #{m}"
		x=Marshal.dump(m)
		#puts "DUMP:#{x}"
		send(x)
	end

	def send(x)
		begin
			@session.puts(x)
			#puts "message sent '#{x}'!"
		rescue
		end
	end
	def type
		:client
	end
end

class Server
	def initialize
		@port = $PORT.to_i
		@server = TCPServer.new('localhost', @port)
		@connections=[]
		@queue=Queue.new
		run
	end
	def join
		@mthread.join
	end
	def send(x)
		@connections.each{|c|
			c.send(x)
		}
	end

	def sendMessage(m)
		@connections.each{|c|
			c.sendMessage(m)
		}
		#send(Marshal.dump(m))
	end
	def recvMessage
		begin
			r=@queue.pop
		rescue
		end
		r
	end

	def eventNewConnection(c)
	end
	def eventLostConnection(c)
	end
	def processMessage(c,m)
	end


	def addConnection(c)
		@connections.push(c)
		eventNewConnection(c)
	end
	def removeConnection(c)
		@connections.delete!(c)
		eventLostConnection(c)
	end
	def pushMessage(c,m)
		if (not processMessage(c,m))
			@queue.push(m)
		end
	end
		
private
	def run
		@mthread=Thread.new {
			while (session = @server.accept)
				Thread.start do
					connection=Connection.new(session,self)
					connection.runConnection
				end
			end
		}
	end
end

