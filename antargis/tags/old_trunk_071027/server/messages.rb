
# here the current messages are defined
# this is really easy. We let ruby make the work of serializing them to binary for now.
# FIXME: Maybe we'll write a new Marshaller for this, because it has a big overhead

class Message
end

class NewConnectionMessage<Message
	attr_reader :id
	def initialize(id)
		@id=id
		puts "NewConnectionMessage<Message"
	end
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
	attr_reader :level,:name
	def initialize(level,name)
		@level=level
		@name=name
		#puts level
		#exit
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

class MoveMessage<Message
	attr_reader :hero,:pos,:dist
	def initialize(h,p,d)
		@hero=h
		@pos=p
		@dist=d
	end
end

class FightMessage<Message
	attr_reader :hero,:target,:defend
	def initialize(h,t,d)
		@hero=h
		@target=t
		@defend=d
	end
end

class RestMessage<Message
	attr_reader :hero,:time
	def initialize(h,t)
		@hero=h
		raise "uid is nil!" if @hero.nil?
		@time=t
	end
end

class NoPlayersMessage<Message
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
		puts "try marshalling #{x}"
		d=Marshal.dump(x)
		len=d.length

		puts "ANtMarshal.dump            LEN:#{len}"

		puts d[0].to_s+" "+d[1].to_s+" "+d[2].to_s

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
		puts "ANtMarshal.load            LEN:#{len} #{x.length-4}"

		if x.length>=len+4
			rest=x[4..-1] # get the rest of the string
			d=rest
			puts d[0].to_s+" "+d[1].to_s+" "+d[2].to_s

			begin
				r=Marshal.load(rest) # recreate object
			#rescue e
				# some major Problem arised.
			#	raise "ERROR in AntMarshal"
			end
			return r
		end
		# still to short
		raise ArgumentError.new
	end
end
