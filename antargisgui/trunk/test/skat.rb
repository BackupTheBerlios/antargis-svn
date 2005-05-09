#!/usr/bin/env ruby
#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# layout.rb
# by David Kamphausen (david.kamphausen@web.de)
#
# The "Antargis" project, including all files needed to compile it,
# is free software; you can redistribute it and/or use it and/or modify it
# under the terms of the GNU General Public License as published
# by the Free Software Foundation; either version 2 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# You should have received a copy of the GNU General Public
# License along with this program.
#

#!/usr/bin/ruby

require 'libantargisruby'
require 'antApp.rb'

include Libantargisruby

$trump="Karo"

# Erstmal ohne:
# - Ramsch
# - Null
# - Buben Trumpf - wie heißt das nochmal???
# - Testen ob richtig bedient

class Array
	def shuffle
		sort{0.5 <=> rand}
	end
end 


# ESTIMATION OF BEST CARD
def valueOf(card)
	a=card.split(" ")
	return a[1]
end

def colorOf(card)
	card.split(" ")[0]
end

def valueToIndex(value)
	werte=["7","8","9","10","Bube","Dame","König","As"]
	return werte.index(value)
end

def colorToIndex(value)
	werte=["Herz","Karo","Pik","Kreuz"]
	return werte.index(value)
end

def compare(c0,c1)
	return valueToIndex(valueOf(c0))<=>valueToIndex(valueOf(c1))
end

# first card was given first - so says which color to play
def sortOutColors(cards)
	cp=colorOf(cards[0])
	cn=[]
	cards.each{|c|
		mc=colorOf(c)
		if mc==cp or mc==$trump then
			cn.push c
		end
	}
	return cn
end

def isTrump(c)
	return (valueOf(c)=="Bube" or colorOf(c)==$trump)
end

def compareTrumps(c0,c1)
	if valueOf(c0)=="Bube" then
		if valueOf(c1)=="Bube" then
			return colorToIndex(colorOf(c0))<=>colorToIndex(colorOf(c1))
		else
			return 1
		end
	elsif valueOf(c1)=="Bube" then
		return -1
	else
		return valueToIndex(valueOf(c0))<=>valueToIndex(valueOf(c1))
	end
end

def getTrumps(cards)
	trumps=[]
	cards.each{|c|
		if isTrump(c) then
			trumps.push(c)
		end
	}
	return trumps
end

def bestTrump(cards)
	trumps=getTrumps(cards)
	if trumps.length>0 then
		trumps.sort! {|a,b|compareTrumps(a,b)}
		trumps.reverse!
		return trumps[0]
	end
	return nil
end
# TIL HERE

# input : array of card-strings
# output : card-string of best card
def bestOf(cards)
	cn=sortOutColors(cards)
	bestT=bestTrump(cards)
	if bestT then
		return bestT
	end
	
	cards.sort {|a,b|compare a,b}
	cards.reverse!
	puts "SORTED:"+cards.to_s
	
	return cards[0]
end

class Player
	def setCards(cards)
		@cards=cards
	end
	def giveFirst()
		@cards[0]
	end
	# input already given cards # FIXME: later with player-connection
	def give(cards)
		@cards[0]
	end
end


def test
	cards=["Herz 7","Herz Dame","Karo 9"]
	puts cards
	puts "BEST:"+bestOf(cards)
end
 
class TestApp <AntApp
	def initialize(autoexit=true)
		@count=0
		@autoexit=autoexit
		super()
		
		$screen=AGScreenWidget.new
	
		l=AGLayout.new($screen,loadFile("skat.xml"))
		$screen.addChild(l)
	
		setMainWidget($screen)

		
		initGame
	end
	def eventQuit(event)
		puts "Quitting"
		super(event)
	end
	def eventIdle
		#puts "idle count:"+@count.to_s
		@count+=1
		if @count>=20 then
			if @autoexit then
				puts "correct quit after 20 idles"
				tryQuit
			end
		end
	end
	
	def initGame
		ns=["7","8","9","10","Bube","Dame","König","As"]
		fs=["Herz","Karo","Pik","Kreuz"]
		@cards=[]
		ns.each{|n|
			fs.each{|f|
				@cards.push(f+" "+n)
			}
		}
		@cards.each{|c|puts c}
		puts "SHUFFLE"
		@cards=@cards.shuffle
		
		@cards.each{|c|puts c}
		
		@players=[]
		
		
		$trump="Karo"
		giveAll
		initCardView
		@game=[]
	end
	
	
	def initCardView
		@cardView=[]
		@gameView=[]
		for i in 0..9 do
			@cardView.push toAGButton($screen.getChild("c"+i.to_s))
		end
		@gameView.push toAGButton($screen.getChild("give0"))
		@gameView.push toAGButton($screen.getChild("give1"))
		@gameView.push toAGButton($screen.getChild("give2"))
		for i in 0..9 do
			@cardView[i].setCaption @players[0][i]
		end
		addHandler($screen.getChild("p0"),:sigClick,:player0give)
		addHandler($screen.getChild("p1"),:sigClick,:player1give)
	end
	
	def giveAll
		@players[0]=[]
		@players[1]=[]
		@players[2]=[]
		
		i=0
		@cards.each{|c|
			@players[i].push(c)
			i=i+1
			i=i%3
		}
		
		@players[0].sort!
		@players[1].sort!
		@players[2].sort!
	end
	
	def player0give
		playerGive(1)
	end
	def player1give
		playerGive(2)
	end
	
	def playerGive(number)
		puts "PLAYER0GIVE"
		p=Player.new
		p.setCards(@players[number])
		if @game.length
			c=p.giveFirst
		else
			c=p.give(@game)
		end
		@players[number].delete(c)
		@gameView[@game.length].setCaption c
		@game.push c
		
		checkFull
	end
	def checkFull
		if @game.length==3 then
			@game=[]
			@gameView[0].setCaption ""
			@gameView[1].setCaption ""
			@gameView[2].setCaption ""
		end
	end
end

def runGame
	puts "MenuTest"
	
	main=AGMain.new
	
	loadTheme(loadFile("theme.xml"))
	
	main.changeRes(1024,768,32,false,true)
	
	app=TestApp.new(false)
	
	app.run
end



runGame

#test