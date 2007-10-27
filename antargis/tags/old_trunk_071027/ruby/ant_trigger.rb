class Trigger
	attr_reader :name, :player
	def initialize(node)
		if node.class==Node
			@pos=AGVector2.new
			@pos.loadXML(node)
			@radius=node.get("radius").to_f
			@name=node.get("name")
			@circle=AGCircle2.new(AGVector3.new(@pos.x,@pos.y,1),@radius)
			@triggerTime=AntRubyMap.getSystemTime
			@player=node.get("player")
			@text=node.get("text")
		else
			@name=node
			@triggerTime=AntRubyMap.getSystemTime
		end
	end
	def saveXML(n)
		@pos.saveXML(n)
		n.set("radius",@radius.to_s)
		n.set("name",@name)
		n.set("player",@player)
		n.set("text",@text)
	end
	def check(hero)
		if AntRubyMap.getSystemTime-@triggerTime>1 # wait at least a second for repeating trigger
			p2=hero.getPos2D
			p3=AGVector3.new(p2.x,p2.y,1)
			#dputs name," ",p3,@pos,@radius,@circle.inCircle(p3), (p2-@pos).length
			if @circle.inCircle(p3)
				getMap.trigger(hero,self)
				@triggerTime=AntRubyMap.getSystemTime
			end
		end
	end
	def xmlName
		"trigger"
	end
end