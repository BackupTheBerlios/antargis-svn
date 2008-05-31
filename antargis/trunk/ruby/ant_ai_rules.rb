#
# Copyright (c) 2006 by David Kamphausen. All rights reserved.
#
# ant_ai_rules.rb
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

#
# Simple rules-based system:
# A rule consists of a condition and one or more actions
# These rules are processed in pre-defined order - look at ant_ai.rb for more
#


class AICondition
  attr_accessor :interface
  def value(hero)
    return 1
  end
end

class AIConditionFoodLow<AICondition
  def value(hero)
    puts "FOOD LOW ???"
    if hero.resources("food")<5
      puts "YES"
      if hero.findNearResource("food").nil?
        puts "BUT NOTHING NEAR!!"
        if hero.resources("food")<1
          puts "TOO LAZY TO WALK _ just dying!!"
          return 1
        end
        return 0
      end
      return 1
    else
      puts "NO"
      return 0
    end
  end
end

class AIEveryNthCondition<AICondition
  def initialize(nth)
    @nth=nth
    @v=0
  end
  def value(hero)
    @v+=1
    @v%=@nth
    return @v==0?1:0
  end
end


# Dummy Action - does nothing at all
class AIAction
  attr_accessor :interface
  def execute(interface,hero)
  end
end

# get food from some building which is owned by the hero
class AIGatherFoodAction<AIAction
  def execute(hero)
    unit=hero.findNearResource("food")
    puts "HERO TAKES FOOD"
    hero.takeFood(unit)
  end
end

# rest for a second
class AIRestAction<AIAction
  def execute(hero)
    hero.rest(10)
  end
end

class AIAttackAction<AIAction
  def execute(hero)
    e=findUsableEnemy(hero)
    if e
      hero.attack(e)
    end
  end
  private
  def findUsableEnemy(hero)
    enemyPlayers=@interface.enemyPlayers
    allEnemyHeroes=enemyPlayers.collect{|p|p.getHeroes+p.getBuildings}.flatten
    allEnemyHeroes.sort!{|a,b|a.menCount<=>b.menCount}
    e=allEnemyHeroes[0]
    if e
      if e.menCount>hero.menCount
        e=nil
      end
    end
    e
  end
end

# build up a rule out of a condition and one or more actions:
# AIRule.new(AICondition.new,AIRestAction.new)
# or
# AIRule.new(AICondition.new,[AIRestAction.new,Someotheraction.new])
class AIRule
  def initialize(condition,actions,interface)
    @c=condition
    @a=actions
    @c.interface=interface
    @a.interface=interface
  end
  def value(hero)
    @c.value(hero)
  end
  def execute(hero)
    if @a.is_a?(Array)
      @a.each{|a|a.execute(hero)}
    else
      @a.execute(hero)
    end
  end
end