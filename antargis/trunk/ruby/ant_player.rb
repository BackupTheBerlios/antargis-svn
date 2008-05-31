#
# Copyright (c) 2005 by David Kamphausen. All rights reserved.
#
# ant_player.rb
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

class AntPlayer
  attr_accessor :name
  def initialize(map,name)
    assert{map.is_a?(AntMap)}
    @map=map
    @heroes=[]
    @heronames=[]
    @name=name
  end
  def eventJobFinished
  end
  def saveXML(n)
    n.set("name",@name)
    @heroes.each{|hero|
      c=n.addChild("hero")
      c.set("name",hero.getName)
    }
  end
  def loadXML(n)
    @name=n.get("name")
    children=n.getChildren("hero")
    children.each{|c|
      @heronames.push(c.get("name"))
    }
  end
  def move(time)
    if @heronames.length then
      @heronames.each{|n|
        ent=getMap.getByName(n)
        if ent.is_a?(AntBoss)
          ent.setPlayer(self)
          @heroes.push(ent)
        else
          puts "HERO NOT FOUND: '#{n}'"
        end
      }
      @heronames.clear
    end
  end
  def getName
    @name
  end
  
  def assignJob(hero)
    hero.newHLRestJob(20)
  end
  def trigger(ent,tr)
  end
  def remove(hero)
    @heroes.delete(hero)
  end
  def add(hero)
    @heroes.push(hero)
    @heroes.uniq!
  end
  def getBuildings
    getBosses.select{|boss|boss.is_a?(AntHouse)}
  end
  def getHeroes
    getBosses.select{|boss|boss.is_a?(AntHero)}
  end
  def getBosses
    @heroes.uniq!
    @heroes.clone
  end
  def getMap
    @map
  end
end

class AntHumanPlayer<AntPlayer
  def eventJobFinished(who,what)
    puts who.to_s+" is ready with is job:"+what.to_s
  end
  def xmlName
    return "humanPlayer"
  end
  def assignJob(hero)
    hero.newHLRestJob(20)
  end
end

class AntComputerPlayer<AntPlayer
  def initialize(map,name)
    super
    @doneSth={}
  end

  def xmlName
    return "computerPlayer"
  end
  def eventJobFinished(who,what)
    puts who.to_s+" is ready with is job:"+what.to_s
  end
  def assignJob(hero)
    if enoughFood(hero)
      hero.newHLRestJob(20)
    end
  end
  
  # simple trigger
  def trigger(ent,tr)
    if tr.player!=@name
      return
    end
    if ent.getPlayer!=self and tr.name=="attack"
      @heroes.each{|h|
        if h.getJob.class==AntHeroRestJob
          puts "ASSIGNING!"
          h.newHLFightJob(ent)
        end
      }
    end
  end
  def attackNextHero(hero)
    puts "ANTPLAER::assignJob"
    
    # simply attack
    target=getMap.getNext(hero,"hero")
    
    if target and target.getPlayer!=self
      hero.newHLFightJob(target)
    else
      house=getMap.getNext(hero,"house")
      housePos=house.getPos2D
      heroPos=hero.getPos2D
      diff=housePos-heroPos
      if diff.length2>150
        hero.newHLMoveJob(0,house.getPos2D,100)
      else
        hero.newHLRestJob(20)
      end
    end
  end
  def getNextHome(hero)
    getMap.getNext(hero,"house")
  end
  def goHome(hero,home)
    housePos=house.getPos2D
    heroPos=hero.getPos2D
    diff=housePos-heroPos
    if diff.length2>4
      hero.newHLMoveJob(0,house.getPos2D,2)
    end
  end
  def attack(hero,target)
    if hero and target and target.getPlayer!=self
      hero.newHLFightJob(target)
    end
  end
  
  def attackChecked(hero,target)
    if [AntHeroRestJob,NilClass].member?(hero.getJob.class)
      attack(hero,target)
    end
  end
  def getNextEnemy(hero)
    getNextEnemyX(hero,"hero")
  end
  def getNextEnemyHouse(hero)
    getNextEnemyX(hero,"house")
  end

  def getNextEnemyX(hero,name,tries=10)
    while tries>0
      e=getMap.getNext(hero,name,0)
      if e.getPlayer!=self
        return e
      end
      tries-=1
    end
    return nil
  end
  

  def enoughFood(hero)
    if hero.resource.get("food")<2
      tryGetFood(hero)
      return false
    end
    return true
  end
  def tryGetFood(hero)
    foodSource=getSource(hero,"food",1)
    if foodSource.nil?
      hero.newHLRestJob(1)
    else
      hero.newHLTakeFoodJob(foodSource)
      @doneSth[hero]=true
    end
  end
  def getSource(hero,what,atleast,tries=5)
    while tries>0
      e=getMap.getNext(hero,what,atleast)
      if e.nil?
        puts "NO SOURCE"
        #return nil
      elsif e.getPlayer==self
        return e
      end
      tries-=1
    end
    puts "OUT OF TRIES"
  end

end

class AntLazyPlayer<AntComputerPlayer
end

class AntOldConqueringPlayer<AntComputerPlayer
  def initialize(map,name)
    super
    @mode=:rest
  end
  def assignJob(hero)
    case @mode
      when :rest
        hero.newHLRestJob(50)
        @mode=:fight
      when :fight
        attackChecked(hero,getNextEnemyHouse(hero))
        @mode=:rest
    end
  end
end

class AntConqueringPlayer<AntComputerPlayer
  def initialize(map,name)
    super
    @mode=:rest
  end
  def assignJob(hero)
    puts "-------------------------------------------------assignJob(hero)"
    puts "assignJob(hero)"
    puts "-------------------------------------------------assignJob(hero)"
    if @doneSth[hero]
      hero.newHLRestJob(3)
      @doneSth[hero]=nil
      return
    end
    if enoughFood(hero)
      if not shouldAttackEnemyHouse(hero)
        case @mode
          when :rest
            hero.newHLRestJob(50)
            @mode=:fight
          when :fight
            attackChecked(hero,getNextEnemyHouse(hero))
            @mode=:rest
        end
      end
    end
  end

  def shouldAttackEnemyHouse(hero)
    puts "def attackEnemyHouse(hero)"
    house=getWeakestEnemyHouse(hero)
    if house
      puts "HOUSE:"
      puts house
      puts "#{house.getMen.length}<#{hero.getMen.length}"
      if house.getMen.length<hero.getMen.length
        puts "AATTTAC"
        attack(hero,house)
        return true
      end
    end
    return false
  end

  def getWeakestEnemyHouse(hero)
    hs=getMap.getEntities("house")
    hs=hs.collect{|h|h.get}.select{|h|h.getPlayer!=self}.sort{|a,b|a.getMen.length<=>b.getMen.length}
    hs[0]
  end



end
