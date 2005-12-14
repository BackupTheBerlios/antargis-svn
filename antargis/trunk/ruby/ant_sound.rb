#!/usr/bin/env ruby

$soundLastCall={}

def playSound(name,minDiff=0.5)
	sounds={"sheep"=>"data/sound/sheep44.wav","error"=>"data/sound/error.wav","hackaxe"=>"data/sound/tree_chop.wav","wind"=>"data/sound/wind.wav","pick"=>"data/sound/new_pick.wav","sword"=>"data/sound/sword1.wav"}
	#puts "PLA"
	puts $soundLastCall.keys
	if not $soundLastCall.keys.member?(name)
		$soundLastCall[name]=-minDiff-1
		#puts "RESET:"+$soundLastCall[name].to_s
	end
	s=sounds[name]
	if not s
		s=name
	end
	puts $soundLastCall[name].to_s+"   "+$systemTime.to_s
	if s && $systemTime-$soundLastCall[name]>minDiff # at a second ago
		getSoundManager.volumeSound(0.4)
		#puts "PLAY:"+s
		getSoundManager.playWave(s)
		$soundLastCall[name]=$systemTime
	end
end

def ambientSound(time)
	#playSound("wind")
end