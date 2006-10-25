#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

class SpriteData<Array
end

def makeRotation(angle)
	a=AGMatrix3.new
	angle*=3.1415/180
	a.set(0,0,Math::cos(angle))
	a.set(1,0,Math::sin(angle))
	a.set(0,1,-Math::sin(angle))
	a.set(1,1,Math::cos(angle))
	a.set(2,2,1)
	a
end

class Sprite
	attr_reader :translation, :angle
	def initialize(data)
		@data=data
		@matrix=AGMatrix3.new
		@matrix.set(0,0,1)
		@matrix.set(1,2,1)
		@matrix.set(1,2,1)

		@dir=AGVector2.new(0,-1)
		@translation=AGVector2.new(0,0)
		@angle=0
	end
	def draw(painter,matrix,c)
		c=c.toVec
		c0=AGVector4.new(c.x,c.y,c.z,0.53)
		c1=AGVector4.new(c.x,c.y,c.z,0.8)
		c2=AGVector4.new(c.x,c.y,c.z,1)
		c0=AGColor.new(c0)
		c1=AGColor.new(c1)
		c2=AGColor.new(c2)

 		c2=AGColor.new(255,255,255,0xFF)

		mydraw(painter,matrix,c0,6)
		mydraw(painter,matrix,c1,3)
		mydraw(painter,matrix,c2,1)
	end		
	def mydraw(painter,matrix,color,l)
		target=painter.getTarget
		#puts target.methods.join(":::")
		target.setLineWidth(l)
		(0..(@data.length-2)).each{|i|
			a=@data[i]
			b=@data[i+1]
			a=@matrix*a
			b=@matrix*b
			painter.drawLine(a.dim2,b.dim2,color)
		}
	end	
	def rotate(angle)
		@matrix=@matrix*makeRotation(angle)
	end
	def translate(v)
		@translation=v
		@matrix.set(2,0,v[0])
		@matrix.set(2,1,v[1])
	end


	def setDirection(angle)
		@angle=angle
		@dir=AGVector2.new(AGAngle.new(-angle*3.1415/180.0))
		@matrix=makeRotation(angle)
		translate(@translation)
	end
	def moveForward(amount)
		translate(@translation+@dir*amount)
	end
end

class GameWorld<Array
end

class GameObject
	def initialize(world)
		@world=world
	end	
	def discard
		@world.delete(self)
	end
	def move(t)
	end
end

class Shot<GameObject
	attr_reader :sprite
	@@max_time=1
	def initialize(widget,dir,t)
		super(widget)
		@speed=300
		@sprite=Sprite.new(makeSprite)
		@sprite.setDirection(dir)
		@sprite.translate(t)
		
		@time=0	
	end

	def move(t)
		@sprite.moveForward(t*@speed)
		@time+=t
		if @time>@@max_time
			#puts "DISCRAD"
			discard
		end
	end

	def makeSprite
		d=SpriteData.new
		d.push(AGVector3.new(0,-5,1))
		d.push(AGVector3.new(-5,0,1))
		d.push(AGVector3.new(0,10,1))
		d.push(AGVector3.new(5,0,1))
		d.push(AGVector3.new(0,-5,1))
		d		
	end
end

class Ship<GameObject
	attr_reader :sprite
	def initialize(world)
		super
		@sprite=Sprite.new(makeNewShip)
	end
end

class LineWidget<AGWidget
	def initialize(p,r,world)
		super(p,r)
		@world=world
	end
	def draw(p)
		a=AGVector2.new(10,10)
		b=AGVector2.new(20,20)
		c=AGColor.new(0xFF,0,0)
		p.drawLine(a,b,c)

		@world.each{|s|
			s.sprite.draw(p,nil,c)
		}

	end
end


def makeShip
	d=SpriteData.new
	d.push(AGVector3.new(0,0,1))
	d.push(AGVector3.new(30,-10,1))
	d.push(AGVector3.new(20,0,1))
	d.push(AGVector3.new(30,10,1))
	d.push(AGVector3.new(0,0,1))
end

def makeNewShip
	d=SpriteData.new
	d.push(AGVector3.new(0,-10,1))
	d.push(AGVector3.new(10,0,1))
	d.push(AGVector3.new(10,-20,1))
	d.push(AGVector3.new(20,0,1))
	d.push(AGVector3.new(10,10,1))
	d.push(AGVector3.new(-10,10,1))
	d.push(AGVector3.new(-20,0,1))
	d.push(AGVector3.new(-10,-20,1))
	d.push(AGVector3.new(-10,0,1))
	d.push(AGVector3.new(0,-10,1))
end

class MyApp<AGApplication
	def initialize
		super
		@world=GameWorld.new
		@w=LineWidget.new(nil,getScreen.getRect,@world)
		setMainWidget(@w)
		#@ship=Sprite.new(makeNewShip)
		#@ship.translate(AGVector2.new(300,100))
		@ship=Ship.new(@world)
		@world.push(@ship) #ShipaddSprite(@ship)
		@keys=[]
		@speed=180
		@aspeed=250

		setKeyRepeat(false)
	end
	def eventFrame(t)
		manageKeys(t)
		manageMovement(t)
		#delay(10)
		GC.start
	end

	def manageKeys(t)
		firstSet=[Libantargis::SDLK_UP,Libantargis::SDLK_RIGHT,Libantargis::SDLK_DOWN,Libantargis::SDLK_LEFT]
		secondSet=[Libantargis::SDLK_w,Libantargis::SDLK_d,Libantargis::SDLK_s,Libantargis::SDLK_a]
		@keypressed=false
		if (firstSet&@keys).length>0
			dir=-1
			[0,2,4,6,1,3,5,7].each{|i|
				if ((i % 2)!=0)
					if @keys.member?(firstSet[i/2]) and @keys.member?(firstSet[(i/2+1) % 4])
						dir=i
					end
				else
					if @keys.member?(firstSet[i/2])
						dir=i
					end
				end
			}
			#puts dir
			@ship.sprite.setDirection(-dir*360.0/8)
			@keypressed=true
		end
		if (secondSet & @keys).length>0 #?(Libantargis::SDLK_SPACE)
			puts "SHOT"

			dir=-1
			[0,2,4,6,1,3,5,7].each{|i|
				if ((i % 2)!=0)
					if @keys.member?(secondSet[i/2]) and @keys.member?(secondSet[(i/2+1) % 4])
						dir=i
					end
				else
					if @keys.member?(secondSet[i/2])
						dir=i
					end
				end
			}


			o=Shot.new(@world,-dir*360.0/8,@ship.sprite.translation)
	
			@world.push(o)
			#@w.addSprite(o.sprite)
		end
	end

	def manageMovement(t)
		if @keypressed
			@ship.sprite.moveForward(t*@speed)
		end
		@world.each{|o|o.move(t)}
	end

	def eventKeyDown(event)
		puts "DOWN"
		@keys.push(event.getKey)
		@keys.sort!
		@keys.uniq!
		puts @keys.join(" ")
		return super
	end
	

	def eventKeyUp(event)
		@keys.delete(event.getKey)
		@keys.sort!
		@keys.uniq!
		#puts @keys.join(" ")
		return super
	end

end

app=MyApp.new
app.run