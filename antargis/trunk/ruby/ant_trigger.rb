#!/usr/bin/env ruby

class Trigger
	attr_reader :name, :player
	def initialize(node)
		@pos=AGVector2.new
		@pos.loadXML(node)
		@radius=node.get("radius").to_f
		@name=node.get("name")
		@circle=AGCircle2.new(AGVector3.new(@pos.x,@pos.y,1),@radius)
		@triggerTime=$systemTime
		@player=node.get("player")
		@text=node.get("text")
	end
	def check(hero)
		if $systemTime-@triggerTime>1 # wait at least a second for repeating trigger
			p2=hero.getPos2D
			p3=AGVector3.new(p2.x,p2.y,1)
			#dputs p3,@circle.inCircle(p3)
			if @circle.inCircle(p3)
				getMap.trigger(hero,self)
				@triggerTime=$systemTime
			end
		end
	end
end