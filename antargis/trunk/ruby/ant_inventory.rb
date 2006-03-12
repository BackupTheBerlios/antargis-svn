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
	end

	def eventInspect
		$app.viewInformation(@inspect)
	end

	def setValue(name,value)
		ok=@resTypes+["boss"]
		if ok.member?(name) then
			w=getChild(name)
			if value.class==String
				w.setText value
			else
				w.setText value.to_i.to_s
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
	def inspect(e)
		setTitle(e.class.to_s) #getType)
		@inspect=e
	end
	def draw(p)
		updateInspection
		super(p)
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
