def logTo(outputChannel,*ar)
  outputChannel << "[" << caller[1].to_s << "] "
  ar.each{|a|
    Antargis::out<<a.to_s

    }
  outputChannel<<Antargis::endl
end

def log(*ar)
  logTo(Antargis::out,*ar)
end

def err(*ar)
  logTo(Antargis::err,*ar)
end

def trace(*ar)
  logTo(Antargis::trace,*ar)
end

def logpp(*ar)
  logTo(Antargis::out,ar.inspect)
end