#!/usr/bin/env ruby

# Inventory view
class AntEnergy<AGWidget
	attr_accessor :hborder
	def initialize(p,rect)
		super(p,rect)

		@keys=[:energy,:morale,:food,:troops]
		@values={}
		@colors={}
		@keys.each{|k|@values[k]=0}

		@colors[:troops]=AGColor.new(0xcf,0,0)
		@colors[:energy]=AGColor.new(0,0xcf,0)
		@colors[:morale]=AGColor.new("#83006c")
		@colors[:food]=AGColor.new("#975500")
		@hborder=0

		#setCaching(true)
		@doRedraw=true
	end

	def setHero(h)
		@hero=h
	end
	
	def eventTick(t)
		#puts "MUH"
		updateValues
		super
	end
	def redraw
		@doRedraw
	end

	def draw(p)
		super
		@doRedraw=false
		r=p.getRect.origin
		w=r.width
		h=r.height

		mh=h/(@values.length*3)
		
		i=0
		@keys.each{|k|
			v=check(@values[k])
			if v<0
				puts v
			raise 1
			end
			c=@colors[k]
			a0=c*0.7
			a1=c*0.7
			a2=c*1.0
			a3=c*0.8
		
			mr=AGRect2.new(@hborder,i*mh,(w-@hborder*2)*v,mh*2)

			p.drawGradient(mr,a0,a1,a2,a3)
			i+=3
		}
	end
private
	def check(s)
		[0,s,1].sort[1]
	end
	def set(n,v)
		o=@values[n]
		if ((o-v).abs>0.1) or (o!=n and (o==0 or o==1))
			queryRedraw
			@doRedraw=true
			@values[n]=v
		end
	end
	def updateValues
		if @hero
			set(:energy,@hero.getEnergy)
			set(:morale,@hero.getMorale)
			set(:troops,@hero.getMen.length/60.0)
			set(:food,@hero.getFood)
		else
			@values.each{|k,v|@values[k]=1}
		end
	end
end

class AntEnergyCreator<AGLayoutCreator
	def initialize()
		super("antEnergy")
	end
	def create(parent,rect,node)
		w=AntEnergy.new(parent,rect)
		w.hborder=node.get("hborder").to_i
		return w
	end
end
$antEnergyCreator=AntEnergyCreator.new
