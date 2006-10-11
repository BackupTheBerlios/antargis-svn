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
	def initialize(data)
		@data=data
		@matrix=AGMatrix3.new
		@matrix.set(0,0,1)
		@matrix.set(1,2,1)
		@matrix.set(1,2,1)
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
		@matrix.set(2,0,@matrix.get(2,0)+v[0])
		@matrix.set(2,1,@matrix.get(2,1)+v[1])
	end
end

class LineWidget<AGWidget
	def initialize(p,r)
		super
		@sprites=[]
	end
	def addSprite(s)
		@sprites.push(s)
	end
	def draw(p)
		#puts "draw"
		a=AGVector2.new(10,10)
		b=AGVector2.new(20,20)
		c=AGColor.new(0xFF,0,0)
		p.drawLine(a,b,c)

		@sprites.each{|s|
			s.draw(p,nil,c)
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
		@w=LineWidget.new(nil,getScreen.getRect)
		setMainWidget(@w)
		@ship=Sprite.new(makeNewShip)
		@ship.translate(AGVector2.new(300,100))
		@w.addSprite(@ship)

	end
	def eventFrame(t)
		delay(50)
		@ship.rotate(1)
	end
end

app=MyApp.new
app.run