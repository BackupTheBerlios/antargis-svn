require 'ant_models.rb'

#
# AntSack is derived from AntAnimal, so that the KillANimal-job
# can be used to catch the contents. eventDie is used to remove the sack
# when the job is finished. So the behaviour is just like a dead animal
# TODO: Find a better name for AntAnimal
#
class AntSack<AntAnimal
	def initialize(map)
		super
		setMesh
		@enabled=true
		@storeGood=["food","tool","corn","stone","wood"]
	end
	# is called by a finished KillAnimal-hl-job
	def eventDie
		getMap.removeEntity(self)
	end
	
	# TODO: move this function to a module, which is included by AntHouse,AntTree and such, too.
	def resourceChanged
		@storeGood.each{|r|
			setProvide(r,resource.get(r)>0)
		}
		super
	end
end