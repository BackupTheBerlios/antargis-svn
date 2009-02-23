require 'pp'

class Array
  
  # *grep* goes through all files given by the filenames that should be contained in *self*
  # and checks for a given RegEx
  # example:
  #
  #  pp Dir["*.h"].grep(/INCLUDE_SWIG/)
  #
  # outputs all filenames of header-files containing the string INCLUDE_SWIG
  def grep(s)
    s=/#{s}/ if s.is_a?(String)
    self.select{|f|
      fd=File.open(f)
      result=(s===fd.read)
      fd.close
      result
    }
  end
end

module Rookey
  # creates a directory *dir* and parent-dirs if they don't exist
  def Rookey.mkdir(dir)
	  return if File.exists?(dir)
	  begin
	    Dir.mkdir(dir.to_s)
	  rescue
	    p=File.split(dir)[0]
	    return if p==dir
	    Rookey.mkdir(p)
	    begin
	      Dir.mkdir(dir)
	    rescue
	    end
	  end
	end
end

unless methods.member?("requireRookeyOld")
  alias :requireRookeyOld :require
  def require(file)
    if file[0..0]=="."
      rel=caller[0].gsub(/:[0-9]+/,"")
      rel=File.expand_path(rel)
      dir=File.split(rel)[0]
      file=File.join(dir,file)
      #pp file
    end
    requireRookeyOld(file)
  end
end
def requireRelative(f)
  require f
end

class FalseClass
  def <=>(o)
    if o==true
      -1
    else
      0
    end
  end
end
class TrueClass
  def <=>(o)
    if o==false
      1
    else
      0
    end
  end
end

