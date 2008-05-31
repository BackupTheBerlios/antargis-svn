$debug||=false
if $debug
  module Kernel
    def invoker(levels = -1)
      st = Thread.current[:callstack]
      st && st[levels - 3]
    end
  end
  
  set_trace_func proc {|*args|
    Thread.current[:callstack] ||=[]
    case args[0]
    when /call$/
      Thread.current[:callstack].push args
    when /return$/
      Thread.current[:callstack].pop
    when /line$/
      Thread.current[:callstack].pop
      Thread.current[:callstack].push args
    end
  }
  
  def dputs(*a)
    i=invoker
    t="["+i[1].to_s+":"+i[2].to_s+" "+i[3].to_s+"]"
    puts t+" "+a.join(";")
  end
  
else
  def dputs(*a)
    puts a.join(";")
  end
end