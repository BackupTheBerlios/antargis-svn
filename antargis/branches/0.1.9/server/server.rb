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
	def eventNewConnection(c)
		super
		puts "---------------------------------"
		puts "new connection"
		puts "---------------------------------"
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
			when LoginMessage
				if @loginTable.check(m.name,m.pw,@challenges[c])
					puts "LOGIN OK"
					puts "doin synccall"
					puts @app
					puts @app.methods.sort.join(" ; ")
					puts "doin synccall"
					con=c
					@app.syncCall { @app.eventNewPlayer(m.name,con) }
					puts "doin synccall!"
				else
					puts "LOGIN WRONG"
					c.sendMessage(ErrorMessage.new("Wrong Login"))
					c.close
				end
				return true
			else
				puts "Unknown message type."
		end
		return false
	end
end

def serverTest
	table=SimpleLoginTable.new
	table.addPlaintext("muh","puh")
	server=LoginServer.new(table)
	
	for i in 0..100
		t=TimeMessage.new(i)
		server.sendMessage(t)
		sleep 0.5
	end
	
	server.join
end


#serverTest
