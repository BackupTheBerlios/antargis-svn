class Story
  
end

StoryScreen=Struct.new(:imageFilename,:text)

class Story
  attr_reader :screens
  attr_accessor :x, :y
  def initialize
    @screens=[]
  end
end

Level=Struct.new(:filename,:x,:y)
Edge=Struct.new(:from,:to,:name)

class Campaign
  def self.getAllFilenames
    dir="data/campaigns"
    getDirectory(dir).uniq.select{|f|f=~/\.xml/}    
  end
  
  def self.getAll
    getAllFilenames.map{|file|self.new(dir+"/"+file)}
  end
  
  def initialize(filename)
    c=loadFile(filename)
    d=Document.new(filename)
    hasEdges=(d.root.getChildren("edges").length>0)
    @nodes={}
    @edges=[]
    @startNode=nil
    @vars={}
    
    varNames.each{|n|@vars[n]=d.root.get(n)}
    
    d.root.getChildren.each{|child|
      #pp child.getName
      node=nil
      case child.getName
        when "cutscene"
          story=Story.new
          child.getChildren("screen").each{|screenNode|
            imageNode=screenNode.getChildren("image")[0]
            textNode=screenNode.getChildren("text")[0]
            image=nil
            text=""
            image=imageNode.get("filename") if imageNode
            text=textNode.get("text") if textNode
            story.screens << StoryScreen.new(image,text)
          }
          node=story
        when "level"
          node=Level.new(child.get("file"))    
      end
      if node
        if child.get("x").to_i.to_s==child.get("x") and child.get("y").to_i.to_s==child.get("y") 
          node.x=child.get("x").to_i
          node.y=child.get("y").to_i
        else
          node.x=@nodes.length
          node.y=0
        end
      #raise "FIXME: read position"
      
	      nodename=child.get("name")
	      
	      nodename="node#{@nodes.length}" if nodename==""
	      @nodes[nodename]=node
        unless hasEdges
          @edges<<Edge.new("node#{@nodes.length-2}",nodename,"") if @nodes.length>1
        end
      end
    }
    puts dump
  end
  
  
  def dump
    doc=Document.new
    root=doc.root
    root.setName("campaign")
    varNames.each{|n|root.set(n,@vars[n])}
    @nodes.each{|name,node|
      l=nil
      case node
        when Story
          l=root.addChild("cutscene")
          l.set("name",name)
          node.screens.each{|screen|
            screenNode=l.addChild("screen")
            if screen.text
              screenNode.addChild("text").set("text",screen.text)
            end
            if screen.imageFilename
              screenNode.addChild("image").set("filename",screen.text)
            end
          }
        when Level
          l=root.addChild("level")
          l.set("name",name)
          l.set("file",node.filename)
      end
      l.set("x",node.x.to_s)
      l.set("y",node.y.to_s)
    }
    @edges.each{|edge|
      l=root.addChild("edge")
      l.set("from",edge.from)
      l.set("to",edge.to)
      l.set("name",edge.name)
    }
    doc.toString
  end
  def varNames
    ["image","name","order","description"]
  end
end
