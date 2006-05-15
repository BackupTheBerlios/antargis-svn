#!/usr/bin/env ruby

class Level
	def initialize
	end

	def eventTrigger(hero,t)
		return false
	end
	def eventOwnerChanged(ent)
	end
	def eventHeroDied(ent)
	end
	def eventHLJobFinished(hero,job)
	end
	def eventDismissed(hero)
	end
	def eventLevelStarted
	end

	def saveXML(node)
		saveLocals(node)
	end
	def loadXML(node)
		loadLocals(node)
	end

	# use this to end a level
	def wonLevel
		puts "WON!!!!!!!!!!!!!!!!!!"
		$app.setResult(GameResult.new("won"))
	end
	def lostLevel
		$app.setResult(GameResult.new("lost"))
	end
	def endLevel
		$app.tryQuit
	end
	def tellStory(flow)
		$app.storyTalk(flow)
		@story=flow
	end

private
	def getLocals
		a={}
		instance_variables.each{|v|
			a[v]=instance_variable_get(v)
		}
		return a
	end
	def saveLocals(node)
		getLocals.each{|n,v|
			c=node.addChild("local")
			c.set("name",n)
			c.set("type",v.class.to_s)
			c.set("value",v.to_s)
		}
	end
	def loadLocals(node)
		node.getChildren("local").each{|c|
			n=c.get("name")
			v=nil
			case c.get("type")
				when "TrueClass"
					v=true
				when "FalseClass"
					v=false
				when "Fixnum","Bignum"
					v=c.get("value").to_i
				when "Float"
					v=c.get("value").to_f
				when "String"
					v=c.get("value")
			end
			instance_variable_set(n,v)
		}
	end
end