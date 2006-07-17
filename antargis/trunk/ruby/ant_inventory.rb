#!/usr/bin/env ruby

# Inventory view
class AntInventory<AGWidget
	attr_reader :job
	def initialize(p,rect)
		super(p,rect)
		$inventory=self
		@resTypes=["wood","stone","men","food","tool","gold","boat","shield","sword","bow","coal","ore","steel","rod","fruit","corn","flour"] #,"bread"]
		setCaching(true)
		@invinited=false

		@buttonNames=["doRecruit","doTakeFood","doTakeWeapons","doConstruct","doUpgrade"]

		addSignal("sigJobChanged")

	end
	def initHandlers
		puts "INITHANDLERS"
		@buttonNames.each{|n|
			addHandler(getChild(n),:sigClick,:eventJobChanged)
		}
		addHandler(getChild("doInspect"),:sigClick,:eventInspect)
	end
	def eventJobChanged(e)
		@job=e.getCaller.getName
		sigJobChanged(e)
		return true
	end

	def eventInspect
		$app.viewInformation(@inspect)
		return true
	end

	def inspect(e)
		getChild("shutter").hide
		if e.getName!="" and (not e.getName=~/0\./)
			setTitle(e.getName)
		else
			n=e.class.to_s
			if n[0..2]=="Ant"
				n.sub!("Ant","")
			end
			setTitle(n)
		end
		@inspect=e
		getChild("InvBar").setHero(e)
		updateInspection
	end
	def prepareDraw
		#return
		checkFriend
		updateInspection
		super
	end
private
	def checkFriend
		if @inspect.nil?
			return
		end
		friend=(@inspect.getPlayer==getMap.getPlayer)
		enemy=(not friend)
		getChild("friend_true").setVisible(friend)
		getChild("friend_false").setVisible(enemy)
		getChild("enemy_true").setVisible(enemy)
		getChild("enemy_false").setVisible(friend)
	end
	def checkButtons
		my=(@inspect.getPlayer==getMap.getPlayer and @inspect!=$app.hero)
		getChild("doRecruit").setEnabled(my)
		getChild("doTakeFood").setEnabled(my)
		getChild("doTakeWeapons").setEnabled(my) # FIXME

		getChild("doConstruct").setVisible(@inspect.is_a?(AntHouse))
		getChild("doUpgrade").setVisible((not @inspect.is_a?(AntHouse)))
		getChild("doConstruct").setEnabled((my and @inspect.is_a?(AntWorkshop)))
		getChild("doUpgrade").setEnabled((my and @inspect.experience>0.95))
		
	end

	def setValue(name,value)
		ok=@resTypes+["boss"]
		if ok.member?(name) then
			w=getChild(name)
			return if not w 
			mv=value
			if value.class!=String
				mv=value.to_i.to_s
			end
			if w.getText!=mv
				w.setText mv
			end
		end
	end
	def setTitle(t)
		t=_(t)
		c=getChild("invTitle")
		if c.class==AGButton
			c.setCaption(t)
		else
			c.setText(t)
		end
	end
	def updateInspection
		if @inspect then
			checkButtons
			checkFriend
			res=@inspect.resource.getAll
			res["men"]=@inspect.menCount.to_f
#			res.each{|a,b|
			@resTypes.each{|a|
				b=res[a]
				b||=0
				setValue(a,b)
			}
			setValue("name",@inspect.getName)
			bossname=""
			if @inspect.methods.member?("getPlayer")
				if @inspect.getPlayer
					bossname=@inspect.getPlayer.getName
				end
			end
			setValue("boss",bossname)

			if bossname
				if bossname==@inspect.getName or bossname==""
					getChild("minion").setText("Obeys nobody")
				else
					getChild("minion").setText("Obeys "+bossname.to_s)
				end
			end
			#redraw
		end
	end
end

class AntInventoryCreator<AGLayoutCreator
	def initialize()
		super("antInventory")
	end
	def create(parent,rect,node)
		return AntInventory.new(parent,rect)
	end
end
$antInventoryCreator=AntInventoryCreator.new
