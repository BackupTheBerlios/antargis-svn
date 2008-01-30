require 'ant_models.rb'

class AntSack<AntAnimal
	def initialize(map)
		super
		setMesh
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
		super
	end
end