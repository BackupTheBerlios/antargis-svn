#!/usr/bin/env ruby

require 'antargisgui.rb'

main=AGMain.new(800,600,32,false,false)

def load(n)
	getSurfaceManager.loadSurface(n)
end

def makeTerrain(triple,thresF=0.5,diffF=0.1)
	ground=load("ground_4.png")
	darkground=load("ground_5.png")
	grass=load("big_grass.png")
	
	ground=load(triple[2])
	darkground=load(triple[1])
	grass=load(triple[0])
	
	
	mask=load("terrain_mask3.png")
	
	terrain=AGSurface.new(128,128)
	
	min=300
	max=0
	for x in 0..127
		for y in 0..127
			c=mask.getPixel(x,y)
			r=toInt(c.brightness)
			min=[min,r].min
			max=[max,r].max
		end
	end
	thresh=(max-min)*thresF+min
	diff=(max-min)*diffF
	
	puts "DIFF:",diff
	
	for x in 0..127
		for y in 0..127
			c=mask.getPixel(x,y)
			r=toInt(c.brightness)
			if r>thresh+diff
				terrain.putPixel(x,y,grass.getPixel(x,y))
			elsif r<thresh-diff
				terrain.putPixel(x,y,ground.getPixel(x,y))
			else
				
				if r<thresh
					g=darkground.getPixel(x,y)
					t=ground.getPixel(x,y)
					v=(r-(thresh-diff))/diff
					#v=1-v
					n=g*v+t*(1-v)
#					terrain.putPixel(x,y,darkground.getPixel(x,y))
					terrain.putPixel(x,y,n)
				else
					g=darkground.getPixel(x,y)
					t=grass.getPixel(x,y)
					v=(r-thresh)/diff
					#v=1-v
					n=g*(1-v)+t*v
#					terrain.putPixel(x,y,darkground.getPixel(x,y))
					terrain.putPixel(x,y,n)
				end
			end
		end
	end
	return terrain
end

ts=[]

triple=["underwater4.png","underwater3.png","underwater3.png"]
ts+=[-0.2,0.30,0.5,0.65].collect{|f|makeTerrain(triple,f)}

triple=["underwater3.png","sand_4.png","sand_4.png"]
ts+=[-0.2,0.30,0.5,0.65].collect{|f|makeTerrain(triple,f)}

triple=["sand_4.png","underwater3.png","ground_4.png"]
ts+=[-0.2,0.30,0.5,0.65].collect{|f|makeTerrain(triple,f)}

triple=["ground_4.png","ground_5.png","big_grass.png"]
ts+=[-0.2,0.30,0.65,1.2].collect{|f|makeTerrain(triple,f)}

triple=["big_grass.png","big_grass3.png","big_grass_2.png"]
ts+=[0.2,0.4,0.6,1.2].collect{|f|makeTerrain(triple,f)}

triple=["big_grass_2.png","big_grass3.png","forest2.png"]
ts+=[0.2,0.4,0.6,1.2].collect{|f|makeTerrain(triple,f)}

triple=["forest2.png","underwater3.png","rock.png"]
ts+=[0.2,0.4,0.6,1.2].collect{|f|makeTerrain(triple,f)}


while ts.length<32
	ts.push(ts[ts.length-1])
end


app=AGApplication.new
app.setMainWidget($s=AGScreenWidget.new)
x=0
y=0
i=0

all=AGSurface.new(128,128*ts.length)
i=0
ts.each{|t|
	all.blit(t,AGRect.new(0,128*i,128,128),AGRect.new(0,0,128,128))
	i+=1
}
$s.addChild(AGImage.new($s,AGPoint.new(0,0),all,false))
all.save("new3d.png")
# ts.each{|t|
# 	$s.addChild(AGImage.new($s,AGPoint.new(x,y),t,false))
# 	t.save("terrain_tile#{i}.png")
# 	x+=128
# 	if x>=128*4
# 		x=0
# 		y+=128
# 	end
# 	i+=1
# }
app.run