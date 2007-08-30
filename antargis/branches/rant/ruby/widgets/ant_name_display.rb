class AntNameDisplay<AGWidget
	@@fontChangeCount=0

	def initialize(p,r,hero,map)
		super(p,r)
		@map=map
		@hero=hero
		@font=getTheme.getFont("heroName.font")
		@oldfont=@font
		@name=@hero.getName
		addChild(@mb=AGButton.new(self,AGRect.new(0,0,width,height),_("")))
		
		@mb.setEnabled(false)
		addChild(@textWidget=AGText.new(self,AGRect.new(0,0,width,height),AGStringUtf8.new(@hero.getName),@font))
		
		@fonts={true=>getTheme.getFont("heroName.font"),false=>getTheme.getFont("enemyHero.font")}
		@oldPlayer=nil
		setCaching(true)
	end

	# enforce integer position	
	def setRect(r)
		super(AGRect.new(r.x.to_i,r.y.to_i,r.w.to_i,r.h.to_i))
	end
	
	def getText
		@name
	end

	def prepareDraw
		@font=@fonts[@hero.getPlayer==@map.getPlayer]
		if @font!=@oldfont
			puts "#{@hero.getPlayer} #{@map.getPlayer} #{@oldPlayer}"
			puts "#{@font} #{@oldfont}"
			@oldPlayer=@hero.getPlayer
			puts "font changed"
			@@fontChangeCount||=0
			@@fontChangeCount+=1
			if @@fontChangeCount>20
				raise 1
			end
			@oldfont=@font
			@textWidget.setFont(@font)
			queryRedraw
		end
		super
	end

	private
end
