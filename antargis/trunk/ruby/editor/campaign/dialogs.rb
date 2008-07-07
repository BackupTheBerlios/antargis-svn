
module CampaignEditor
  class Dialog<AGLayout
    #include AGHandler
    def initialize(p)
      super
    end
    def loadXML(f)
      super
      addHandler(getChild("cancel"),:sigClick,:eventCancel)
      addHandler(getChild("ok"),:sigClick,:eventOk)
    end
    def eventCancel
      close
    end
    def eventOk
      close
    end
  end
  
  class SaveDialog<Dialog
    def initialize(p,&block)
      super
      p.addChild(self)
      loadXML(loadFile("data/gui/layout/editor/campaign/save_dialog.xml"))
      getChild("ok").setEnabled(false)
      addHandler(getChild("filename"),:sigModified,:eventFilenameChanged)
      
      listbox=getChild("listBox")
      Campaign.getAllFilenames.each{|f|
        listbox.insertItem(f,AGStringUtf8.new(f))
      }
      
      addHandler(listbox,:sigSelect,:eventSelected)
      @block=block
    end
    def eventOk
      super
      @block.call(getFilename)
    end
    def eventFilenameChanged
      getChild("ok").setEnabled(getChild("filename").getText.to_s.length>0)
    end
    def eventSelected
      getChild("filename").setText(AGStringUtf8.new(getChild("listBox").getSelectedValue))
      eventFilenameChanged
    end
    private
    def getFilename
      getChild("filename").getText.to_s
    end
	end
	
	class LoadDialog<Dialog
    def initialize(p,&block)
      super
      p.addChild(self)
      loadXML(loadFile("data/gui/layout/editor/campaign/load_dialog.xml"))
      
      listbox=getChild("listBox")
      Campaign.getAllFilenames.each{|f|
        listbox.insertItem(f,AGStringUtf8.new(f))
      }
      
      @block=block
    end
    def eventOk
      super
      @block.call(getFilename)
    end
    private
    def getFilename
      getChild("listBox").getSelectedValue.to_s
    end
	  
	end
end