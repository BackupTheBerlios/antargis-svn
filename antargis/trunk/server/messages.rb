#!/usr/bin/env ruby

require 'message.rb'

class ChallengeMessage<Message
	attr_reader :challenge
	def initialize(c)
		@challenge=c
	end
end

class LoginMessage<Message
	attr_reader :name, :pw
	def initialize(name,hashedpw,version)
		@name=name
		@pw=hashedpw
	end
end

class WelcomeMessage<Message
	attr_reader :level
	def initialize(level)
		@level=level
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

def getProtocolVersion
	1
end


module AntMarshal
	def AntMarshal.dump(x)
		d=Marshal.dump(x)
		len=d.length
		puts "MARSHAL LEN:#{len}"
		[len].pack("N")+d
	end
	def AntMarshal.load(x)
		if x.length<6
			puts "TOO SHORT"
			raise ArgumentError.new
		end
		#x[0..10].each_byte{|b|print "#{b};"}
		#puts
		#x.sub!(/^\10----/,"")
		#x[0..10].each_byte{|b|print "#{b};"}
		#puts
		len=x.unpack("N")[0]
		#puts "#{x.length}>=#{len+4} ??"
		if x.length>=len+4
			rest=x[4..-1]
			begin
				r=Marshal.load(rest)
			rescue e
				puts "ERROR in AntMarshal"
				puts e	
			end
			return r
		end
		raise ArgumentError.new
	end
end