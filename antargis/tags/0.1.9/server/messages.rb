
# here the current messages are defined
# this is really easy. We let ruby make the work of serializing them to binary for now.
# FIXME: Maybe we'll write a new Marshaller for this, because it has a big overhead

class Message
end

class ChallengeMessage<Message
	attr_reader :challenge, :version
	def initialize(c,version)
		@challenge=c
		@version=version
	end
end

class LoginMessage<Message
	attr_reader :name, :pw, :version
	def initialize(name,hashedpw,version)
		@name=name
		@pw=hashedpw
		@version=version
	end
end

class WelcomeMessage<Message
	attr_reader :level
	def initialize(level)
		@level=level
	end
end

class NewPlayerMessage<Message
	attr_reader :pos, :name
	def initialize(pos,name)
		@pos=pos
		@name=name
	end
end

class ChatMessage<Message
	def initialize(name,text)
		@name=name
		@text=text
	end
end

class TimeMessage<Message
	attr_reader :time
	def initialize(time)
		@time=time
	end
end

class ErrorMessage<Message
	attr_reader :error
	def initialize(e)
		@error=e
	end
end

# Protocol-version
# FIXME: this should be checked somewhere in the protocol
def getProtocolVersion
	1
end


# this is a wrapper for the standard Marshal-functions
# it adds a check for truncated dumps by adding a length-value before the actual string

module AntMarshal
	def AntMarshal.dump(x)
		d=Marshal.dump(x)
		len=d.length
		#puts "MARSHAL LEN:#{len}"
		# add length (4 bytes) in network-byte-order
		[len].pack("N")+d
	end
	def AntMarshal.load(x)
		# check if string is long enough for getting length
		if x.length<6  # add some dummy value (1)
			#puts "TOO SHORT"
			raise ArgumentError.new
		end
		len=x.unpack("N")[0] # get the length
		if x.length>=len+4
			rest=x[4..-1] # get the rest of the string
			begin
				r=Marshal.load(rest) # recreate object
			rescue e
				# some major Problem arised.
				raise "ERROR in AntMarshal"
			end
			return r
		end
		# still to short
		raise ArgumentError.new
	end
end
