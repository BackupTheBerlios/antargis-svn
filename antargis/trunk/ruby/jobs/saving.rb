module XMLSaver
  def saveXML(node)
    XMLSaver.saveXML(self,node)
    if false
    instance_variables.each{|n|
      if n!="@machine"
        v=instance_variable_get(n)
        
        m=node.addChild("member")
        m.set("name",n.to_s)
        m.set("type",v.class.to_s)
        unless XMLSaver.saveXML(v,m)
          puts "unknown type #{n}:#{v}"
        end
        p "#{n}:#{v}"
      end
    }
    pp instance_variables
    end
    #raise "not implemented"
  end
  
  def XMLSaver.saveXML(what,node)
    node.set("type",what.class.to_s)
    case what
      when Numeric,String,FalseClass,TrueClass,Symbol
        node.set("value",what.to_s)
      when AntEntity
        node.set("value",what.uid.to_s)
      when Hash
        what.keys.each{|k|
          v=what[k]
          n=node.addChild("element")
          saveXML(k,n.addChild("key"))
          saveXML(v,n.addChild("value"))
        }
      when Array
        what.each{|v|
          n=node.addChild("element")
          #n.set("type",v.class.to_s)
          XMLSaver.saveXML(n,v)
        }
      when HLJob_BaseState, AntNewHLJob
        what.instance_variables.sort.each{|n|
          if n!="@machine"
            v=what.instance_variable_get(n)
            
            m=node.addChild("member")
            m.set("name",n.to_s)
            #m.set("type",v.class.to_s)
            unless XMLSaver.saveXML(v,m)
              puts "unknown type #{n}:#{v}"
            end
          end
        }        
      else
        pp "UNKNOWN:",what.class.to_s
        return false
    end
    true
  end
  
end

module XMLLoader
  def XMLLoader.loadXML(entity,node,current=nil)
    current||=entity
    job=nil
    case current
      when AntEntity
        node.getChildren("hljob").each{|n|
          t=n.get("type")
          case t
            when "AntHeroMoveJob"
              job=AntHeroMoveJob.new(entity,0,AGVector2.new(0,0),0)
            when "AntHeroRestJob"
              job=AntHeroRestJob.new(entity,10)
            when "AntHeroRecruitJob"
              target=n.getChildren.select{|c|c.get("name")=="@target"}[0]
              if target
                target=entity.getMap.getByUID(target.get("value"))
              
                job=AntHeroRecruitJob.new(entity,target)
              else
                raise "target missing in saved AntHeroRecruitJob"
              end
            else
              # FIXME!!!
              raise "unknown job: #{t}"
          end
          XMLLoader.loadXML(entity,n,job)
          #job.loadXML(n)
        }
      else
        node.getChildren.each{|c|
          if c.getName=="member"
            name=c.get("name")
            type=c.get("type")
            value=c.get("value")
            if value!=""
              value=case type
                when "Integer","Fixnum"
                  value.to_i
                when "Float"
                  value.to_f
                when "String"
                  value
                when "Symbol"
                  value.intern
                when "FalseClass"
                  false
                when "TrueClass"
                  true
                when *getDescendantsOfClass(AntRubyEntity).map{|c|c.to_s}
                  entity.getMap.getByUID(value.to_i)
                else
                  pp getDescendantsOfClass(AntRubyEntity).map{|c|c.to_s}
                  raise "Unknnown type #{type}"
              end
              current.instance_variable_set(name,value)
            else
              case type
                when Hash
                when Array 
              end
            end
          end
        }
    end
    job
  end
end