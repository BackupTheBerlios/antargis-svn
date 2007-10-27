#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

class Array
	def cross(p)
		n=[]
		self.each{|x|
			p.each{|y|
				n<<=[x,y]
			}
		}
		n
	end
end


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
					c=getColor(x,y)
					p.putPixel(AGVector2.new(x-1,y-1),c)
				}
			}
		end
		i
	end
	def getColor(x,y)
		t=get(x,y)
		c=toColor(t)
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

class MyHeightMap<MyMap
	def initialize(pw,ph)
		super
		@hs=[]
		@hschange=[]
		@dist=[]
		@ndist=[]
		(1..@w).each{|x|
			(1..@h).each{|y|
				setHeight(x,y,0)
			}
		}
		applyHeightChanges
	end
	def setHeight(x,y,v)
		@hschange[x+@w*y]=v
	end
	def applyHeightChanges
		@hs=@hschange.clone
	end
	def getHeight(x,y)
		return 0 unless x>0 and x<=@w and y>0 and y<=@h
		@hs[x+@w*y]
	end


	def computeHeights(type)
		puts "computing height #{type}"
		(1..@w).each{|x|
			(1..@h).each{|y|
				addRandomHeight(x,y,type)
			}
		}
		applyHeightChanges
	end

	def blurHeights(w)
		a=filterMatrix(w)
# 		a=[1,2,1]
# 		case w
# 			when 5
# 				a=[1,6,15,20,15,6,1]
# 			when 4
# 				a=[1,5,10,10,5,1]
# 			when 3
# 				a=[1,4,6,4,1]
# 			when 2
# 				a=[1,3,3,1]
# 		end
		n=crossProductVecVec(a,a)
		arr=toIndexMap(n)

		f=arr.inject(0){|a,b|a+b[2]}
	
		(1..@w).each{|x|
			(1..@h).each{|y|
				v=0
				arr.each{|p|
					v+=getHeight(x+p[0],y+p[1])*p[2]
				}
				v/=f
					
				setHeight(x,y,v)
			}
		}
		applyHeightChanges
	end

	def addRandomHeight(x,y,type)
		v=get(x,y)
		r=range(v,type)
		h=r[0]+(r[1]-r[0])*rand
		setHeight(x,y,getHeight(x,y)+h)
	end

	def getShadow(x,y)
		a=[[-1,0,-1],
				[0,-1,-1],
				[1,0,1],
				[0,1,1]]
		v=0
		a.each{|t|
			v+=getHeight(x+t[0],y+t[1])*t[2]
		}
		v=[-0.5,v,0.5].sort[1]
		#puts v
		0.5+v
	end

	def getColor(x,y)
		#AGColor.new(0xFF,0xFF,0xFF)*getShadow(x,y)
		super*getShadow(x,y)
	end


	def range(v,type)
		case type
			when :veryrough
				case v
					when :undefined
						return [0,0]
					when :green
						return [0.3,6]
					when :water
						return [0,1]
					when :wood
						return [0.3,8]
					when :rock
						return [0.4,15]
				end
			when :rough
				case v
					when :undefined
						return [0,0]
					when :green
						return [0.3,0.6]
					when :water
						return [0,0.1]
					when :wood
						return [0.3,0.8]
					when :rock
						return [0.4,1.5]
				end
			else
				case v
					when :undefined
						return [0,0]
					when :green
						return [0,0.2]
					when :water
						return [0,0.1]
					when :wood
						return [0,0.1]
					when :rock
						return [0,0.4]
					else
						return [0,0]
				end
		end
	end

	def rockIncreaseAbove(level,types,inc)
		#a=[1,6,15,20,15,6,1]
		a=[1,5,10,10,5,1]
		n=crossProductVecVec(a,a)
		arr=toIndexMap(n)

		f=arr.inject(0){|a,b|a+b[2]}

		increased=0
		checked=0
		#a=[-1,0,1]
		#a=a.cross(a)
		(1..@w).each{|x|
			(1..@h).each{|y|
				if types.member?(get(x,y))
					checked+=1
					v=0
					arr.each{|p|
						cx=x+p[0]
						cy=y+p[1]
						#if types.member?(get(cx,cy))
							v+=getHeight(cx,cy)*p[2]
						#end						
					}
					v/=f
					#puts "#{v} #{v*f} #{f} #{level}"
					if v>level
						setHeight(x,y,getHeight(x,y)+inc)
						increased+=1
					end
				end
			}
		}
		puts "INCREASED:#{increased} CHECKED:#{checked} on level #{level}"
		applyHeightChanges
	end

	def erode(terrainTypes,amount=0.3)
		eroded=0
		(1..@w).each{|x|
			(1..@h).each{|y|
				if terrainTypes.member?(get(x,y))
					vs=[]
					(-1..1).each{|dy|
						(-1..1).each{|dx|	
							vs.push(getHeight(x+dx,y+dy))
						}
					}
					ch=getHeight(x,y)
					if ch<vs.sort[4]
						setHeight(x,y,ch-amount)
						eroded+=1
					end
				end
			}
		}		
		puts "ERODED: #{eroded}"
		applyHeightChanges
	end

	def rockIncrease
		h=0.6
		while h<3
			#rockIncreaseAbove(h,[:rock,:wood],0.3)
			rockIncreaseAbove(h,[:rock],0.15)
			h+=0.3
		end
		#}
	end

	def getDistance(x,y)
		return 0 unless x>0 and x<=@w and y>0 and y<=@h
		d=@dist[x+@w*y]
		if d
			return d
		end
		return 0
	end

	def applyDistChanges
		@dist=@ndist.clone
	end

	def setDistance(x,y,d)
		@ndist[x+@w*y]=d
	end

	def computeDistances(myTerrain)
		ar=[]
		(-1..1).each{|x|
			(-1..1).each{|y|
				ar.push([x,y,Math::sqrt(x*x+y*y)])
			}
		}

		found=0
		nfound=0
		# initial run
		(1..@w).each{|x|
			(1..@h).each{|y|
				if get(x,y)==myTerrain
					test=0
					ar.each{|p|
						dx=x+p[0]
						dy=y+p[1]
						if get(dx,dy)!=myTerrain
							setDistance(x,y,p[2]+rand)
							found+=1
							test+=1
							break
						end
					}
					if test==0
						nfound+=1
					end
				end
			}
		}

		applyDistChanges
		# fill in random valleys
		(1..20).each{|i|
			ok=true
			while ok
				x=(rand*@w).to_i+1
				y=(rand*@h).to_i+1
				if get(x,y)==myTerrain and getDistance(x,y)==0
					setDistance(x,y,1+rand)
					found+=1
					ok=false
				end
			end
		}



		applyDistChanges
		puts "FOUND #{found} NFOUND #{nfound}"
		
		modified=true
		try=0
		while modified
			modified=false
			try+=1
			puts "TRY #{try}"
			if try>20
				break
			end

			# do loop and increase distances where distance==0
			(1..@w).each{|x|
				(1..@h).each{|y|
					if get(x,y)==myTerrain and getDistance(x,y)==0
						ar.each{|p|
							dx=x+p[0]
							dy=y+p[1]
							r=rand+p[2]
							cd=getDistance(dx,dy)
							if cd>0 and (cd+r<getDistance(x,y) or getDistance(x,y)==0)
								found+=1
								setDistance(x,y,cd+r)
								setHeight(x,y,getHeight(x,y)+(cd+r)*0.1)
								modified=true
							end
						}			
					end
				}
			}
			puts "FOUND #{found}"
			applyDistChanges
		end
		applyHeightChanges
	end

private
	def computeNextFilter(a)
		n=[]
		l=0
		a.each{|c|
			n.push(l+c)
			l=c
		}
		n.push(l)
		n
	end
	def filterMatrix(n)
		m=[1,1]
		while n>1
			m=computeNextFilter(m)
			n-=1
		end
		m
	end
	
	def crossProductVecVec(a,b)
		n=[]
		a.each{|x|
			l=[]
			b.each{|y|
				l.push(x*y)
			}
			n.push(l)
		}
		n
	end
	
	def toIndexMap(a)
		n=[]
		y=0
		h=a.length
		w=a[0].length
		dy=h/2
		dx=w/2
		a.each{|r|
			x=0
			r.each{|v|
				n.push([x-dx,y-dy,v])
				x+=1
			}
			y+=1
		}
		n
	end


end

	

w=h=128

#myMap=MyMap.new(w,h)
myMap=MyHeightMap.new(w,h)
myMap.someX(10,:water)
myMap.someX(10,:green)
myMap.someX(10,:wood)
myMap.someX(10,:rock)
myMap.blurAll
myMap.to_surface.save("mapgen0.png")
if false
	myMap.computeHeights(:rough)
	myMap.blurHeights(5)
	myMap.computeHeights(:medium)
	myMap.blurHeights(2)
	myMap.computeHeights(:fine)
	myMap.blurHeights(1)
	myMap.to_surface.save("mapgen1.png")
	myMap.rockIncrease
	myMap.to_surface.save("mapgen2.png")
	myMap.blurHeights(1)
	myMap.erode([:rock])
	myMap.erode([:rock])
else
	myMap.computeHeights(:veryrough)
	myMap.blurHeights(10)
	myMap.computeHeights(:rough)
	myMap.blurHeights(5)
	myMap.computeDistances(:rock)
	myMap.computeHeights(:fine)
	myMap.blurHeights(1)
end
myMap.to_surface.save("mapgen3.png")
app=AGApplication.new
i=myMap.to_surface
i.save("mapgen9.png")
image=AGImage.new(nil,AGRect2.new(0,0,w,h),i,false)
app.setMainWidget(image)

app.run

