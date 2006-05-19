#!/usr/bin/env ruby

if ENV["_"]=~/credits.rb/
	require 'antargisgui.rb'
end

class AntTicker<AGWidget
	def initialize(p,r,text)
		super(p,r)
		@text=text
		@y=-height
		@time=0
		@text=text.gsub("\t","").split("\n")
		@nfont=AGFont.new("Arial.ttf",24)
		@cfont=AGFont.new("Arial.ttf",24)
		@starttime=-1
		@speed=0.03
	end
	def setNFont(f)
		@nfont=f
	end
	def setCFont(f)
		@cfont=f
	end
	def draw(p)
		now=$menuapp.getTicks
		if @starttime>0
			@y+=(now-@starttime)*@speed
		end
		@starttime=now
		ty=0
		
		@text.each{|r|
			font=nil
			if r=~/_/	
				#r.sub!("_","")
				r=r[1..(r.length-1)]
				font=@cfont
			else
				font=@nfont
			end

			w=font.getWidth(r)
			tx=((width-w)/2).to_i
			p.renderText(r,AGVector2.new(tx,(ty-@y)),font)
			ty+=40
		}
	end
end

class AntTickerCreator<AGLayoutCreator
	def initialize
		super("antTicker")
	
	end
	def create(p,r,n)
		t=AntTicker.new(p,r,n.get("text"))
		t.setNFont(getTheme.getFont(n.get("nfont")))
		t.setCFont(getTheme.getFont(n.get("cfont")))
		return t
	end
end
$antTickerCreator=AntTickerCreator.new

if ENV["_"]=~/credits.rb/
	m=AGMain.new(640,480,32,false,true)
	
	app=AGApplication.new
	$menuapp=app
	t=AntTicker.new(nil,AGRect.new(0,0,640,480),"bla\nxyz\nhallo\nkjsdhdfhdf ksdfhjsdfj h\nkjsdfjs")
	app.setMainWidget(t)
	app.run
end