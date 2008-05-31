class AntHeroBuildJob<AntHeroMoveJob
  FARTHEST_AWAY=40

  attr_reader :finished
  def initialize(hero,targetpos,building)


    tpos=targetpos
    tpos=targetpos.getPos2D if targetpos.is_a?(AntBuildingSite)
    super(hero,0,tpos,5)
    @targetpos=tpos
    @building=building
    @usedmen=0

    @restype={}
    
    @flat=0

    if targetpos.is_a?(AntBuildingSite)
      @target=targetpos
      @building=@target.building
    else
      # make buildingsite
      @target=AntBuildingSite.new(getMap)
      @target.setPos(targetpos)
      @target.building=building
      getMap.insertEntity(@target)
    end
  end
  def image
    "data/gui/build.png"
  end
  def check(man)
    if moveFinished
      @hero.formation=AntFormationRest.new(@hero)
      if man.is_a?(AntBoss)
        checkEat(man)
        if not @constructStarted
          @men.each{|m|m.delJob}
          @constructStarted=true
        end
        man.newRestJob(20) # do nothing at all
        return
      end
      @constructStarted=true
      wantmen=(@men.length-1)*@hero.getAggression/3.0
      case man.getMode
        when "fetch" # go to resource
          res=getNeededResources
          res,nearest=getNextWithResource(res)
          @restype[man]=[res,nearest]
          if nearest.nil?
            puts "resource not found! #{res}"
            man.newRestJob(5)
          else
            if (nearest.getPos2D-@target.getPos2D).length<FARTHEST_AWAY
              man.newMoveJob(0,nearest.getPos2D,0.5) # near but not actually there
              man.setMode("harvest")
            else
              man.newRestJob(1)
              puts "TOO FAR"
            end
          end
        when "harvest" # harvest resource
          if (@restype[man][1].getPos2D-man.getPos2D).length<3
            man.newRestJob(2)
            man.digResource(@restype[man][0])
            man.setMode("collect")
          else
            puts "WAS TOO FAR AWAY - Probably job discarded"
            man.newMoveJob(0,@target.getPos2D,0)
            man.setMode("")
          end
        when "collect" # bring back
          # FIXME: add sub resource from nearest
          p=@restype[man]
          amount=[p[1].resource.get(p[0]),man.canCarry].min
          p[1].resource.sub(p[0],amount)
          man.resource.add(p[0],amount)
          man.newMoveJob(0,@target.getPos2D,0)
          man.collectResource(@restype[man][0])
          man.setMode("brought")
        when "brought"
          man.setMode("after_brought")
          man.newRestJob(1)
          # take all natural resources - but not food and weapons - otherwise men starve!
          myResources.each{|r|
            @target.resource.take(man.resource,r)
          }
        when "after_brought"
          fpos=@hero.getFormation(man,@hero.getPos2D)
          man.newMoveJob(0,fpos,0)
          man.setMode("construct_torest")
        when "goto_construct"
          man.newMoveJob(0,@targetpos,0)
          man.setMode("to_construct")
        when "to_construct"  # do some constructing
          #@target.incSmoke
          man.newRestJob(5 - @hero.getAggression*0.5) # work for 3.5-5 seconds (depending on aggression)
          man.setMode("constructing")
          man.playSound("construct")
          man.setMeshState("pick") # FIXME: add hammering
        when "constructing"
          # was constructing
          readyConstructed
          man.incExperience(man.learnAmount)
          #@target.decSmoke
          man.setMode("construct_torest")
          fpos=@hero.getFormation(man,@hero.getPos2D)
          man.newMoveJob(0,fpos,0)
        when "construct_torest"
          man.setMode("rest")
          man.newRestJob(3/@hero.getAggression) # shorter pauses when aggression is higher
          if man.getAggression<3
            checkEat(man)
          end
          @usedmen-=1
        else
          if wantmen>@usedmen
            @usedmen+=1
            man.newMoveJob(0,@targetpos,0)
            if enoughResources
              man.setMode("goto_construct")
            else
              man.setMode("fetch")
            end
          end
      end
    else
      super(man)
    end
  end
private
  def myResources
    ["wood","stone"]
  end
  def enoughResources
    # FIXME support more resources
    m=myResources.min{|a,b|@target.resource.get(a)<=>@target.resource.get(b)}
    value=@target.resource.get(m)
    return value>5
    #(@target.resource.get("stone")>5 and @target.resource.get("wood")>5)
  end
  def getNeededResources
    m=myResources.sort{|a,b|@target.resource.get(a)<=>@target.resource.get(b)}
    puts "NEEDED: #{m}"
    return m
  end
  def getNextWithResource(res)
    #goods={"wood"=>"tree","stone"=>"stone","food"=>"tree","coal"=>"mine","ore"=>"mine"}
    #enttype=goods[res]
    ret=nil
    res.each{|r|
      ret=getMap.getNext(@target,r,1)
      if not ret.nil?
        ret=[r,ret]
        break
      end
    }

    ret
  end
  def checkEat(man)
    puts "CHECKEAT"
    if man.getFood<0.5
      if man.resource.get("food")>0
        man.incFood(1)
        man.resource.sub("food",1)
      elsif heroHasFood
        man.incFood(1)
        @hero.resource.sub("food",1)
      end
    end
  end
  def heroHasFood
    @hero.resource.get("food")>0
  end
  def readyConstructed

    px=@targetpos.x.to_i+1
    py=@targetpos.y.to_i+1
    if @flatpositions.nil?
      @flatpositions=[]
      (-3..3).each{|y|
        (-3..3).each{|x|
          v=1-Math::sqrt(x**2+y**2)/4.0
          v=[0,1,v*1.5].sort[1]
          @flatpositions.push([x+px,y+py,v])
        }
      }
      @flatheight=getMap.get(px,py)
      @flatpositions.shuffle
    end


    
    if @flatpositions.length>0
      (0..1).each{|i|
        break if @flatpositions.length==0
        p=@flatpositions.shift
        v=getMap.get(p[0],p[1])*(1-p[2])+@flatheight*p[2]
        getMap.set(p[0],p[1],v)
        v=getMap.getTerrain(p[0],p[1],EARTH)*(1-p[2])+p[2]
        getMap.setTerrain(p[0],p[1],EARTH,v)
      }
      getMap.endChange
      return
    end



    #neededResources={"stone"=>2,"wood"=>2} # FIXME: depends on house-type
    neededResources=@building.buildResources
  
    neededResources.each{|k,v|
      if @target.resource.get(k)<v
        return # oooooh
      end
    }
    neededResources.each{|k,v|
      @target.resource.sub(k,v)
    }
    @target.incProgress(@building.buildSteps)
    
    if @target.ready
      # delete buildingsite and replace with building
      getMap.removeEntity(@target)
      house=@building.new
      house.setPos(@targetpos)
      getMap.insertEntity(house)
      house.setPlayer(@hero.getPlayer)
      house.setName(house.class.to_s.gsub("Ant",""))
      house.resource.takeAll(@target.resource) # give remaining resources to house
      @finished=true
    end
  end
end
