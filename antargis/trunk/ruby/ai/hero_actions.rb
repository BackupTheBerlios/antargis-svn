module AI
  module HeroAction
    class Base
      attr_reader :interface
      def initialize(interface)
        @interface=interface
      end
    end

    class GetFood<Base
      def start(hero,target)
        puts target,target.class
        #raise 1
        hero.takeFood(target)
      end
    end

    class AttackHero<Base
      def start(hero,target)
        assert{hero!=target}
        hero.attack(target)
      end
    end

    class Rest<Base
      def start(hero)
        hero.rest(20)
      end
    end

    class Recruit<Base
      def start(hero,target)
        assert{hero!=target}
        hero.recruit(target)
      end
    end

    class ConstructEquipment<Base
      def start(hero,target)
        assert{hero!=target}
        hero.recruit(target)
      end
    end

    class GetEquipment<Base  
      def start(hero,target)
        assert{hero!=target}
        hero.takeWeapon(target)
      end
    end

  end
end