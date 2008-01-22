
#
# +CombinedPathFinder+ combines the C++-implemented basic funtions with
# heuristc and path-find to a complete-pathfinder, that reacts to the
# abilities of the calling entity.
# It provides:
# * loading/saving a cache of the computed data in file named after the map's hash
# * computing a path/a distance w.r.t. to the abilities of an entity
# ** currently it differentiates between having a boat and not
#
# It uses the following C++-classes:
# * SimpleGraph/DecimatedGraph
# * makeGraph (which produces a SimpleGraph)
# * StoredHeuristicFunction
# * Pathfinder
#
class CombinedPathFinder
	attr_accessor :scene

	# the current available modes, in which pathes will be computed. You can add some more here
  # but you'll have to modify the functions getPathWeighter, getMode, displayPathfindingGraph
	MODES=[:normal,:overWater]

	def initialize(map)
		@graph={}
		@heuristic={}
		@path={}
		@map=map

		# check Cache
		cacheFilename=getCacheFilename(map)
		if fileExists(findFile(cacheFilename))
			loadFromFile(cacheFilename)
		else
			computeEverything(map)
			saveToFile(cacheFilename)
		end

		# put them together
		MODES.each{|mode|
			@path[mode]=Pathfinder.new(@graph[mode],@heuristic[mode])
		}
	end

	def distance(p0,p1,entity)
		@path[getMode(entity)].distance(p0,p1)
	end

	def computePath(p0,p1,entity)
		path=@path[getMode(entity)].computePath(p0,p1)
		#displayPath(path)
		path
	end
	def refinePath(waypoints,entity)
		mode=getMode(entity)
		path=@path[mode].refinePath(waypoints,getPathWeighter(@map,mode))
		#displayPath(path,AGVector4.new(1,0,0,1))
		path
	end


	def displayPathfindingGraph(map,scene)
		return
		# 		colors={:normal=>AGVector4.new(1,0,0,1),:overWater=>AGVector4.new(1,1,0,1)}
		# 		width=2
		# 		MODES.each{|mode|
		# 			wireframe=Boa3dWireframe.new(scene,colors[mode],width)
		# 			(0..(@graph[mode].edges-1)).each{|i|
		# 				edge=@graph[mode].getEdgePosition(i)
		# 				a=edge[0]
		# 				b=edge[1]
		# 				a=AGVector3.new(a.x,a.y,map.getHeight(a.x,a.y)+0.15)
		# 				b=AGVector3.new(b.x,b.y,map.getHeight(b.x,b.y)+0.15)
		# 				wireframe.addLine(a,b)
		# 			}
		# 			scene.addNode(wireframe)
		# 			width+=2
		# 		}
	end

	def displayPath(path,color=AGVector4.new(1,1,0,1))
		if path.length>2
			width=5
			node=Boa3dWireframe.new(@scene,color,width)
			on=nil
			path.each{|n|
				if on
					a=AGVector3.new(n.x,n.y,@map.getHeight(n.x,n.y)+0.15)
					b=AGVector3.new(on.x,on.y,@map.getHeight(on.x,on.y)+0.15)
					node.addLine(a,b)
					pp a,b
				end
				on=n
			}
			@scene.addNode(node)
		end
	end

private
	# get path-finding mode for the given +entity+
	def getMode(entity)
		if entity.resource.get("boat")>0
			# if entity has boat then is can cross water - FIXME: put this into an entity-based function
			:overWater
		else
			:normal
		end
	end

	def getCacheFilename(map)
		levelHash=map.hash # build a hash out of the height-map
		cacheFilename=levelHash+".cache"
	end

	def loadFromFile(filename)
		content=loadFile(filename)
		stream=BinaryStringIn.new(content)
		MODES.each{|mode|
			@graph[mode]=SimpleGraph.new(stream)
			@heuristic[mode]=StoredHeuristicFunction.new(stream)
		}
	end

	def saveToFile(filename)
		# save everything to the cachefile
		stream=BinaryStringOut.new
		MODES.each{|mode|
			@graph[mode].printTo(stream)
			@heuristic[mode].printTo(stream)
		}
	
		saveFile(filename,stream.getString)
	end

	def computeEverything(map)
		MODES.each{|mode|computeMode(map,mode)}
	end

	def computeMode(map,mode)
		# build a map-height/distance weighter
		weighter=getPathWeighter(map,mode)

		# set initial distance of waypoints	
		minDist=4

		begin
			# make a path-finding graph
			sgraph=makeGraph(map,weighter,minDist)
			minDist*=2
			puts "---"
			pp minDist
			pp sgraph.size,sgraph.edges
		end while(sgraph.size>2000) # use a smaller resolution, if there are too many nodes in graph

		assert {sgraph.size>0}
		assert {sgraph.edges>0}

		# copy to a decimating graph
		graph=DecimatedGraph.new(sgraph)

		# compute a decimation-factor
		factor=1.0-800.0/graph.size

		# debugging settings
		#factor=0.4
		#factor=1.0-220.0/@dgraph.size

		graph.decimate(factor,weighter)

		# compute a distance-field and use this as a pre-computed heuristic
		@heuristic[mode]=computeHeuristic(graph)
		@graph[mode]=graph
	end

	def getPathWeighter(map,mode)
		case mode
			when :normal
				return MapPathWeighter.new(map,false)
			when :overWater
				return MapPathWeighter.new(map,true)
		end
	end
end