#!/usr/bin/env ruby

require 'ruby/antargis.rb'
require 'server/client.rb'

class AntClient<Client
	attr_accessor :name
	def initialize(user,pw)
		super()
		@name=user
		@pw=pw
		@state=:login
	end
	def processMessage(m)
		case m
			when ChallengeMessage
				sendMessage(LoginMessage.new(@name,myhash(m.challenge+myhash(@name+@pw)),getProtocolVersion))
				puts "try login"
				return true
			when WelcomeMessage
				@state=:loggedin
				super
				return true
			else
				super
		end
		return false
	end

	def state
	end
	def getName
		@name
	end
end

#login
client=AntClient.new("muh","puh")

welcomeMessage=nil
#wait for welcome message
while true
	welcomeMessage=client.getMessage
	if welcomeMessage.is_a?(WelcomeMessage) #(not welcomeMessage.nil?)
		break
	end
	sleep 0.1
end

# decompress level-text
leveltext=uncompress(welcomeMessage.level)
client.name=welcomeMessage.name
File.open("leveltext","w").puts leveltext
# start level
startGame(leveltext,client)


