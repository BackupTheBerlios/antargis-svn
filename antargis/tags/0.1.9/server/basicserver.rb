
require 'config.rb'
require 'thread.rb'
require 'socket'

#
# These classes provide a threaded TCP-Server that handles messages
#

# A connection is an object handling the connection of a server to the client
class Connection
	attr_accessor :name
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
		x=AntMarshal.dump(m)
		send(x)
	end
	# identifies the other side of the connection
	def type
		:client
	end

private
	def send(x)
		begin
			puts "TRY SENDING"
			@session.puts(x)
			puts "SENT STH"
		rescue
		end
	end
	def run
		while @session
			c=@session.gets #recv(10000000)
			if c.nil?
				break
			end
			@instr+=c
			begin
				m=AntMarshal.load(@instr)
				l=AntMarshal.dump(m).length
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
end


# Server opens an TCP-port on $PORT (defined in config.rb) on the localhost 
class Server
	def initialize
		@port = $PORT.to_i
		@server = TCPServer.new('localhost', @port)
		@connections=[]
		@queue=Queue.new
		run
	end

	# clean shutdown
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
	end

	def sendToAllBut(m,but)
		@connections.each{|c|
			if c!=but
				c.sendMessage(m)
			end
		}
	end

	# deprecated function - see processMessage
	def recvMessage
		begin
			r=@queue.pop
		rescue
		end
		r
	end


	# event-handlers - can be overridden
	def eventNewConnection(c)
	end
	def eventLostConnection(c)
	end


	# processMessage is run in the connection's thread - so beware not to call any BoA thingies
	# FIME: call this via syncCall - this way we've moved all thread-dependent things to this class and don't have to care about this
	#       anywhere else. Have a look at ant_server how to do this. Then we can discard these other queues, too.
	def processMessage(c,m)
	end


	# semi-private functions - should be called by Connection-objects only
	# add new connection
	def addConnection(c)
		@connections.push(c)
		puts "new connection #{c}"
		eventNewConnection(c)
		puts "ok. had new connection"
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
					begin
						connection=Connection.new(session,self)
						connection.runConnection
					rescue e
						puts e
					end
				end
			end
		}
	end
end

