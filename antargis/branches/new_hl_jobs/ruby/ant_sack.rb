require 'ant_models.rb'

class AntSack<AntAnimal
	def initialize(p=AGVector2.new(0,0))
		super
		setMesh(createModel(:sack))
		@enabled=true
		@storeGood=["food","tool","corn","stone","wood"]
	end
	def eventDie
		getMap.removeEntity(self)
	end
	def resourceChanged
		@storeGood.each{|r|
			setProvide(r,resource.get(r)>0)
		}
	end
end