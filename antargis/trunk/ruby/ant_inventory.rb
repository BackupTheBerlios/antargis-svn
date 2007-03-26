# Inventory view
class AntInventory<AGWidget
	@@inventory=nil

	attr_reader :job
	def initialize(p,rect)
		super(p,rect)
		@@inventory=self
		@resTypes=["wood","stone","men","food","tool","gold","boat","shield","sword","bow","coal","ore","steel","rod","fruit","corn","flour"] #,"bread"]
		setCaching(true)
		@invinited=false

		@buttonNames=["doRecruit","doTakeFood","doTakeWeapons","doConstruct","doUpgrade","doContinue","doSupport"]

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

	def inspectEntity(e)
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
# 	def prepareDraw
# 		#return
# 		# FIXME
# 		if false
# 			checkFriend
# 			updateInspection
# 		end
# 		super
# 	end

	def update(e)
		trace
		if @inspect==e
			trace
			updateInspection
		end
	end

	def draw(p)
		trace
		super
	end
private
	def checkFriend
		#trace
		if @inspect.nil?
			return
		end
		friend=(@inspect.getPlayer==getMap.getPlayer)
		enemy=(not friend)
		puts "friend #{friend}"
		getChild("friend_true").setVisible(friend)
		getChild("friend_false").setVisible(enemy)
		getChild("enemy_true").setVisible(enemy)
		getChild("enemy_false").setVisible(friend)
	end
	def checkButtons
		trace	
		my=(@inspect.getPlayer==getMap.getPlayer and @inspect!=$app.hero)
		
		#myok=my # reenable other options when under attack
		myok=(my and (not @inspect.underAttack))

		puts "my #{my} myok #{myok}"
		getChild("doRecruit").setVisible(((not my) or (not @inspect.underAttack)))
		getChild("doRecruit").setEnabled(myok)
		getChild("doTakeFood").setEnabled(myok)
		getChild("doTakeWeapons").setEnabled(myok) # FIXME

		getChild("doConstruct").setVisible(@inspect.is_a?(AntHouse))
		getChild("doUpgrade").setVisible((not @inspect.is_a?(AntHouse)))
		getChild("doConstruct").setEnabled((my and @inspect.is_a?(AntWorkshop)))
		getChild("doUpgrade").setEnabled((my and @inspect.experience>0.95))
		getChild("doContinue").setVisible(@inspect.is_a?(AntBuildingSite)) # FIXME!!!!!!!!!!!
		getChild("doContinue").setEnabled(@inspect.is_a?(AntBuildingSite)) # FIXME!!!!!!!!!!!

		# support / hide recruit
		getChild("doSupport").setVisible((my and @inspect.underAttack))

		
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
			if w.getText.to_s!=mv
				w.setText AGStringUtf8.new(mv)
			end
		end
	end
	def setTitle(t)
		t=_(t)
		c=getChild("invTitle")
		if c.class==AGButton
			c.setCaption(t)
		else
			c.setText(AGStringUtf8.new(t))
		end
	end
	def updateInspection
		trace
		if @inspect then
			trace
			checkButtons
			checkFriend
			res=@inspect.resource.getAll
			res["men"]=@inspect.menCount.to_f
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
					getChild("minion").setText(_("Obeys nobody"))
				else
					getChild("minion").setText(_("Obeys {1}",bossname.to_s))
				end
			end
			#redraw
		end
	end

	def AntInventory.inspectEntity(entity)
		@@inventory.inspectEntity(entity) if @@inventory
	end
	def AntInventory.update(entity)
		trace
		@@inventory.update(entity) if @@inventory
		trace
	end
	def AntInventory.resetPointer
		@@inventory=nil
	end
end

class AntInventoryCreator<AGLayoutCreator
	def create(parent,rect,node)
		setResult(AntInventory.new(parent,rect))
	end
end
getLayoutFactory.addCreator("antInventory",AntInventoryCreator.new)
