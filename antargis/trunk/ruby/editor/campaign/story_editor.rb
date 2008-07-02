require File.join(File.split(__FILE__)[0],'image_list.rb')
require File.join(File.split(__FILE__)[0],'image_list_2d.rb')


class StoryEditor<AGWidget
  def initialize(p,r,options)
    super(p,r)
    @story=[]
    layout=AGLayout.new(self)
    layout.loadXML(loadFile("data/gui/layout/editor/campaign/story_editor.xml"))
    addChild(layout)
    
    @image=getChild("image")
    @imageList=getChild("imageList")
    @imageList.sigSelected.connect(self,:eventImageSelected)
    @next=getChild("next")
    @prev=getChild("prev")
    @edit=getChild("edit")
    @ok=getChild("ok")
    @cancel=getChild("cancel")
    
    @prev.setEnabled(false)
    @story=[makeNewScreen]
    
    addHandler(@next,:sigClick,:eventNext)
    addHandler(@prev,:sigClick,:eventPrev)
    addHandler(@edit,:sigModified,:eventText)
    
    addHandler(@cancel,:sigClick,:eventCancel)
    addHandler(@ok,:sigClick,:eventOk)
    @cursor=0
  end
  
  def eventCancel
    close
    true
  end
  def eventOk
    close
    true
  end
  
  def eventText(e)
    @story[@cursor].text=@edit.getText
    true
  end
  
  def eventNext(e)
    puts "NEXT"
    @cursor+=1
    @prev.setEnabled(true)
    @story << makeNewScreen while @cursor>=@story.length
    updateView
    true
  end
  def eventPrev(e)
    puts "PREV"
    @cursor-=1
    @prev.setEnabled(false) if @cursor==0 
    updateView
    true
  end
  
  def eventImageSelected(file,texture)
    @image.setTexture(texture)
    @story[@cursor].imageFilename=file
  end  
  private
  def makeNewScreen
    StoryScreen.new(firstImage,AGStringUtf8.new(""))
  end
  def firstImage
    @imageList.getFirstImage
  end
  
  def updateView
    screen=@story[@cursor]
    @image.setTexture(getImage(screen.imageFilename))
    @edit.setText(screen.text)
    getChild("info").setText(AGStringUtf8.new("Page #{@cursor+1} of #{@story.length}"))
  end
  def getImage(filename)
    @imageList.getImageByFilename(filename)
  end
end

[StoryEditor].each{|c|standardLayoutCreator(c)}
