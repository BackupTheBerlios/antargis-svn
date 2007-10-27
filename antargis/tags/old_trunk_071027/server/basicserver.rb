#!/usr/bin/env ruby

# These classes provide a threaded TCP-Server that can send and receive Messages (look messages.rb).
# The goal is, that the main-program-thread simply checks for messages from time to time and responds
# as it likes. So _NO_ threads apart from the main-thread should be used outside of this file!
# NOTE: BoA currently has NO support for threading in it (neither the GUI-lib nor the entity-managing)
#
# The usage of the Server-class goes something like this:
# server=Server.new   # PORT is defined in config.rb
# while true
#   sender,message=server.recvMessage
#   if message
#     # do some processing and respond to this client
#     server.sendMessage(sender,newMessage)
#   end
#   # maybe send some more Message to everybody
#   server.sendMessageToAll(newMessage2)
#   # and copy the received message to everybody else
#   server.sendMessageToAllBut(message,sender)
#

require 'config.rb'
require 'thread.rb'
require 'socket'


# The Connection-class should NOT be used outside of this file!
# A connection is an object handling the connection of a server to the client - 
class Connection
	attr_accessor :name, :id

	# make new Connection with the connection "session" and the parent server-object "server"
	def initialize(session,server)
		@session=session
		@server=server
		# set queue to empty string
		@instr=""
	end

	# this should be run by the server-object only
	# it adds this connection to the parent server and then runs a thread, which listens to the tcp-connection
	def runConnection
		@server.addConnection(self)
		run
		@server.removeConnection(self)
	end

	# send some message to the client
	def sendMessage(m)
		# serialize the message using specialized AntMarshal-class (messages.rb)
		x=AntMarshal.dump(m)
		# send it
		send(x)
	end

	# identifies the other side of the connection
	def type
		:client
	end

private
	# tries to send the message and removes connection from parent server, if this fails
	# (pipe broken or connection reset)
	def send(x)
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
	end

	# runs a thread to listen on the given connection
	def run
		begin
			while @session
				c=@session.gets
				if c.nil? or c==""
					# break if connection didn't give us any reasonable result
					break
				end
				@instr+=c # add new content to string-queue
				begin
					# try to 
					m=AntMarshal.load(@instr)
					l=AntMarshal.dump(m).length
					@instr=@instr[(l+2)..-1] # ignore 2 bytes for the "newline" in the input-queue
					# enqueue parsed message
					@server.pushMessage(self,m)
				rescue TypeError => e
					# this is a major problem !
					# something in the input-string is horribly wrong!
					@instr=@instr[1..-1]
					puts "crappy data"
					puts e
					raise
				rescue ArgumentError => e
					# ignore - short of data
				end
				if @instr.nil?
					@instr=""
				end
				# give us some impression on how long the input queue is.
				puts "INSTR: #{@instr.length}"
			end
		rescue Errno::EPIPE => e
			@server.removeConnection(self)
		rescue Errno::ECONNRESET => e
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
		@connections={} # holds the connections by id: {0=>#Connection1, 2=>Connection6}
		@queue=Queue.new
		run
	end

	# this can do a clean shutdown
	def join
		@mthread.join
	end

	def sendMessage(id,m)
		c=@connections[id]
		if c
			c.sendMessage(m)
		end
	end

	# sends the given message to all clients
	def sendMessageToAll(m)
		@connections.each{|k,c|
			c.sendMessage(m)
		}
	end

	# sends the given message to all clients but not to "but"
	def sendToAllBut(m,but)
		raise 1 if but.is_a?(Connection)
		@connections.each{|k,c|
			puts "sendToAllBut(m,but) #{k} #{c}"
			if k!=but
				c.sendMessage(m)
			end
		}
	end

	# this tries to pop a message from the input queue, nil is returned on failure
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
			# FIXME: eventually exit server if all clients disconnected ???
			# however we send a NoPlayersMessage to the server, so it can decide what's to do
			m=NoPlayersMessage.new
			@queue.push([0,m])
		end
	end

	# enqueue message m from connection c
	def pushMessage(c,m)
		id=@connections.find{|k,v|v==c}[0]
		@queue.push([id,m])
	end
		
private
	# runs a listening thread
	def run
		@mthread=Thread.new {
			while (session = @server.accept)
				# start new thread for connection-listening
				Thread.start do
					begin
						connection=Connection.new(session,self)
						connection.runConnection
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

