# this file contains:
# * AntButtonPanel - the button panel on the right of the game-screen containing the jobButtons
# * AntButtonPanelCreator - a creator-class for AntButtonPanel (needed for layouts to create such a widget)


# AntButtonPanel is the widget that's displayed on the top left of the screen.
# It contains some (hl-)job-buttons, a display of the current job, the selected heroes is doing and
# the name of this hero.
# It's embedded through a layout into the screen and thus needs a creator class named AntButtonPanelCreator.
#
# Important note:
# This class contains two signals named
# * sigAggressionChanged
# * sigJobChanged
# You can attach your listeners to these to receive events when the aggression or a job is changed.
class AntButtonPanel<AGWidget
	attr_reader :job
	

	def initialize(p,r)
		super(p,r)
		setName("ButtonPanel")
		clearHandlers
		@jobButtons=["doRest","doDismiss","doDropFood","doDropWeapon","doBuild"]
		@aggButtons={"doAgg0"=>1,"doAgg1"=>2,"doAgg2"=>3}
		@inited=false
		@agg=1

		addSignal("sigAggressionChanged")
		addSignal("sigJobChanged")

		@hero=nil
	end

	# set the current hero *h* of class AntHero
	def setHero(h)
		assert{h.is_a?(AntHero) || h.nil?}
		@hero=h
		if @hero
			setName(@hero.getName)
		else
			setName(_("no hero"))
		end
	end


	# event-handlers can't be initialized in the constructor initialize, because
	# possible children are not yet created.
	# *initHandlers* initializes event-handlers for the job- and aggression-selection buttons.
	# FIXME: this function should only be called once ! may be check this
	def initHandlers
		getChild("doAgg0").setChecked(true)
		@job="doRest"
		@jobButtons.each {|b|
			c=getChild(b)
			addHandler(c,:sigClick,:eventJobSelected)
		}
		@aggButtons.each {|b,a|
			c=getChild(b)
			addHandler(c,:sigClick,:eventAggSelected)
		}
	end

	# checks if the view has to be updated
	def prepareDraw
		updateJobView
		super
	end

	
	def eventJobSelected(e)
		@job=e.getCaller.getName
		sigJobChanged(e)
		return true
	end

	# another aggression was selected, so put it forward to this signal *sigAggressionChanged*
	def eventAggSelected(e)
		@agg=@aggButtons[e.getCaller.getName]
		sigAggressionChanged(e)
		return true
	end
	
	# force setting an aggresion 
	# FIXME: which aggressions are ok ??? Check this in here!
	def setAggression(l)
		l=l.to_i
		@aggButtons.each{|n,b|
			getChild(n).setChecked((b==l))
		}
		@agg=l
	end

	# get the current aggression
	def getAggression
		@agg
	end
	def setPointing
		@job="doPoint"
		getChild("doFight").setChecked(true)
	end
private
	def updateJobView
		return if @hero.nil?
		if @hero.getJob
			i=@hero.getJob.image
		else
			i="data/gui/bed.png"
		end
		if @job!=i
			@job=i
			getChild("jobView").setTexture(getTextureCache.get(i))
		end
	end

	# set another name for the hero-name-display
	def setName(n)
		if getChild("heroName")
			getChild("heroName").setText(_(n))
		end
	end

end

# factory for buttonpanel-widget
class AntButtonPanelCreator<AGLayoutCreator
	def create(p,r,n)
		setResult AntButtonPanel.new(p,r)
	end
end
getLayoutFactory.addCreator("antButtonPanel",AntButtonPanelCreator.new)
