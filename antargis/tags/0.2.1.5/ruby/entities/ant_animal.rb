# some very simple base-class for animals. contains really nothing.
class AntAnimal<AntRubyEntity
	def initialize(map)
		super
	end
	def AntAnimal.xmlName
		""
	end
protected
	# give Birth every 40th call
	# returns true, if birth given
	# at most 50 entities of a special type will be created
	def giveBirth
		@lastBirth||=getRand*40

		@lastBirth+=1
		# BIRTHRATE is here:
		if @lastBirth>40 then
			fishList=getMap.getAllByType(self.class)
			return if fishList.length>50
			# make child
			#puts "A FISH IS BORN"
			fish=self.class.new(getMap)
			fish.setPos(getPos2D)
			getMap.insertEntity(fish)
			getMap.endChange
			newRestJob(2)
			@lastBirth=-getRand()*10
			puts "GIVE BIRTH"
			return true
		end
		false
	end
end

