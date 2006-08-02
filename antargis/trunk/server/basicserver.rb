#!/usr/bin/env ruby

require 'config.rb'
require 'thread.rb'
require 'socket'

#
# These classes provide a threaded TCP-Server that handles messages
#

# A connection is an object handling the connection of a server to the client
class Connection
	attr_accessor :name, :id
	def initialize(session,server)
		@session=session
		@server=server
		@instr=""
	end

	# this should be run by the server-object only
	def runConnection
		@server.addConnection(self)
		run
		@server.removeConnection(self)
	end

	# send some message to the client
	def sendMessage(m)
		#Thread.start{
			#puts "Marshalling #{m.class}"
			x=AntMarshal.dump(m)
			#File.open("muh","w").puts x
			send(x)
		#}
	end
	# identifies the other side of the connection
	def type
		:client
	end

private
	def send(x)
		#begin
			puts "TRY SENDING"
			puts Thread.object_id
			puts "#{x.length} #{x.class}"
			begin
				@session.puts(x)
			rescue Errno::EPIPE => e
				puts "pipe broken"
				@server.removeConnection(self)
			rescue Errno::ECONNRESET => e
				puts "pipe broken"
				@server.removeConnection(self)
			end
			puts "SENT STH"
		#rescue
		#end
	end
	def run
		begin
			while @session
				c=@session.gets #recv(10000000)
				if c.nil? or c==""
					break
				end
				@instr+=c
				begin
					m=AntMarshal.load(@instr)
					l=AntMarshal.dump(m).length
					@instr=@instr[(l+2)..-1] #(@instr.length-1)]
					#@instr=@instr[(l+1)..-1]
					@server.pushMessage(self,m)
				rescue TypeError => e
					@instr=@instr[1..-1] #(@instr.length-1)] # ignore one char
					puts "crappy data"
					puts e
					raise
				rescue ArgumentError => e
					# ignore - short of data
					puts "too short?"
				end
				if @instr.nil?
					@instr=""
				end
				puts "INSTR: #{@instr.length}"
	
			end
		rescue Errno::EPIPE => e
			puts "pipe broken"
			@server.removeConnection(self)
		rescue Errno::ECONNRESET => e
			puts "pipe broken"
			@server.removeConnection(self)
		end
		puts "LOST CONNECTION"

	end
end


# Server opens an TCP-port on $PORT (defined in config.rb) on the localhost 
class Server
	def initialize
		@port = $PORT.to_i
		@server = TCPServer.new('localhost', @port)
		@connections={}
		@queue=Queue.new
		run
	end

	# clean shutdown
	def join
		@mthread.join
	end
# 	def send(x)
# 		@connections.each{|k,c|
# 			c.send(x)
# 		}
# 	end

	def sendMessage(id,m)
		c=@connections[id]
		if c
			c.sendMessage(m)
		end
	end

	def sendMessageToAll(m)
		@connections.each{|k,c|
			c.sendMessage(m)
		}
	end

	def sendToAllBut(m,but)
		raise 1 if but.is_a?(Connection)
		@connections.each{|k,c|
			puts "sendToAllBut(m,but) #{k} #{c}"
			if k!=but
				c.sendMessage(m)
			end
		}
	end

	# deprecated function - see processMessage
	def recvMessage
		r=nil	
		begin
			if @queue.length>0
				r=@queue.pop
			end
		rescue
		end
		r
	end

	# semi-private functions - should be called by Connection-objects only
	# add new connection
	def addConnection(c)
		id=getFirstConnectionID
		@connections[id]=c
		c.id=id
		puts "new connection #{c}"
		#eventNewConnection(c)
		m=NewConnectionMessage.new(id)
		@queue.push([id,m])
		puts "ok. had new connection"
	end
	def removeConnection(c)
		@connections.delete(c.id)
		
		#eventLostConnection(c)

		if @connections.length==0
			# FIXME
			puts "EEEEEEEEEEEEEEEXXXXXXXXXXXXXXIT"
			m=NoPlayersMessage.new
			@queue.push([0,m])
			#exit
		end
	end
	def pushMessage(c,m)
		#if (not processMessage(c,m))
		id=@connections.find{|k,v|v==c}[0]
		@queue.push([id,m])
		puts "PUSHED MESSAGE #{m}"
		#end
	end
		
private
	def run
		@mthread=Thread.new {
			while (session = @server.accept)
				Thread.start do
					begin
						connection=Connection.new(session,self)
						connection.runConnection
					#rescue e
					#	puts e
					end
				end
			end
		}
	end
	def getFirstConnectionID
		(0..1000).each{|i|
			return i if @connections[i].nil?
		}
		raise "problem - too many connections"
	end
end

