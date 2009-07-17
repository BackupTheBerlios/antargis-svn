#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_hero.rb
# by David Kamphausen (david.kamphausen@web.de)
#
# The "Antargis" project, including all files needed to compile it,
# is free software; you can redistribute it and/or use it and/or modify it
# under the terms of the GNU General Public License as published
# by the Free Software Foundation; either version 2 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# You should have received a copy of the GNU General Public
# License along with this program.
#

require 'entities.rb'
require 'ant_hljobs.rb'
require 'ant_boss.rb'
require 'ant_fire.rb'
require 'ant_ring.rb'
require 'ant_manbase.rb'

class AntHero<AntBoss
  include AntManBase

  attr_reader :meshState, :dead
  def initialize(map)
    super
    @fire=nil
    @fireSound=nil

    @men.push(self)
    setProvide("hero",true)
    @appearance="hero"
    getMap.setLight(self)
    setMinimapColor(AGColor.new(0xFF,0,0))
    @meshStand="stand"
    self.age=(getRand*20+20)
    setHunger(0.006)
  end
  def setAppearance(a)
    @appearance=a
  end
  
  def saveXML(node)
    super(node)
    node.set("appearance",@appearance.to_s)
  end
  def loadXML(node)
    super(node)
    @appearance=node.get("appearance")
    if @appearance==""
      @appearance="hero"
    end
    @portrait=node.get("portrait")

    if node.get("primary")=="true"
      #setMoraleHealSpeed=1000
      setMoraleFlag(false)
    end
  end
  
  def noHLJob
    if @player
      @player.assignJob(self)
      #stopFireSound
    else
      # no player , so simply rest - to infinity (or at least 5 seconds)
      newHLRestJob(5)
    end
  end
  
  def startFireSound
    if not @fireSound
      @fireSound=AntSound.playLoopSoundGlobal(self,"fire",getPos2D,0.4)
    end
  end  
  def stopFireSound
    if @fireSound
      AntSound.stopLoopSound(self)
      #getSoundManager.stopChannel(@fireSound)
      @fireSound=nil
    end
  end

  def newFightJob(d,ptarget)
    checkResources
    super
    @moving=true
    setMeshState("fight")
  end
  
  def newHLRestJob(time)
    @job.stopJob if @job
    @job=nil
    setFire(true)
    super(time)
    assignJob2All
  end
  
  def assignJob(man)
    checkHLJobEnd(man)
  end  
  def moveHome(man)
    if @formation.nil?
      raise "formation is nil!"
    end
    pos=@formation.getPosition(man)
#     pos=getSitFormation(man)
    if (man.getPos2D-pos).length>1
      man.newMoveJob(0,pos,0)
    else
      man.newRestJob(2)
    end
  end
  
  
  def newHLMoveJob(prio,pos,dist)
    @job.stopJob if @job
    @job=AntHeroMoveJob.new(self,prio,pos,dist)
    assignJob2All
  end
  def newHLRecruitJob(target)
    assert{target!=self}
    @job.stopJob if @job
    @job=AntHeroRecruitJob.new(self,target)
    assignJob2All
  end
  def newHLConstructJob(target)
    @job.stopJob if @job
    @job=AntHeroConstructJob.new(self,target)
    assignJob2All
  end
  def newHLTakeFoodJob(target)
    @job.stopJob if @job
    #puts "take food job #{self} #{target}"
    @job=AntHeroTakeJob.new(self,target,"food")
    assignJob2All
  end
  def newHLTakeWeaponJob(target)
    @job.stopJob if @job
    @job=AntHeroTakeJob.new(self,target,"weapon")
    assignJob2All
  end
  def newHLFightJob(target)
    @job.stopJob if @job
    @job=AntHeroFightJob.new(self,target)
    assignJob2All
  end
  def newHLFightAnimalJob(target)
    @job.stopJob if @job
    @job=AntHeroFightAnimalJob.new(self,target)
    assignJob2All
  end
  def newHLBuildJob(pos,type)
    @job.stopJob if @job
    
    if pos.is_a?(AGVector2) or pos.is_a?(AGVector3) 
      if getMap.getHeight(pos[0],pos[1])<=0
        # TODO: should view an error message
        messageBox(getMap.getApp,_("Warning"),_("You can't build on water."),MB_OK)
        return
      end
    end
    
    if pos.is_a?(AGVector2) or pos.is_a?(AGVector3)
      assert{pos.is_a?(AGVector2) or pos.is_a?(AGVector3)}
      target=AntBuildingSite.new(getMap)
      target.setPos(pos)
      target.building=type
      getMap.insertEntity(target)
      @job=AntHeroBuildJob.new(self,target) #pos,type)
    else
      assert{pos.is_a?(AntBuildingSite)}
      @job=AntHeroBuildJob.new(self,pos)
    end
    assignJob2All
  end
  
  def newHLDismissJob()
    @job.stopJob if @job
    @job=nil
    agg=getAggression
    men=@men.select{|m|not m.is_a?(AntHero)} # exclude hero
    c=(men.length-1)*agg/3
      
    men=men[0..c]
    men.each{|m|
      # seek new boss
      b=getMap.getNextList(m,"house",0).collect{|e|log e,e.class;e.get}.select{|e|e.getPlayer==getPlayer}.shuffle[0]
      if b
        m.setBoss(b)
      else
        m.setNoBoss
      end
      @men.delete(m)
      m.delJob
    }
    getMap.eventHLDismissed(self)
  end
  
  def upgradeMan(man)
    # copy properties through xml-node  
    xmlnode=Node.new("test")
    man.setNoBoss
    man.saveXML(xmlnode)
    @@tmpName||=1
    @@tmpName+=1
    man.setName("xxxxxxxx #{@@tmpName}") # reset name - so there's some distinction
          
    nhero=AntHero.new
    nhero.preloadXML(xmlnode)
    nhero.loadXML(xmlnode)
    nhero.eventMapChanged
    removeMan(man)
    getMap.removeEntity(man)
    getMap.insertEntity(nhero)
    #getMap.endChange # commit
    nhero.newRestJob(0)
    newHLRestJob(1)
    #nhero.setName("Sir "+nhero.getName)
    nhero.setOwner(self)
    nhero.experience=0
  end
  
  
  def setFire(flag)
    if flag
      if getPos3D.z>0 # won't start fire in water!!
        if not @fire
          @fire=AntFire.new(getMap)
          @fire.setPos(getPos3D+AGVector3.new(0.7,0,0))
          getMap.insertEntity(@fire)
        end
        startFireSound
      end
    else
      #raise 1
      if @fire
        @fire.disable
        @fire=false
      end
      stopFireSound
    end
  end

  def eventAttacked(by)
    super
  end
  
  def assignJob2All
    super
    if @job.class!=AntHeroRestJob
      setFire(false)
    end
    doEvent(:newJobAssigned)
  end

  def setMeshState(name)

    if name=="row"
      name="walk"
    end

    @origMeshState=name
    name=checkOnWater(name)
    @meshState=name
    dir=getDirection
    case name
      when "row"
        setMesh
        addMesh(AntModels.createModel(self,:boat),AGVector3.new(0,0,0))
      when "dead"
        setMesh(:grave_hero)
      else
        setMesh
    end
    setDirection(dir)
    setupRing

  end



   def setupMesh
    setMeshState("normal")
  end
  def getRing
    makeRingMesh
  end

  def getHero
    self
  end

  def getImage
    @portrait||=""
    r=nil

    filename=""
    if @portrait.length!=0
      filename=@portrait
      #r=getTextureCache.get(@portrait)
    else
      log "no portrait defined"
      filename="data/gui/portraits/#{getName}.png"
    end
    if fileExists(filename)
      r=getTextureCache.get(filename)
    else
      r=getTextureCache.get("data/gui/portraits/none.png")
    end

    return r
  end
  def getDescription
    nameage=_("This is {1}. He is {2} years old.",getName,age)
    support=nil
    if getMen.length==1
      support=_("Nobody supports him.")
    else
      support=_("{1} men support him.",getMen.length-1)
    end
    nameage+" "+support
  end

  def spreadResources
    # FIXME
  end

  def eventDie
    super
    setFire(false)
    @job=nil
    if @player
      @player.remove(self)
    end
    getMap.eventHeroDied(self)
  end

end

