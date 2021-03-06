require "ruby/ant_sound.rb"
require 'ruby/tools/logging.rb'

class Meshes
  @@meshes={}
  def self.getMeshData(file,zoom,texture="",shadow=true)
    id=file+":"+texture
    if not @@meshes[id]
      raise "Texture-file not found!" unless (fileExists(texture) or texture=="")
      @@meshes[id]=MeshData.new(file,zoom,texture,shadow)
    end
    return @@meshes[id]
  end

  def self.getAnimMeshData(file)
    if not @@meshes[file]
      @@meshes[file]=AnimMeshData.new(file)
      @@meshes[file].setTransform(AGMatrix4.new(Math::PI,
          AGVector3.new(0,0,1))*AGMatrix4.new(Math::PI/2,AGVector3.new(1,0,0)))

    end
    return @@meshes[file]
  end

end


def getMeshData(*args)
  Meshes.getMeshData(*args)
end
def getAnimMeshData(*args)
  Meshes.getAnimMeshData(*args)
end


def displayError(error)
  getSoundManager.playWave(["data","sound","error.wav"].join("/"))
  playSound(error)
  dputs "ERROR:"+error.to_s
end

class AntSet<Array
  def push(x)
    super
    sort!
    uniq!
  end
end

def getDescendantsOfClass(p)
  c=[]
  ObjectSpace.each_object(Class){|a|c.push(a) if a.ancestors.member?(p)}
  return c
end

class Class
  def descendants
    getDescendantsOfClass(self)
  end
end

#def trace
#  puts "TRACE #{caller[0]}"
#end

class AntargisException<Exception
  attr_accessor :text
  
  def to_s
    super+":"+text.to_s
  end
end

def assert(text="",&block)
  if not block.call
    exception=AntargisException.new
    exception.text="#{text} #{block} failed in #{caller[0]} #{block.binding}"
    raise exception
  end
end


class Object
  def define_cmethod(method_name, method = nil, &proc)
    if proc.nil?
      raise ArgumentError
    else
      method = proc
    end if method.nil?
  
    (class << self; self; end).module_eval do
      define_method( method_name.to_sym, method )
    end
  end
end


class File
  def self.load(filename)
    f=File.open(filename,"r")
    c=f.read
    f.close
    c
  end
end
