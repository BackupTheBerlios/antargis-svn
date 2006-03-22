#!/usr/bin/env ruby

# Inventory view
class AntInventory<AGWidget
	attr_reader :job
	def initialize(p,rect)
		super(p,rect)
		$inventory=self
		@resTypes=["wood","stone","men","food","tool"]
		setCaching(true)
		@invinited=false

		@buttonNames=["doRecruit","doTakeFood","doTakeWeapons"]

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
		if e.getName!=""
			setTitle(e.getName)
		else
			n=e.class.to_s
			if n[0..2]=="Ant"
				n.sub!("Ant","")
			end
			setTitle(n)
		end
		@inspect=e
		checkButtons
		checkFriend
		updateInspection
	end
	def prepareDraw
		checkFriend
		updateInspection
		super
	end
private
	def checkFriend
		friend=(@inspect.getPlayer==getMap.getPlayer)
		enemy=(not friend)
		getChild("friend_true").setVisible(friend)
		getChild("friend_false").setVisible(enemy)
		getChild("enemy_true").setVisible(enemy)
		getChild("enemy_false").setVisible(friend)
	end
	def checkButtons
		#if @
		puts "CHECKBUT"
		puts @inspect.getPlayer==getMap.getPlayer
		getChild("doRecruit").setEnabled((@inspect.getPlayer==getMap.getPlayer and @inspect!=$app.hero))
		getChild("doTakeFood").setEnabled((@inspect.getPlayer==getMap.getPlayer and @inspect!=$app.hero))
		getChild("doTakeWeapons").setEnabled(false) # FIXME
		#end
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
		c=getChild("invTitle")
		if c.class==AGButton
			c.setCaption(t)
		else
			c.setText(t)
		end
	end
	def updateInspection
		if @inspect then
			res=@inspect.resource.getAll
			res["men"]=@inspect.menCount.to_f
			reset
			res.each{|a,b|
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
		end
	end
	def reset
		@resTypes.each{|x|setValue(x,0)}
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
