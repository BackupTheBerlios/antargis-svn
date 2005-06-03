require 'libantargisruby'
include Libantargisruby

$cardHeight=140
$cardWidth=80

class Card
	def initialize(pName)
		@name=pName
	end
	def name
		@name
	end
end

class Vector
	def initialize(point,y=nil)
		if y==nil
			@x=point.x.to_f
			@y=point.y.to_f
		else
			@x=point
			@y=y
		end
	end
	def to_s
		"("+@x.to_s+","+@y.to_s+")"
	end
	def toAGPoint
		AGPoint.new(@x.to_i,@y.to_i)
	end
	def norm
		Math.sqrt(@x*@x+@y*@y)
	end
	def normalize!
		n=norm
		if n
			@x/=n
			@y/=n
		end
	end
	def normalized
		n=norm
		Vector.new(@x/n,@y/n)
	end
	def x
		@x
	end
	def y
		@y
	end
	def +(f)
		Vector.new(x+f.x,y+f.y)
	end
	def -(f)
		Vector.new(x-f.x,y-f.y)
	end
	def *(f)
		Vector.new(x*f,y*f)
	end
	def /(f)
		Vector.new(x/f,y/f)
	end
end

class CardView<AGRadio
	def initialize(card,pos)
		super($skatview,AGRect.new(pos.x,pos.y,$cardWidth,$cardHeight))
		setCaption(card.name)
		@ready=true
		setTheme("antButton")
		@card=card
		@speed=1000
		@pos=Vector.new(pos)
		setEnabled(false)
		@wait=0
		@shallWait=true
	end
	def card
		@card
	end
	def move(time)
		if @wait>0
			@wait-=time
			if @wait<0
				@wait=0
				@ready=true
			end
		end
			
		if @wait==0 and not @ready
			setState(NORMAL)
			#puts "MOVING:"+@targetPos.to_s
			pos=@pos
			norm=(@targetPos-pos).norm
			if @speed*time>norm
				newPos=@targetPos
				if @shallWait
					@shallWait=false
					@wait=0.5
				else
					@ready=true
				end
			else
				newPos=pos+(@targetPos-pos).normalized*@speed*time
			end
			@pos=newPos
			r=getRect
			setLeft(newPos.x.to_i)
			setTop(newPos.y.to_i)
		end
	end
	def ready
		#puts "READY:"+@ready.to_s
		@ready
	end
	def moveTo(pos,wait=false)
		@targetPos=Vector.new(pos)
		@ready=false
		@shallWait=wait
		puts "MOVETO:"+pos.to_s
	end
	def setPlayable(value)
		setEnabled(value)
	end
	
	def eventMouseClick(e)
		wasChecked=false
		if isChecked
			wasChecked=true
		end
		super(e) # must be called for several reasons
		if wasChecked
			setChecked(false)
			$view.cardClick(self)
		end
	end
end

class TableMiddle<AGImage
	def initialize
		surface=getSurfaceManager.loadSurface("table_middle.png")
		super($skatview,AGPoint.new(300,200),surface,false)
	end
end

class SkatView<AGWidget
	include AGHandler
	def initialize(p,rect)
		super(p,rect)
		$skatview=self
		@cards=[]
		@middle=[]
		@human=[]
		@handler=[]
		@todel=[]
		
		addTable
		c=Card.new("Pik 7")
		addCardC0(c)
		playCard(c)
	end
	
	def playCard(card)
		@cards.each{|c|
			if c.card==card then
				c.moveTo(getMiddle,true)
				@middle.push(c)
				@human.delete(c)
			end
		}
	end
	
	# cards go to player = (human|computer0|computer1)
	def awayCards(player)
		if player=="human"
			pos=Vector.new(512,600)
		elsif player=="computer0"
			pos=Vector.new(-100,0)
		else
			pos=Vector.new(1024,0)
		end
			
		@middle.each{|c|
			c.moveTo(pos)
			@todel.push(c)
		}
		@middle.clear
		
	end
	
	def middle
		cards=[]
		@middle.each{|c|
			cards.push(c.card)
		}
		return cards
	end
	
	def beginHumanTurn
		@human.each{|cw| cw.setPlayable(true)}
		@humanTurn=true
	end
	def endHumanTurn
		@human.each{|cw| cw.setPlayable(false)}
	end
	
	def addCardHuman(card)
		px=@human.size
		cardWidget=CardView.new(card,AGPoint.new(px*($cardWidth+5)+95,550))
		addChild(cardWidget)
		@cards.push(cardWidget)
		@human.push(cardWidget)
	end
	
	def addCardC0(card)
		card=CardView.new(card,AGPoint.new(0,0))
		addChild(card)
		@cards.push(card)
	end
	

	def move(time)
		ready=true
		@cards.each{|c|
			c.move(time)
			if not c.ready
				ready=false
			end
		}
		if @humanTurn
			ready=false
		end
		if ready
			sigReady
		end
	end
	
	def addHandler(readyH)
		@handler.push(readyH)
	end
	
	def sigReady
		cleanup
		puts "SIGREADY"
		@handler.each{|h|h.sigReady}
		#awayCards("human")
	end
	
	def cardClick(cardW)
		@humanTurn=false
		@handler.each{|h|
			h.cardClicked(cardW.card)
		}
	end
	
	
	private
	def addTable
		m=TableMiddle.new
		@middleWidget=m
		addChild(m)
	end
	
	#
	#def addCards
	#	card=CardView.new(Card.new("Pik As"),AGPoint.new(100,100))
	#	addChild(card)
		#@cards.push(card)
		#
	#	card.moveTo(getMiddle)
	#end
	
	def cleanup
		@todel.each{|c|
			removeChild(c)
		}
		@todel.clear
	end
	
	
	def getMiddle
		r=@middleWidget.getRect
		w=r.width
		h=r.height
		mx=r.getX+w/2
		my=r.getY+h/2
		
		count=@middle.size
		if count==0
			mx-=w/3
		elsif count==2
			mx+=w/3
		end
		mx-=$cardWidth/2
		my-=$cardHeight/2
		return Vector.new(mx,my)
	end
	
end