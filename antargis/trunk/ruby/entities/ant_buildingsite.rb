
class AntBuildingSite<AntRubyEntity
  attr_accessor :building
  def initialize(map)
    super(map)
    @progress=0
    #init fieldMeshes-var
    setupMesh
    newRestJob(300) # wait at least 5:00 minutes
    @doneSth=true
  end

  def incProgress(steps)
    @steps=steps
    o=@progress.to_i
    @progress+=(1.0/steps)*getStepCount
    pp @progress
    if o!=@progress.to_i
      setupMesh
    end
    @doneSth=true
  end
  def ready
    @progress>AntModels.getMeshCount(:buildingsite)
  end

  # removes building site if nothing was done in some time
  def eventJobFinished
    if @doneSth
      newRestJob(30) # wait another 30 seconds
    else
      getMap.removeEntity(self) # remove building site
    end
    @doneSth=false
  end

  def getDescription
    a=_(building.to_s.gsub("Ant",""))
    d=_("This will be a {1}.",a)
    if @steps
      b=((getStepCount-1)-@progress)*@steps/(getStepCount-1)
      b=b.to_i.to_s
      d+=_("It will be ready after {1} more steps.",b)
    end
    d
  end

#  private
  def setupMesh
    pp @progress.to_i
    setMesh(@progress.to_i)
  end

  def getStepCount
    AntModels.getMeshCount(:buildingsite)
  end
end
