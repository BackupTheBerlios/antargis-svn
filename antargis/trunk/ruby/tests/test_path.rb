#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

scene=Scene.new(800,600)
map=AntMap.new(scene,32,32)
map.loadMap("data/levels/birth/birth3.antlvl") #data/levels/tutorial/tutorial0.antlvl")
#map.loadMap("data/levels/tutorial/tutorial0.antlvl")

weighter=MapPathWeighter.new(map)

puts "MAKING GRAPH"
graph=makeGraph(map,weighter)

graph.printInfo
dg=DecimatedGraph.new(graph)
dg.printInfo
puts "decimating"
dg.decimate(0.95)

#dg.print
dg.printInfo

pf=Pathfinder.new(dg)
p=pf.computePath(AGVector2.new(0,0),AGVector2.new(90,90))
