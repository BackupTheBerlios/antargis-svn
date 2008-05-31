class AntGrave<AntRubyEntity
  TYPES=[:normal,:hero]

  def initialize(map)
    super
    @type=:normal
    setMesh(@type)
    @waited=false
  end
  def type=(type)
    if TYPES.member?(type)
      @type=type
      setMesh(@type)
    end
  end
  def eventNoJob
    if @waited
      getMap.removeEntity(self)
      return
    else
      newRestJob(40)
      @waited=true
    end
  end
end