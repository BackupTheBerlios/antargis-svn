# A farm produces food, uses men and tools
#
# so
#     needs at least: 1 man
#                     1 tool per man
#     at most:        10 man
#                     30 tools
#
# (A farm is connected to a village)
# A village is a virtual object, which provides:
#   - barn
#   - hotel / living house
#   - werkstatt

#
#
# food production is done through:
#    - get sheep
#    - (go fishing)
#    - go hunting (deer, wolf)
#    - fetch vegetables
#    - farming (wheat)

class AntTownHall<AntHouse
	def initialize
		super
		@type=3
		@defeated=[]
		@atHome=[]
		setDirection(-10)
	
		@storeGood=["food","tool","corn"]
	end
	
	def setupMesh
		setMesh
	end

	def resourceChanged
		@storeGood.each{|r|
			setProvide(r,resource.get(r)>0)
		}
		super
	end

	def neededStock
		s={}
		@storeGood.each{|r|s[r]=15}
		s
	end

	def houseType
		"townhall"
	end
end