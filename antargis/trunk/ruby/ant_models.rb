require 'pp'
require 'meshes/grass.rb'

module AntModels
  @@useImpostors=false
  
  def AntModels.getStaticModelRotation(name)
    rot={
      "tower"=>-30,
      "ant_mill"=>-30,
      "mine2"=>-40,
      "workshop"=>-50,
      "townhall_try2"=>-60,
      "well"=>-30,
      "ant_bakery"=>-30,
      "ant_coach"=>-50,
      "livinghouse"=>-20,
      "farm"=>-30,
    }
  
    if rot.member?(name)
      return rot[name]
    end
    return 0
  end
  
  def AntModels.getStaticModelScaling(name)
    scales={
      "sack"=>0.3,
      "stub"=>0.04,
      "tower"=>3,
      "ant_mill"=>3.2,
      "mine2"=>0.2,
      "workshop"=>0.18,
      "townhall_try2"=>3.2,
      "well"=>0.06,
      "rip"=>0.3,
      "fish"=>0.02,
      "arrow"=>0.1,
      "ant_bakery"=>3.0,
      "boat_simple"=>0.12,
      "twig"=>0.7,
      "ant_coach"=>0.08,
      "druid_lp"=>0.08,
      "smith_lp"=>0.08,
      "livinghouse"=>0.16,
      "farm"=>0.2,
      "fire"=>0.3,
      "big_stone"=>0.7,
      "grave"=>0.2,
      "hero_lp"=>0.08,
    }
  
    if scales.member?(name)
      return scales[name]
    end
    return 1
  end

  def AntModels.generateMesh(entity,size)
    case entity
      when AntBush
        makeBushMesh(entity.getMap.getScene,size*3)
      when AntGrass
        makeGrassMesh(entity.getMap.getScene,size)
      else
        nil
    end
    
  end
  
  def AntModels.getMeshMap
    animMeshes={
      :man=>{
        []=>"data/models/man_e_walk.anim",
        ["wood"]=>"data/models/man_e_wood.anim",
        ["stone","flour","corn"]=>"data/models/man_e_stones.anim",
        ["walk","sitdown","sit","stand"]=>"data/models/man_e_walk.anim",
        ["fight_dagger"]=>"data/models/man_e_dagger.anim",
        ["fight_shield"]=>"data/models/man_e_shield.anim",
        ["fight_sword"]=>"data/models/man_e_sword.anim",
        ["fight_bow"]=>"data/models/man_e_bow.anim",
        ["pick"]=>"data/models/man_pick.anim",
        ["axe"]=>"data/models/man_axe.anim",
        [:grave]=>"grave",
      },
      :grave=>{
        [:normal]=>"grave",
        [:hero]=>["data/models/grave.ant2",0.2,"data/textures/models/grave3.png"],
      },
      :hero=>{
        []=>"hero_lp",
        [:grave_hero]=>["data/models/grave.ant2",0.2,"data/textures/models/grave3.png"],
      },
      :sack=>{
        []=>"sack"
      },
      :tree=>{
        []=>["data/models/fir2.ant2",0.45,"data/textures/models/fir5.png",false],
        ["stub"]=>"stub",
        [0]=>["data/models/fir2.ant2",0.45,"data/textures/models/fir5.png",false],
        [1]=>["data/models/fir2.ant2",0.45,"data/textures/models/fir7.png",false],
        [2]=>["data/models/tree5.ant2",0.45,"data/textures/models/tree3.png",false],
        [3]=>["data/models/tree5.ant2",0.45,"data/textures/models/tree5.png",false],
        [4]=>["data/models/tree5.ant2",0.45,"data/textures/models/tree9.png",false],
        [5]=>["data/models/tree6.ant2",0.45,"data/textures/models/tree5.png",false],
        [6]=>["data/models/tree5.ant2",0.45,"data/textures/models/tree10.png",false],
        [7]=>["data/models/tree_simple1.ant2",0.3,"data/textures/models/tree_simple1.png"],
        [8]=>["data/models/tree_simple2.ant2",0.3,"data/textures/models/tree_simple1.png"],
        [9]=>["data/models/tree_simple5.ant2",0.3,"data/textures/models/tree_simple5.png"]
      },
      :buildingsite=>{
        []=>["data/models/building_site0.ant2",1.7,"data/textures/models/building_site0.png"],
        [0]=>["data/models/building_site0.ant2",1.7,"data/textures/models/building_site0.png"],
        [1]=>["data/models/building_site1.ant2",1.7,"data/textures/models/building_site1.png"],
        [2]=>["data/models/building_site2.ant2",1.7,"data/textures/models/building_site1.png"],
      },
      :field=>{
        []=>["data/models/crop_tiny.ant2",2.2,"data/textures/models/crop_tiny.png"],
        [0]=>["data/models/crop_tiny.ant2",2.2,"data/textures/models/crop_tiny.png"],
        [1]=>["data/models/crop_small.ant2",2.2,"data/textures/models/crop_green.png"],
        [2]=>["data/models/crop_med.ant2",2.2,"data/textures/models/crop_yellow.png"],
        [3]=>["data/models/crop_high.ant2",2.2,"data/textures/models/crop_gold.png"]
      },
      :tower=>{
        []=>"tower"
      },
      :wolf=>{
        []=>"ant_wolf",
        [:rip]=>"rip"
      },
      :mill=>{
        []=>"ant_mill"
      },
      :mine=>{
        []=>"mine2"
      },
      :workshop=>{
        []=>"workshop"
      },
      :townhall=>{
        []=>"townhall_try2"
      },
      :well=>{
        []=>"well"
      },
      :fish=>{
        []=>"fish"
      },
      :sheep=>{
        []=>"data/models/sheep.anim",
        [:rip]=>"rip"
      },
      :arrow=>{
        []=>"arrow"
      },
      :bakery=>{
        []=>"ant_bakery"
      },
      :boat=>{
        []=>"boat_simple"
      },
      :deco=>{
        [:floor_deco]=>["data/models/floor.ant2",0.5,"data/textures/splats/stones1a.png"],
      },
       :twig=>{  
         []=>"twig"
       },
      :decomesh=>{ # FIXME: remove this and the class, too ??
        []=>"ant_coach"
      },
      :coach=>{
        []=>"ant_coach"
      },
      :decal=>
      {
        [:floor_gravel]=>["data/models/floor.ant2",0.8,"data/textures/gravel4.png"],
      },
      :npc=>{
        [:druid]=>"druid_lp",
        [:smith]=>"smith_lp"
      },  
      :dwelling=>
      {
        []=>"livinghouse"
      },
      :farm=>
      {
        []=>"farm"
      },  
      :fire=>
      {
        []=>["data/models/fire.ant2",0.3,"data/textures/models/fire.png"],
        [:on]=>["data/models/fire.ant2",0.3,"data/textures/models/fire.png"],
        [:off]=>["data/models/fire.ant2",0.3,"data/textures/models/fire2.png"]
      },
      :fishinghut=>
      {
        []=>["data/models/fishing_hut.ant2",3.2,"data/textures/models/fishing_hut.png",false]
      },
      :stone=>
      {
        []=>"big_stone"
      },
    }
  end
  
  def AntModels.getMeshCount(entityType)
    getMeshMap[entityType].length
  end
  
  def AntModels.createModel(entity,entityType,subType=nil,angle=nil)
    assert {entity.is_a?(AntRubyEntity)}
    
    generatedMesh=generateMesh(entity,subType)
    return generatedMesh if generatedMesh

    assert {entityType.is_a?(Symbol)}
    mesh=nil
  
    animMeshes=getMeshMap
    #raise "muh" unless animMeshes.member?(entityType)
    assert{animMeshes.keys.member?(entityType)}
    map=animMeshes[entityType]
  
    map.each{|k,v|
      if k.member?(subType) or k.length==0
        mesh=v
      end
    }
    scenenode=nil
    if mesh
      if mesh.is_a?(Array)
        pp mesh
        ant2name=mesh[0]
        scale=mesh[1]
        pngname=mesh[2]
        culling=true
        if mesh.length>3
          culling=mesh[3]
        end
        data=getMeshData(ant2name,scale,pngname)
        data.setCulling(culling)
        data.setTransparent(true)
        name=mesh
        angle||=getStaticModelRotation(name)
        scenenode=Mesh.new(entity.getMap.getScene,data,AGVector4.new(0,0,0),angle)
            
      elsif mesh=~/anim$/
        scenenode=AnimMesh.new(entity.getMap.getScene,getAnimMeshData(mesh))
      else
        ant2name="data/models/"+mesh+".ant2"
        pngname="data/textures/models/"+mesh+".png"
        pngname="" unless fileExists(pngname)
        name=mesh
        angle||=getStaticModelRotation(name)

        s=entity.getMap.getScene

        data=getMeshData(ant2name,getStaticModelScaling(name),pngname)
        v=AGVector4.new(0,0,0)
        scenenode=Mesh.new(s,data,v,angle)
      end
    end
  
    if @@useImpostors
      # FIXME:replace by impostor
      if entityType!=:fish
        return createModel(:fish)
      end
    end
  
    return scenenode
  end
end

module Libantargis
  # dummy function - so that for meshes and animmeshes you can call setAnimation(.)
  class Mesh
    def setAnimation(name)
    end
  end
end