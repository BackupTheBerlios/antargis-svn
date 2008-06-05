#!/usr/bin/env ruby

$:.push("ext")
ENV["PATH"]+="./ext"

require 'antargisgame.so'

include Antargisgame
include Antargisgui
include Antargisvideo
include Antargismath
include Antargisbasic

getMain.getVideo.initVideo(640,480,32,false,false)

w=16
h=16

heightMap=HeightMap.new(nil,w,h)

(0..w).each{|x|
  (0..h).each{|y|
    heightMap.set(x,y,rand)
  }
}


heightMap=AntMap.new(nil,64,64)
heightMap.loadMap("data/levels/birth/birth2.antlvl")


w=heightMap.getW
h=heightMap.getH

puts "W:#{w} H:#{h}"
#exit

distComputer=DistanceComputer.new(heightMap,1)

pathData=PathV3Data.new(8)

pathData.compute(distComputer)

fsize=4

image=AGSurface.new(w*fsize,h*fsize)

def getColor(i)
  j=i%8
  j+=1
  #puts "J:#{j}"
  AGColor.new((j % 2)*255, ((j /2)%2)*255, (j / 4).to_i*255) #*(255.0-(i*2)/255.0)
end


(0..(w-1)).each{|x|
  (0..(h-1)).each{|y|
    fieldnum=pathData.getField(AGVector2.new(x,y))
    if fieldnum<20000
      puts "field:#{fieldnum}"
      color=getColor(fieldnum)
      puts "COLOR:#{color}"
      (0..(fsize-1)).each{|dx|
        (0..(fsize-1)).each{|dy|
          image.putPixel(x*fsize+dx,y*fsize+dy,color)
        }
      }
    end
  }
}


#texture=AGTexture.new(image)

app=AGApplication.new
imageW=AGImage.new(nil,AGRect2.new(0,0,w*fsize,h*fsize),image,false)
#imageW.setSurface(image)
app.setMainWidget(imageW)
app.run

#getScreen.blit(texture,AGRect2.new(0,0,w*fsize,h*fsize),AGRect2.new(0,0,w*fsize,h*fsize))
#getScreen.flip

#getMain.delay(20000)
