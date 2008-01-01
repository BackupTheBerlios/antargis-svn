module AI
# This file contains a dynamic and simplified representation of the gaming world.
# This is needed for computing the max/min-tree within the AI.
#
# The representation holds the following information (roughly spoken):
# * players
# * heroes incl. position/health/man-count/resource(from all men summed up)
# * trees/resources
# * buildings with resources/man-count
#
# These information must be sufficient for:
# * estimating actions
# * estimating value of the current situation
#
# Furthermore this complete representation must be copyable, so that
# in each node of the max/min-tree, a complete world can take place.
# Thus it must be pretty small, easy to compute and right detailed enough
# to let the AI be a little decent
#
# TODO:
# * check if trees can be combined to forests as an object ?
#
#
	class DynPlayer
		attr_accessor :player
		attr_reader :realPlayer
		def initialize(player)
			@realPlayer=player
		end
	end

	class DynHero
		def initialize(hero)
			@realHero=hero
			@pos=hero.getPos2D
			allResources=hero.getMen.map{|man|man.resource.getAll}

			@resource=allResources.inject({})
			{|a,new|
				new.each{|k,v|
					a[k]||=0
					a[k]+=v
				}
				a
			}
			@health=hero.getStrength
			@men=hero.getMen.length
		end
	end

	class DynMap
		def initialize(map)
			@players=map.getPlayers.map{|p|DynPlayer.new(p)}
			@heroes=map.getHeroes.map{|h|DynHero.new(h)}
			@buildings=map.getBuildings.map{|b|DynBuilding(b)}
			@myplayer=@players.select{|p|p.realPlayer==map.getPlayer}[0]
			assert{@myplayer}
			@sources=[AntTree,AntFir,AntStone,AntMine].map{|type|map.getByType(type)}.flatten.uniq.map{|e|DynSource.new(e)}
		end
	end
end

