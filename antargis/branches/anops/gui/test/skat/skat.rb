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
include Libantargisruby
require 'antApp.rb'

require 'skatview.rb'

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
	def shuffle!
		sort!{0.5 <=> rand}
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
		@cards=@cards.shuffle!
		
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

###################################################################

class CompPlayer
	def initialize(number)
		@number=number
		@cards=[]
	end
	# middle is an array of already played cards (in order of appearance)
	def play(middle)
		if middle.size==0
			# play out card
			playOutCard
		else
			react(middle)
		end
	end
	
	def addCard(card)
		@cards.push(card)
	end
	
	def playOutCard
		card=@cards[0]
		@cards.delete(card)
		playCard(card)
	end
	
	def react(middle)
		# FIXME: add some AI here - check if plays alone and so
		playedColor=getPlayedColor(middle)
		playableCards=getPlayableCards(middle)
		
		if playableCards.size>0
			# take biggest
			playableCards.sort!
			playableCards.reverse!
			puts "PLAYABLE:"
			puts playableCards
			c=playableCards[0]
		else
			# take anything
			c=@cards[0]
		end
		playCard(c)
		@cards.delete(c)
	end
	
	def getPlayedColor(middle)
		puts "GETPLAYEDCOOR"
		puts middle[0].value.value
		if middle[0].value.value!="Bube"
			return middle[0].color
		else
			return $game.trump
		end
	end
		
	
	def getPlayableCards(middle)
		if middle.size==0
			return @cards
		end
		playedColor=middle[0].color
		cs=getCardsWithColor(playedColor)
		if playedColor==$game.trump
			cs+=getBuben()
		end
		return cs
	end
	
	def getCardsWithColor(color)
		cs=[]
		@cards.each{|c|
			if c.color==color and c.value.value!="Bube"
				cs.push(c)
			end
		}
		return cs
	end
	def getBuben
		cs=[]
		@cards.each{|c|
			if c.value.value=="Bube"
				cs.push(c)
			end
		}
		return cs
	end
	
	def playCard(card)
		$skatview.addCardComputer(card,@number)
		$skatview.playCard(card)
		$game.playCard("computer"+@number.to_s,card)
	end
end

class Game
	def initialize(view)
		@view=view
		$game=self
		view.addHandler(self)
		@turn="computer1"
		@player0=CompPlayer.new(0)
		@player1=CompPlayer.new(1)
		@trump=CardColor.new("Karo")
		genCards
		giveCards
		@played=[]
	end
	
	def genCards
		@cards=[]
		for color in ["Kreuz","Pik","Herz","Karo"]
			for value in ["7","8","9","Bube","Dame","König","10","As"]
				@cards.push(Card.new(color+" "+value))
			end
		end
		@cards.shuffle!
	end
	
	def giveCards
		@cards.shuffle!
		humanCards=[]
		for i in 0..29 do
			c=@cards[i]
			puts c
			if i<10
				# first pc0
				@player0.addCard(c)
			elsif i<20
				@player1.addCard(c)
			else
				humanCards.push(c)
			end
		end
		humanCards.sort.each{|c|
			@view.addCardHuman c
		}
	end
	
	def trump
		@trump
	end
	
	def cardClicked(card)
		@view.playCard(card)
	end
	
	def assignTurn
		puts "assignTurn:"+@turn
		if @turn=="human"
			@view.beginHumanTurn
		elsif @turn=="computer0"
			@player0.play(@view.middle)
		elsif @turn=="computer1"
			@player1.play(@view.middle)
		end
	end
	
	def playCard(player,card)
		@played.push([player,card])
	end
	
	def sigReady
		if @view.middle.size<3
			assignTurn
			@turn=getNext
		else
			checkMiddle
		end
	end
	
	def checkMiddle
		winner=@played[0][0]
		bestCard=@played[0][1]
		playedColor=bestCard.color
		@played[1..2].each{|pair|
			p=pair[0]
			c=pair[1]
			puts "BETTER THAN:"
			puts "BEST:"+bestCard.name
			puts "NOW:"+c.name
			if c.betterThan(bestCard,playedColor,@trump)
				puts "BETTER!!!"
				winner=p
				bestCard=c
			else
				puts "LESSS!"
			end
		}
		@played=[]
		@view.awayCards(winner)
		@turn=winner
	end
	
	private
	def getNext
		if @turn=="human"
			return "computer0"
		elsif @turn=="computer0"
			return "computer1"
		else
			return "human"
		end
	end
end


class NewApp<AntApp
	def initialize
		super()
		$view=SkatView.new(nil,AGRect.new(0,0,1024,768))
		setMainWidget($view)
		$game=Game.new($view)
	end
	def eventIdle
		delay(20)
	end
	def eventFrame(time)
		$skatview.move(time)
	end
end

def runGame
	puts "MenuTest"
	
	main=AGMain.new
	
	loadTheme(loadFile("theme.xml"))
	
	main.changeRes(1024,768,32,false,true)
	
	app=NewApp.new()
	
	app.run
end



runGame

#test
