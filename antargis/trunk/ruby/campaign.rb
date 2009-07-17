require 'ant_local.rb'

# Campaigning
#################################
# Campaigns consist of two kinds of parts:
# - a level 
# - a cutscene
# These can be combined in random fashion.
# They're loaded from an xml-file (look at data/campaigns/* for examples)
# 
#
#

def getCampaignFiles
  Dir[File.join("data","campaigns","*.xml")]
end

def getCampaigns
  $campaigns||=getCampaignFiles.collect{|f|Campaign.new(f)}.select{|f|f.enabled}.sort_by{|campaign|campaign.order}
end

class CampaignLevel
  attr_reader :result
  def initialize(node)
    @level=node.get("file")
    node.getChildren("failscene").each{|c|
      @lostScene=CutScene.new(c)
    }
    @current=node.get("current")
    if @current==""
      @current=@level
    end
  end
  def save(n)
    n.set("file",@level)
    if @app
      raise 'Problem - $campaign not net' if $campaign==nil
      sname=$campaign.savename.gsub("antcmp","antlvl")
      n.set("current",sname)
      @app.getMap.saveMap(sname)
    end
  end


  # this function is very similar to startGame in antargis.rb
  # FIXME: join them
  def play
    loadscreen=LoadApp.new
    loadscreen.setValue(0)
    loadscreen.tick
    begin
      fileContent=loadFile(@current)
      w=getVideo.width
      h=getVideo.height
      @app=AntGameApp.new(fileContent,w,h,loadscreen)
      @app.run
      @result=@app.result
      @app=nil
      getSoundManager.stopAllChannels
    end
    $app=nil
    $map=nil
    $antView=nil
    $buttonPanel=nil
    $inventory=nil

    $screen=nil
    GC.start

    puts "RESULT.won #{@result.won}"
    puts @result
    
    case @result.won
      when "lost"
        @lostScene.play
    end

  end

  def result
    @result
  end
end

class CutSceneDisplay<AntApplication
  def initialize(image,text)
    super()
    @layout=AGLayout.new(nil)
    @layout.loadXML(loadFile("data/gui/layout/cutscene.xml"))
    setMainWidget(@layout)
    addHandler(@layout.getChild("continue"),:sigClick,:eventQuit)
    @text=@ctext=""
    @time=0
    @speed=0.1
  end
  def eventQuit
    if @finished
      tryQuit
    else
      @ctext=@text
      setTextForReal(@ctext)
      @finished=true
      #eventFrame(10000) # as if much time has passed
    end
    return true
  end
  def eventKeyDown(e)
    eventQuit
    return super
  end
  
  def setImage(image)
    w=@layout.getChild("image")
    w2=@layout.getChild("imageFrame")
    if image
      w2.show
      w.setSurface(image)
    else
      w2.hide
    end
  end
  
  def setText(text)
    @text=_(text)
    @ctext=""
    @time=0
    setTextForReal(_(""))
  end
  
  def eventFrame(t)
    t=[t,0.1].min
    @time+=t
    old=@ctext
    while @time>@speed
      len=[@ctext.length+1,@text.length].min.to_i
      puts len,len.class,@ctext,@ctext.class,@text,@text.class
  
      # FIXME: map [] to substr

      @ctext=@text[0..(len-1)]
      if old==@ctext
        break
      end
      if @ctext[-1..-1]!=AGStringUtf8.new(" ")
        @time-=@speed
      end
    end
    if old!=@ctext
      setTextForReal(@ctext)
      if @ctext==@text
        @finished=true
      end
    end
    delay(20)
    return true
  end
  
  private
  def setTextForReal(text)
    w=@layout.getChild("text")
    if text
      w.show
      w.setText(text)
    else
      w.hide
    end
  end
end

class TestClass
end

class CutScene
  #attr_reader :finished
  def initialize(node)
    @screens=[]
    @image=@text=nil
    node.getChildren("screen").each{|s|
      screen={}
      screen[:text]=""
      s.getChildren("image").each{|c|
        imageName=c.get("filename")
        screen[:imageName]=imageName
        if imageName
          screen[:image]=AGSurface.load(imageName)
        end
      }
      s.getChildren("text").each{|c|
        screen[:text]+=c.get("text")
      }
      @screens.push(screen)
    }
    #@finished="won"
  end
  def save(n)
    @screens.each{|s|
      c=n.addChild("screen")
      c.addChild("text").set("text",s[:text]) if s[:text]
      c.addChild("image").set("filename",s[:imageName]) if s[:imageName]
    }
  end
  def mytest
    puts "muh"
    #TestClass.new
  end
  def play
    display=CutSceneDisplay.new(@image,@text)
    @screens.each{|s|
      display.setImage(s[:image])
      display.setText(s[:text])
      display.run
    }
    GC.start
  end
  def result
    GameResult.new("won")
  end
end

class Campaign
  attr_reader :name, :image, :imageName, :description, :texture, :enabled, :order
  def initialize(filename)
    if not fileExists(filename)
      raise "file not found #{filename}"
    end
    @doc=Document.new(filename)
    @xmlRoot=@doc.root
    @enabled=(@xmlRoot.get("enabled")!="false")
    @name=@xmlRoot.get("name")
    @order=@xmlRoot.get("order").to_i
    @imageName=@xmlRoot.get("image")
    @image=AGSurface.load(@imageName)
    @texture=AGTexture.new(@image)
    @description=@xmlRoot.get("description")
    @partID=@xmlRoot.get("part").to_i
    
    @part=[]
    @xmlRoot.getChildren.each{|c|
      case c.getName
        when "cutscene"
          @part.push(CutScene.new(c))
        when "level"
          @part.push(CampaignLevel.new(c))
      end
    }
  end
  def savename
    @savename
  end
  def save(filename)
    @savename=filename
    doc=Document.new
    root=doc.root
    root.setName("campaign")
    root.set("name",@name)
    root.set("image",@imageName)
    root.set("description",@description)
    root.set("part",@partID.to_s)
    @part.each{|p|
      c=nil
      case p.class.to_s
        when "CutScene"
          c=root.addChild("cutscene")
        when "CampaignLevel"
          c=root.addChild("level")
        else
          raise "Unknown class type:"+p.class.to_s
      end
      p.save(c)
    }
    
    saveFile(filename,doc.toString)
  end
  
  def getCurrentPart
    @part[@partID]
  end
  def proceed
    @partID+=1
    return @partID>=@part.length
  end
  def restart
    @partID=0
  end
end

def startCampaign(campaign)
  campaign=campaign.dup # copy
  continueCampaign(campaign)
end

def continueCampaign(campaign)
  begin
    $campaign=campaign
    part=campaign.getCurrentPart
    part.play
    if part.result.won=="won"
      if campaign.proceed
        break
      end
    end
  end while part.result.won!="canceled"
  campaign.save("campaign0.antcmp") # FIXME:test save
  $campaign=nil
  puts "EXIT CAMPAIGNING"
end
