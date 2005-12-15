#!/usr/bin/env ruby

$soundLastCall={}

def playSound(name,minDiff=0.5)
	sounds={"sheep"=>"data/sound/sheep44.wav",
		"error"=>"data/sound/error.wav",
		"hackaxe"=>"data/sound/tree_chop.wav",
		"wind"=>"data/sound/wind.wav",
		"pick"=>"data/sound/new_pick.wav",
		"sword"=>"data/sound/sword1.wav",
		"fire"=>"data/sound/fire.wav",
		"ugh"=>"data/sound/ugh.wav",
		"ugh_end"=>"data/sound/ugh_end.wav",
		"won"=>"data/sound/yay.wav",
		"mylord"=>"data/sound/yes_mylord.wav",
		}
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
	if not $ambientSound
		$ambientSound=getSoundManager.loopPlay("data/sound/wind_loop.wav",0.2)
		#getSoundManager.playMp3(
		#getSoundManager.playMp3("data/music/ant2.ogg")
	end
end