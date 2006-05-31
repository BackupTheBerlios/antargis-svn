#!/usr/bin/env ruby

require 'basicserver.rb'
require 'messages.rb'
require 'md5.rb'

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


def checkLogin(m)
	m.name=="puh" && m.pw==myhash("puhmuh")
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
		puts "new connection"
		# do nothing
		puts c
		seed=myhash(rand.to_s)
		@challenges[c]=seed
		cm=ChallengeMessage.new(seed)
		c.sendMessage(cm)
		puts "ok conn"
	end
	def processMessage(c,m)
		case m
			when LoginMessage
				if @loginTable.check(m.name,m.pw,@challenges[c])
					puts "LOGIN OK"
					puts "muh"
					puts @app
					@app.syncCall { puts "SYNC" ; m=@app.makeWelcomeMessage(m.name,c) ; c.sendMessage(m)}
					
					#puts m
					#puts "savegame sent"
				else
					puts "LOGIN WRONG"
					c.sendMessage(ErrorMessage.new("Wrong Login"))
					c.close
				end
				return true
			else
				puts "elsse?????????"
		end
		return false
	end
	def makeWelcomeMessage
		puts "SHIT"
		puts "makeW"
		d=Document.new
		d.root.setName("antargisLevel")
		@map.saveXML(d)
		puts "toxml"
		m=WelcomeMessage.new("") #d.toString)
		puts "ok"
		return m
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
