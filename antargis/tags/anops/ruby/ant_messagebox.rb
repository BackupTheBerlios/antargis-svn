#!/usr/bin/env ruby

class MessageBox<AntDialog
	include AGHandler

	attr_accessor :block
	def initialize(p,f)
		super
		addHandler(getChild("ok"),:sigClick,:eventClick)
		addHandler(getChild("cancel"),:sigClick,:eventClick)
	end
	def eventClose(e)
		puts "EVENT CLOSE"
		super
		if @block
			@block.call("close")
		end
		return true
	end
	def eventClick(e)
		#super
		if @block
			@block.call(e.getCaller.getName)
		end
		return false
	end
end

MB_OK=1
MB_CANCEL=2
MB_YES=4
MB_NO=8

def messageBox(title,text,flags,&block)
	mw=getApplication.getMainWidget
	w=MessageBox.new(mw,"data/gui/layout/messagebox.xml")
	w.block=block
	w.getChild("title").setText(title)
	w.getChild("text").setText(text)
	mw.addChild(w)
end
