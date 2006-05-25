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
	def initialize(name,hashedpw)
		@name=name
		@pw=hashedpw
	end
end

class WelcomeMessage<Message
	def initialize
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