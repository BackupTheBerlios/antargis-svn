#!/usr/bin/env ruby

$:.push("ext")
ENV["PATH"]+="./ext"

require 'antargisgame.so'

include Antargisgame
include Antargisgui
include Antargisvideo
include Antargismath
include Antargisbasic

getMain.getVideo.initVideo(640,480,32,false,true)

w=16
h=16

heightMap=HeightMap.new(nil,w,h)

(0..w).each{|x|
	(0..h).each{|y|
		heightMap.set(x,y,rand)
	}
}


heightMap=AntMap.new(nil,64,64)
heightMap.loadMap("data/levels/tutorial/tutorial2.antlvl")
#heightMap.loadMap("data/levels/birth/birth3.antlvl")


w=heightMap.getW
h=heightMap.getH

puts "W:#{w} H:#{h}"
#exit

distComputer=DistanceComputer.new(heightMap,4)

puts "creating FieldCollection"
fieldCollection=FieldCollectionWithDistances.new

puts "assigning Fields"
assignFields(fieldCollection,distComputer,64)

#fieldCollection.computeDistances

puts "initing distances for field #0"

field=fieldCollection.getField(0)
puts field
#field.initLocalDistances(distComputer)



fsize=4

image=AGSurface.new(w*fsize,h*fsize)

def getColor(i)
	j=i%8
	j+=1
	AGColor.new((j % 2)*255, ((j /2)%2)*255, (j / 4).to_i*255) #*(255.0-(i*2)/255.0)
end


def displayFieldAssign(fieldCollection,image,w,h,fsize)
	puts "DISPLAYING DATA"
	(0..(w-1)).each{|x|
		(0..(h-1)).each{|y|
			fieldnum=fieldCollection.getFieldId(AGVector2.new(x,y))
			if fieldnum<20000
				color=getColor(fieldnum)
				(0..(fsize-1)).each{|dx|
					(0..(fsize-1)).each{|dy|
						image.putPixel(x*fsize+dx,y*fsize+dy,color)
					}
				}
			end
		}
	}
end

def displayDistanceData(field,image)
	m=field.getMiddle
	field.getVectors.each{|v|
		
		image.putPixel(v.x.to_i,v.y.to_i,AGColor.new(0xFF,0xFF,0xFF)*(1-(v-m).length*0.3))
	}
end

def displayNeighbors(field,image)
	field.getNeighbors.each{|v|
		image.putPixel(v.x.to_i,v.y.to_i,AGColor.new(0xFF,0,0))
	}
end
puts "COMPUTE  DISTACNES:"
fieldCollection.computeDistances(distComputer)
puts "\n\n\nok"

#displayFieldAssign(fieldCollection,image,w,h,fsize)
(0..(fieldCollection.getFieldCount-1)).each{|num|
	field=fieldCollection.getField(num)
	#field.init
	displayDistanceData(field,image)
	#displayNeighbors(field,image)
}


app=AGApplication.new
imageW=AGImage.new(nil,AGRect2.new(0,0,w*fsize,h*fsize),image,false)
app.setMainWidget(imageW)
app.run

