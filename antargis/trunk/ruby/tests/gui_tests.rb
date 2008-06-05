#!/usr/bin/env ruby

require 'ruby/antargislib.rb'

class AGApplication
  alias getNewEventOld getNewEvent
  def getNewEvent
    @eventDebugging||=File.open("events.txt","r")
    if @eventDebugging.eof
      return nil
    end
    s=@eventDebugging.readline
    return toSDLEvent(s)
  end 
end
