#Dir.chdir(File.split(__FILE__)[0])

require 'ruby/spec_helper.rb'
require 'ruby/spec/level_testing.rb'
require 'ruby/ant_application.rb'

# TODO: redesign spec for different runs

describe 'Tutorial campaign' do
	describe '- First level' do
	  include LevelTesting
    describe "- disjoint tests" do
			before do
				@app=getTestApp(:tutorial0)
		    @app.step
				
			end
			it "should start off with a story telling dialog" do
				telltaleWidget=widget("TellTale")
				telltaleWidget.should_not be_nil
				telltaleWidget.should be_visible
				textWidget=telltaleWidget.child("text")
				textWidget.should_not be_nil
				text=textWidget.getText.to_s
				okButton=telltaleWidget.child("ok")
				okButton.should_not be_nil
				okButton.click
				text.should_not ==(textWidget.getText.to_s)
				@app.step
				clickAwayStory
				
			end

      it "should be no problem to recruit only a few men at the beginning when aggression is low" do
        storyIsDisplayed.should be_true
        clickAwayStory
        rowen=hero("Rowen")
        keep=building("Keep")
        rowen.getMen.length.should == 1
        menCount=(rowen.getMen+keep.getMen).length
        rowen.getAggression.should == 1
        rowen.newHLRecruitJob(keep)
        while rowen.hasHLJob
          advance
        end
        rowen.getAggression.should == 1
        rowen.hasHLJob.should be_false
        rowen.getMen.length.should < menCount
        keep.getMen.length.should > 0
      end      
      	    
	    it "should be no problem to set aggression to maximum at the beginning" do
	      storyIsDisplayed.should be_true
	      clickAwayStory
	      rowen=hero("Rowen")
	      keep=building("Keep")
	      rowen.getMen.length.should == 1
	      menCount=(rowen.getMen+keep.getMen).length
	      rowen.getAggression.should == 1
	      rowen.setAggression(3.0)
	      rowen.newHLRecruitJob(keep)
	      while rowen.hasHLJob
	        advance
	      end
	      rowen.getAggression.should == 3
	      rowen.hasHLJob.should be_false
	      rowen.getMen.length.should == menCount
	      keep.getMen.length.should == 0
	    end
    end    
    
		describe '- State at the beginning' do
		  before(:all) do
	      @app=getTestApp(:tutorial0)
			  @map=@app.getMap
			  @map.should be_a_kind_of(AntRubyMap)
			  #clickAwayStory
			  @enemy=nil
			  @hero=nil
        advance
		  end
      it "should display a story-telling" do
        storyIsDisplayed.should be_true
        clickAwayStory
      end
      it "should display no story-telling anymore at this time" do
        storyIsDisplayed.should be_false
      end
      it "- hero should have low aggression at the beginning" do
        hero("Rowen").getAggression.should == 1
      end

			            	
		  it "should include Rowen and an enemy" do
		    heroes=@map.getOwnHeroes
				enemies=@map.getHeroes-@map.getOwnHeroes
				heroes.length.should == 1
				enemies.length.should == 1
				@hero=heroes[0]
				@enemy=enemies[0]
		  end
		  it "should have a single enemy with a few troopers" do
		  	getEnemy.getMen.length.should >2
		  end

		  it "should have Rowen with no followers yet" do
		  	getHero.getMen.length ==1
		  end

		  it "should have a tower with a few men" do
		  	towers=@map.getBuildings
				towers.length.should == 1
				@tower=towers[0]
				@tower.should be_a_kind_of(AntTower)
				@tower.getMen.length>2
		  end
		  it "should include more men in tower than enemy has" do
		  	tower=@map.getBuildings[0]
				tower.getMen.length.should > getEnemy.getMen.length
		  end
		  
		end
    
    describe "- normal run" do
      before(:all) do
        # nothing
        @app=getTestApp(:tutorial0)
        advance
        clickAwayStory
      end
    
			it "should be possible to recruit some men (not all)" do
				# do a recruit job
				@map=@app.getMap
				hero=getHero
	      hero.getAggression.should == 1
				building=getBuilding
				hero.newHLRecruitJob(building)
				while hero.hasHLJob
					advance
				end
				hero.getJob.should_not be_a_kind_of(AntHeroRecruitJob)
				widget("TellTale").should be_visible
				building.getMen.length.should > 0
				hero.getMen.length.should >= 2
			end
	    it "should display a story part" do
	      storyIsDisplayed.should be_true
	      clickAwayStory
	    end
	
	    it "should be possible to recruit the remaining men" do
	      rowen=hero("Rowen")
	      keep=building("Keep")
	      rowen.setAggression(3)
	      rowen.newHLRecruitJob(keep)
	      menCount=rowen.getMen.length+keep.getMen.length
	      while rowen.hasHLJob ; advance; end
	      rowen.getMen.length.should == menCount
	      keep.getMen.length.should == 0
	      rowen.getMen.length.should > getEnemy.getMen.length
	    end
	    
			it "should be possible to defeat enemy with low aggression" do
			  
				hero=getHero
				enemy=getEnemy
	      hero.setAggression(1)
				hero.newHLFightJob(enemy)
				hero.getJob.should be_a_kind_of(AntHeroFightJob)
				enemy.hasHLJob.should be_false
	      trials=300
				while hero.hasHLJob
					puts hero.getJob
					advance
	        break if storyIsDisplayed
	        trials.should > 0
	        trials-=1
				end
	      storyIsDisplayed.should be_true
				clickAwayStory
				advance
		    @app.result.won.should == "won"
	      map.getOwnHeroes.length.should == 2
			end
			
			
			it "should be possible to dismiss some people" do
				hero=getHero
				hero.setAggression(1)
				oldMenCount=hero.getMen.length
				oldMenCount.should > 1
				hero.newHLDismissJob
				while hero.hasHLJob
					advance
				end
				menCount=hero.getMen.length
				menCount.should < oldMenCount
			end
    end			
		
	
		def getEnemies
			enemies=map.getHeroes-map.getOwnHeroes
			enemies
		end
		def getEnemy
		  getEnemies[0]
		end
		def getHero
		  @app.getMap.getOwnHeroes[0]
		end
		def getBuilding
			@app.getMap.getBuildings[0]
		end
	end
end