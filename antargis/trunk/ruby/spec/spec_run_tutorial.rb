#Dir.chdir(File.split(__FILE__)[0])

require 'ruby/spec_helper.rb'
require 'ruby/gui/testing.rb'
require 'ruby/ant_application.rb'

describe 'Tutorial campaign' do
	describe '- First level' do
		include GuiTest
		before(:all) do
			# FIXME
			file="data/levels/tutorial/tutorial0.antlvl"
			clientConnection=nil
			@app=makeTestAppClass(AntGameApp).new(loadFile(file),getVideo.width,getVideo.height,nil,clientConnection)
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
		describe '- State at the beginning' do
		  before(:each) do
			  @map=@app.getMap
			  @map.should be_a_kind_of(AntRubyMap)
			  clickAwayStory
			  @enemy=nil
			  @hero=nil
		  end
          it "should display no story-telling anymore at this time" do
          	widget("TellTale").should be_nil
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
		it "should be possible to recruit some men" do
			# do a recruit job
			@map=@app.getMap
			hero=getHero
			building=getBuilding
			hero.newHLRecruitJob(building)
			while hero.hasHLJob
				advance
			end
			hero.getJob.should_not be_a_kind_of(AntHeroRecruitJob)
			widget("TellTale").should be_visible
			building.getMen.length.should == 0
			hero.getMen.length.should > 2
			hero.getMen.length.should > getEnemy.getMen.length
			

		end
		it "should be possible to defeat enemy" do
	       clickAwayStory
			hero=getHero
			enemy=getEnemy
			hero.newHLFightJob(enemy)
			hero.getJob.should be_a_kind_of(AntHeroFightJob)
			enemy.hasHLJob.should be_false
			while hero.hasHLJob
				puts hero.getJob
				advance
			end
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
	    def advance
	    	@app.getMap.move(0.3)
			@app.step
	    end
		def map
		  @app.getMap
		end
	end
	
	def clickAwayStory
		telltaleWidget=widget("TellTale")
		return if telltaleWidget.nil?

		telltaleWidget.should_not be_nil
		telltaleWidget.should be_visible
		okButton=telltaleWidget.child("ok")
		okButton.should_not be_nil
		okButton.click
		trials=0
		while telltaleWidget.visible?
			okButton.click
			@app.step
			trials+=1
			trials.should <10
		end

	end
end