require 'ant_local.rb'

# this variables are used for playing sounds, which should be done with AntRubyEntity::playSound
INNER_VOL_SIZE=9 # size of circle around camera-middle with max volume
OUTER_VOL_SIZE=25 # size circle around camera describing volume descend

# AntRubyEntity slighty enhances the functionality already provided by AntEntity.
# This should be used for all the entity-types.
class AntRubyEntity<AntEntity
	attr_accessor :birthday
	attr_reader :uid

	def AntRubyEntity.setMap(map)
		@@map=map
	end
	def getMap
		@@map
	end

	def getScene
		@@map.getScene
	end


	# create a new entity at the position *p*
	# set some default settings
	# get a unique ID
	# loading must be done externally in loadXML !
	def initialize(position)
		super(position)
		@xmlProps={}
		@birthday=getMap.getTime
		@mode=""
		@handlers={}
		self.learnAmount=0.05
	
		@uid=getMap.getUniqueID

		setHunger(0) # general entities have no hunger
	end


	# play a sound identified by +name+. Sounds of this type shouldn't called when they were only called
	# +minDiff+ (or less) seconds before. Note that the sound is played at the place where this entity is placed.
	# So it's not hearable far away from it.
	def playSound(name,minDiff=0.5)
		scene=getMap.getScene
		d=((scene.getCamera.dim2-getPos2D).length-INNER_VOL_SIZE)
		vol=1
		if d>0
			vol=[(OUTER_VOL_SIZE-d)/OUTER_VOL_SIZE,0].max
		end
		AntSound.playSoundGlobal(name,vol,minDiff)
	end

	
	# :section: Editing

	# Within the editor you can change additional properties, e.g. count of men for a hero. This functions are used for this.
	def setXMLProp(n,v)
		@xmlProps[n]=v
	end
	def getXMLProp(n)
		if @xmlProps[n]==nil
			return ""
		else
			return @xmlProps[n]
		end
	end

	# :section: XML loading/saving

	def preloadXML(node)
		if node.get("birthday")!=""
			@birthday=node.get("birthday").to_f
		end
		@mode=node.get("mode")
		if node.get("uid")!=""
			@uid=node.get("uid").to_i
			getMap.checkUID(@uid)
		end
		setName(node.get("name"))
	end

	def loadXML(node)
		super
	end
	def saveXML(node)
		super(node)
		@xmlProps.each{|n,v|
			node.set(n,v)
		}
		if @birthday
			node.set("birthday",@birthday.to_s)
		end
		node.set("mode",@mode)
		node.set("uid",@uid.to_s)
	end
	def getDescription
		_("This is an entity - no more info here.")
	end

	# :section: miscellanous

	# simple comparison operator, so that ents can be distinguished
	# for what is this needed ???
	def <=>(e)
		to_s<=>e.to_s
	end

	def menCount
		0
	end
	def getPlayer
		nil
	end
	def fightTarget
		@fightTarget
	end
	def getRand
		if false
			# FIXME - implement me (network code)
			#rand
			puts "mrand:#{@mrand}"
			@mrand||=AGRandomizer.new("")
			val=@mrand.randFloat(1)
	
			puts "#{self} getRand #{val}  #{@mrand}"
			puts caller.join("\n")
	
			return val
		end
		agRand(1.0)
	end

	def setStrength(v)
		super
		setMoraleStrength(v*2)
	end

	# get the age of this entity - computed from @birthday
	def age
		((getMap.getTime-@birthday).to_f/YEAR).to_i
	end
	# @birthday will be set according to the current date (get it by calling getMap.getTime)
	def age=(years)
		@birthday=getMap.getTime-years*YEAR
	end
	def getMen
		[]
	end

	
	def setMode(mode)
		@mode=mode
	end
	def getMode
		@mode
	end



	def isOnOpenWater(p=nil)
		if (not p)
			p=getPos2D
		end
		getMap.getPos(p).z<-0.2
	end
	def isOnWater
		getMap.getPos(getPos2D).z<0
	end

	# give name under which this entity is stored in xml. It's generated from the classname. The first character is downcased.
	# The rest is camel-case as usual.
	# For instace: AntShop => antShop
	def xmlName
		xml=self.class.to_s
		xml=xml[0..0].downcase+xml[1..1000]
		return xml
	end

	def addHandler(eventName,&block)
		@handlers[eventName]||=[]
		@handlers[eventName].push(block)
	end

	def doEvent(name)
		if @handlers[name]
			@handlers[name].each{|b|
				b.call
			}
		end
	end

	# :section: job-handling
	# These functions add support for event-Handling within Entities' jobs. This is (will be) used for
	# scripting and AI.
	#	
	# FIXME: THis should be moved somewhere else (?)
	#
	# For more information on scripting link:files/ruby/scripting/README.html

	def newFightJob(p,target,distance)
		@fightTarget=target
		super
		doEvent(:eventNewFightJob)
	end
	def newRestJob(t)
		super
		doEvent(:eventNewRestJob)
	end

	# :section eventHandling
	def eventNoJob
		super
		doEvent(:eventNoJob)
	end
	def eventJobFinished
		super
		doEvent(:eventJobFinished)
	end

	def experienceFull
		super
		self.experience=1
	end


	# :section: status-information

	# an event-handler for resources being changed. In this case a possible view on the inventory is updated
	def resourceChanged	
		super
		begin
			AntInventory.update(self)
		rescue NameError
		end
	end

	# is this entity under attack - here this is always false, because it doesn't make sense for all entities
	def underAttack
		false
	end

	# :section: state-changes

	# set a mesh for this entity - have a look at AntModels for more information on how this works
	# * normally you give a subtype or nothing at all to this function and AntModels will take care of the right
	#   mesh. *sym* override the current entities type.
	# * you can pipe in a SceneNode-based object through *subtype* though this is no good !
	def setMesh(subtype="",sym=nil)
		return if getMap.getScene.nil?
		if subtype.is_a?(SceneNode)
			puts  "THIS SHOULD NOT BE USED ANY LONGER: setMesh(realMesh) !!!!!!!!!!!!"
			super(subtype) # wrapper
			return subtype
		end
		@map={:AntSack=>:sack}
		t=self.class.to_s
		t=t.gsub("Ant","").downcase
		t=t.to_sym
		if sym
			t=sym
		end
		
		super(mesh=AntModels.createModel(t,subtype))
		return mesh
	end




	# :section: deprecated


	# FIXME: remove this - this is a backward-compability function 
	def get
		self
	end
end
