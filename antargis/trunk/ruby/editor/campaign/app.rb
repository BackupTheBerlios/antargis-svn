require File.join(File.split(__FILE__)[0],'drag_grid.rb')


# This is the main application-class of the campaign-editor.
# It manages the interaction of the different widgets within
# and calls the sub-editors (level and story-editor)
class CampaignEditorApp<AGApplication
  def initialize()
    super
    createSignal :sigFrame
    layout=AGLayout.new(nil)
    @layout=layout
    file=File.join('data','gui','layout','editor','campaign','main.xml')
    layout.loadXML( loadFile(file) )
    setMainWidget(layout)
    layout.setApp(self)
    if layout.getChild("bigTable")
      env=layout.getChild("dragEnvironment")
      @grid=layout.getChild("dragGrid")
      addHandler(@grid,:sigClick,:eventDeselect)
    end
    
    addHandler(layout.getChild("edit"),:sigClick,:eventEdit)
    addHandler(layout.getChild("quit"),:sigClick,:eventQuit)
    addHandler(layout.getChild("trash"),:sigClick,:eventTrashClicked)
    
    initSavingLoading
  end
  def eventQuit
    tryQuit
  end
  def eventDeselect
    @grid.select(nil) if @grid
    false
  end
  def eventFrame(t)
    sigFrame(t)
    super
  end
  def getEffect(name)
    @layout.getChild(name)
  end
  def eventEdit
    widget=nil
    case @grid.selected
      when DragBoxStory
        widget=StoryEditor.new(@layout,@layout.getRect,{})
      when DragBoxLevel
        # FIXME
    end
    
    @layout.addChild(widget) if widget
    true
  end
  
  def startEffect(name)
    case name
      when "showEdit"
        getEffect(name).run
        getEffect("hideEdit").stop
      when "hideEdit"
        getEffect(name).run
        getEffect("showEdit").stop
    end
  end
  
  def eventTrashClicked
    if @grid.selected
      @grid.removeChild(@grid.selected)
    end
  end
  
  def initSavingLoading
    addHandler(@layout.getChild("load"),:sigClick,:eventLoad)    
    addHandler(@layout.getChild("save"),:sigClick,:eventSave)    
  end
  
  def eventLoad
    CampaignEditor::LoadDialog.new(@layout) {|filename|
      campaign=Campaign::loadCampaign(filename)
      viewData(campaign)
    }
    
    true
  end
  def eventSave
    CampaignEditor::SaveDialog.new(@layout) {|filename|
      puts filename
      exit
    }
    true
  end
  
  
  private
  def viewData(campaign)
    @grid.clear
    
    boxes={}
    
    campaign.nodes.each{|name,node|
      x=node.x
      y=node.y
      c=getCell(x,y)
      box=nil
      case node
        when Level
          box=DragBoxLevel.new(@grid,c.rect.shrink(5),name,false)
          box.filename=node.filename
          @grid.addChild(box)
        when Story
          box=DragBoxStory.new(@grid,c.rect.shrink(5),name,false)
          box.story=node.screens
          @grid.addChild(box)
      end
      boxes[name]=box
    }
    
    campaign.edges.each{|edge|
      line=DragLine.new(@grid,@grid.getRect,boxes[edge.from])
      line.endObject=boxes[edge.to]
      @grid.addChild(line)
    }
    
  end
  
  def getCell(x,y)
    r=@grid.cells[0].rect
    x+=0.5
    y+=0.5
    x*=r.width
    y*=r.height
    @grid.cells.select{|cell|cell.rect.contains(AGVector2.new(x,y))}[0]
  end
end



