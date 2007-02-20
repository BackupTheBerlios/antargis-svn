#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

class MyBaseMap
	attr_reader :w,:h
	def initialize(pw,ph)
		@w=pw
		@h=ph
		@a=[]
		(1..@w).each{|x|
			(1..@h).each{|y|
				set(x,y,:undefined)
			}
		}
	end
	def set(x,y,v)
		@a[x+@w*y]=v
	end
	def get(x,y)
		return :undefined unless x>0 and x<=@w and y>0 and y<=@h
		@a[x+@w*y]
	end
	def to_surface
		i=AGSurface.new(w,h)
		
		begin
			p=AGPainter.new(i)
			(1..@w).each{|x|
				(1..@h).each{|y|
					t=get(x,y)
					c=toColor(t)
					p.putPixel(AGVector2.new(x-1,y-1),c)
				}
			}
		end
		i
	end
	def toColor(t)
		case t
			when :undefined
				return AGColor.new(0xFF,0x70,0)
			when :water
				return AGColor.new(0,0,0x70)
			when :green
				return AGColor.new(0,0xFF,0)
			when :wood
				return AGColor.new(0,0x7F,0)
			when :rock
				return AGColor.new(0x7f,0x7F,0x7f)
		end
		return AGColor.new(0,0,0)
	end
end

class MyMap<MyBaseMap
	def initialize(w,h)
		super
		initRandom
	end


	def putAtRandPos(v)
		x=rand*@w.to_i+1
		y=rand*@h.to_i+1
		set(x,y,v)
	end

	def someX(amount,what)
		(1..amount).each{|v|
			putAtRandPos(what)
		}
	end

	def blurAt(x,y)
		cv=get(x,y)
		return nil unless cv==:undefined
		what={}
		(-1..1).each{|dx|
			(-1..1).each{|dy|
				t=get(x+dx,y+dy)
				what[t]||=0
				what[t]+=1
			}
		}
		what[:undefined]=0
		ps=[]
		what.each{|k,v|
			ps.push([k,v])
		}
		ps=ps.shuffle.sort{|a,b|b[1]<=>a[1]}
		#puts ps[0][0]
		ps[0][0]
	end


	def initRandom
		@cs=[]
		(1..@h).each{|y|
			(1..@w).each{|x|
				@cs<<[x,y]
			}
		}
		
		@cs=@cs.shuffle
	end

	def blur
		modified=false
		@cs.each{|p|
			x,y=p
			v=blurAt(x,y)
			if v.nil?
				#modified=true
			elsif v!=:undefined
				set(x,y,v)
				modified=true
			end
		}
		modified
	end

	def blurAll
		modified=true
		tries=0
		while modified and tries<1000
			modified=blur
			tries+=1
		end
	end
	
end
	


w=h=256

myMap=MyMap.new(w,h)
myMap.someX(10,:water)
myMap.someX(10,:green)
myMap.someX(10,:wood)
myMap.someX(10,:rock)
myMap.blurAll
app=AGApplication.new
i=myMap.to_surface
image=AGImage.new(nil,AGRect2.new(0,0,w,h),i,false)
app.setMainWidget(image)

app.run