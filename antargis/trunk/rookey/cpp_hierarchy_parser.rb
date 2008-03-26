require 'ostruct'

module Rookey
  
  def Rookey.getRubyClasses(classes)
    cs=[classes.select{|c|c.name=="AGRubyObject"}[0]]
    begin
      s=cs.length
      cs=[cs+cs.map{|c|c.children}].flatten.uniq
    end while s!=cs.length
    cs
  end
  
  def Rookey.getOrderedCppHeaders(classes)
    map={}
    order={}
    
    allClasses=classes.dup
    classes=getRubyClasses(classes)
    
    classes.each{|c|
      map[c.name.to_s]=c
    }
    classes.each{|c|
      order[c.name]=0 if map[c.parent.to_s].nil? 
    }
    oneNotSet=false
    maxNumber=0
    begin
      oneNotSet=false
      classes.each{|c|
        if order[c.name].nil?
          if order[c.parent].nil?
            oneNotSet=true
          else
            order[c.name]=order[c.parent]+1
            maxNumber=[maxNumber,order[c.name]].max
          end
        end
      }
    end while oneNotSet
    #puts "MAX:",maxNumber
    cs=[]
    (0..maxNumber).each{|i|
      order.each{|k,v|
        if v==i
          cs << map[k]
        end
      }
    }
    pp order
    allFiles=allClasses.map{|c|c.header_file}.uniq
    result=classes.sort{|a,b|order[a.name]<=>order[b.name]}.map{|c|c.header_file}.uniq
    result=result+(allFiles-result)
    pp result
    result
    #cs.map{|c|c.header_file}.uniq
  end
  
  
  class CppHierarchyParser
    CppClass=Struct.new(:name,:parent,:header_file,:children)
	  
    
    def initialize(files)
      @cppClasses=[]
      parseFiles(files)
      processDerivations
    end
	
	  def getAllClasses
	    @cppClasses
	  end
    
    def getAllRubyClasses
      clist=["AGRubyObject"]
      while true
        oldLength=clist.length
        @cppClasses.each{|c|
          if clist.member?(c.parent)
            clist=(clist+[c]).uniq
          end
        }
        break unless oldLength!=clist.length
      end
      clist
    end
    
    # returns array of (subclass,superclass)
    def getAllDerivations
      @cppClasses.select{|c|c.parent}.map{|c|[c.name,c.parent]}
    end
    
	  
  private
    
    
    def processDerivations
      m={}
      @cppClasses.each{|c|m[c.name]=c}
      @cppClasses.each{|c|
        if c.parent
          if m[c.parent].nil? 
            puts "parent unknown #{c.parent}"
          else 
	          m[c.parent].children << c
          end
        end
      }
    end
    
	  def parseFiles(files)
		  class2File={}
		  deriveList={} # x=>y :x is child of y
		  allClasses=[]
		
		  files.each{|fn|
		    file=File.open(fn)
		    cn=""
		    content=file.read.gsub(/\/\*([^*]|\*[^\/])*\*\//,"") # /*...*/ delete comments - FIXME: // comments will be ignored!!!
		
		    content.split("\n").each{|a|
		      abak=a
		      a.gsub!("AGEXPORT","")
		      a.gsub!("EXPORT","")
		      
		      if a =~ /^ *class.*/ then
		        cn=a.gsub(/ *class /,"").gsub(/:.*/,"").gsub("\n","").gsub(" ","")
		        allClasses << cn.gsub(";","")
		        if cn=~/^[A-Z].*/
		          if a=~ /.*public.*/ then
		            pn=a.gsub(/.*public /,"").gsub("\n","")
	              @cppClasses << CppClass.new(cn,pn,fn,[])
		            allClasses << pn
		            deriveList[cn]=pn
		            class2File[cn]=fn
		          elsif not a=~/;/ then
		            @cppClasses << CppClass.new(cn,nil,fn,[])
		            deriveList[cn]=nil
		            class2File[cn]=fn
		          end
		        end
		      end
		    
		    }
		  }
		  classList = class2File.keys
		  allClasses = allClasses.sort.uniq
		end  
  end

end