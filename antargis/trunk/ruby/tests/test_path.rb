#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

scene=Scene.new(800,600)
map=AntMap.new(scene,32,32)
#map.loadMap("data/levels/birth/birth3.antlvl") #data/levels/tutorial/tutorial0.antlvl")
map.loadMap("data/levels/tutorial/tutorial0.antlvl")
#map.loadMap("data/levels/dev/testai.antlvl")

weighter=MapPathWeighter.new(map)

puts "MAKING GRAPH"
graph=makeGraph(map,weighter)

graph.printInfo
dg=DecimatedGraph.new(graph)

# ms=AGSurface.new(800,600)
# dg.paint(ms.getRect,ms)
# ms.save("graph.png")


dg.printInfo
puts "decimating"
dg.decimate(0.8,weighter)

# ms=AGSurface.new(800,600)
# dg.paint(ms.getRect,ms)
# ms.save("graph2.png")


dg.printInfo

heuristic=computeHeuristic(dg)

# class MyHeu<HeuristicFunction
# 	def distance(
# end
# 
# heuristic

$startVec=dg.findNearestVector(AGVector2.new(0,0))
$endVec=dg.findNearestVector(AGVector2.new(90,90))

nodeVec=dg.findNearestVector(AGVector2.new(90,90))
$heuristic=Heuristic.new(nodeVec,heuristic)
GC.disable

$dg=dg

$app=AGApplication.new

$i=0
class MDebug<PathDebugging
	def debugPath(p,h)
		$i+=1
		#ms=AGSurface.new(800,600)
		t=getScreen
		$dg.paintNode(t.getRect,t,$startVec,AGColor.new(0xFF,0,0xFF))
		$dg.paintNode(t.getRect,t,$endVec,AGColor.new(0,0,0xFF))
		$dg.paint(t.getRect,t,$heuristic)
		p.paint(t.getRect,t,$dg.width)
		
		#t=AGTexture.new(ms)
		#getScreen.blit(t,ms.getRect,ms.getRect)
		p=AGPainter.new(getScreen)
		p.renderText(h.to_s,AGVector2.new(80,0),AGFont.new)
		p=nil


		getScreen.flip
		$app.delay(500)
	end
end

d=MDebug.new
pf=Pathfinder.new(dg,heuristic,d)

#n=dg.findNearest(AGVector2.new(90,90)

p=pf.computePath(AGVector2.new(0,0),AGVector2.new(90,90))
