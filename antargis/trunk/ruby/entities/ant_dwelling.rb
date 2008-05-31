BIRTHING_TIME=80
MAX_RESIDENTS=10

# A dwelling has zero or more residents. Whenever there some residents they will reproduce themselves.
class AntDwelling<AntHouse
  def initialize(map)
    super
    setProvide("dwelling",true)
    @lastBirth=0
    setDirection(-20)
  end

  # when my job is finished check for a new birth
  def eventJobFinished
    super
    checkBirth
  end

  # setup the mesh
  def setupMesh
    setMesh
  end

protected
  def neededStock
    {"food"=>15}
  end

  def houseType
    "dwelling"
  end
  
private
  def checkBirth
    # only men, which are at home can add to birth rate
    @lastBirth+=[@atHome.length+resource.get("food")/3,10].min
    if @lastBirth>BIRTHING_TIME then
      man=AntMan.new
      getMap.insertEntity(man)
      man.setPos(getPos2D)
      man.setBoss(self)
      #getMap.endChange
      @lastBirth=-BIRTHING_TIME
    end

    while getMen.length>MAX_RESIDENTS
      m=getMen
      m[0..4].each{|man|
        man.setBoss(nil)
      }
    end
  end
  
end

