#!/usr/bin/env ruby

require 'basicserver.rb'
require 'messages.rb'
require 'md5.rb'

#
# This classes implement a very simple login-process (problems with man-in-the-middle??)
# 1) On a new connection, the server sends a challenge-seed to the client
# 2) the client computes hash(seed+hash(username+pw)) ands sends this with the username
# 3) server has stored username and hash(username+pw) and can this way check if everything's ok


class SimpleLoginTable
	def initialize
		@logins={}
	end
	def addPlaintext(user,pw)
		add(user,myhash(user+pw))
	end
	def add(user,pw)
		@logins[user]=pw
	end
	def check(user,pw,seed)
		puts "#{pw}==#{myhash(seed+@logins[user])}"
		pw==myhash(seed+@logins[user])
	end
end

class LoginServer<Server
	def initialize(loginTable,app)
		super()
		@loginTable=loginTable
		@challenges={}
		@app=app
	end

	def processMessages(map)
		while (r=recvMessage)
			c,m=r
			processMessage(c,m)
			map.processMessage(m)
		end
	end
	def eventNewConnectionOld(c)
		super
		# do nothing
		seed=myhash(rand.to_s)
		puts "SEED #{seed}"
		@challenges[c]=seed
		cm=ChallengeMessage.new(seed,$VERSION)
		puts "cm created"
		c.sendMessage(cm)
	end

	def processMessage(c,m)
		case m
			when NewConnectionMessage
				seed=myhash(rand.to_s)
				@challenges[c]=seed
				cm=ChallengeMessage.new(seed,$VERSION)
				sendMessage(c,cm)
			when NoPlayersMessage
				puts "NO PLAYERS LEFT!"
			when LoginMessage
				if @loginTable.check(m.name,m.pw,@challenges[c])
					con=c
					@app.eventNewPlayer(m.name,con)
				else
					puts "LOGIN WRONG"
					c.sendMessage(ErrorMessage.new("Wrong Login"))
					c.close
				end
				return true
			else
				sendToAllBut(m,c)
		end
		return false
	end
end
