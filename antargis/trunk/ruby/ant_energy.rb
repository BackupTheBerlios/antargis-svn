#!/usr/bin/env ruby

# Inventory view
class AntEnergy<AGWidget
	attr_accessor :hborder
	attr_accessor :vborder
	def initialize(p,rect)
		super(p,rect)

		@keys=[:energy,:morale,:food,:troops,:exp]
		@values={}
		@colors={}
		@keys.each{|k|@values[k]=0}

		@colors[:troops]=AGColor.new(0xcf,0,0)
		@colors[:energy]=AGColor.new(0,0xcf,0)
		@colors[:morale]=AGColor.new("#5555ff")
		@colors[:food]=AGColor.new("#975500")
		@colors[:exp]=AGColor.new("#FFFFFF")
		@hborder=0
		@vborder=0

		#setCaching(true)
		@hero=nil
	end

	def setHero(h)
		@hero=h
	end
	
	def eventTick(t)
		updateValues
		super
	end

	def draw(p)
		super
		r=p.getRect.origin
		w=r.width
		h=r.height-@vborder

		v=@vborder/2

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
		if ((o-v).abs>0.1) or (o!=v and (v==0 or v==1))
			queryRedraw
			@values[n]=v
		end
	end
	def updateValues
		if @hero
			set(:energy,@hero.getEnergy)
			set(:morale,@hero.getMorale)
			set(:troops,@hero.getMen.length/60.0)
			set(:food,@hero.getFood)
			set(:exp,@hero.experience)
		else
			@values.each{|k,v|@values[k]=1}
		end
	end
end

class AntEnergyCreator<AGLayoutCreator
	def create(parent,rect,node)
		w=AntEnergy.new(parent,rect)
		w.hborder=node.get("hborder").to_i
		w.vborder=node.get("vborder").to_i
		setResult w
	end
end
getLayoutFactory.addCreator("antEnergy",AntEnergyCreator.new)

