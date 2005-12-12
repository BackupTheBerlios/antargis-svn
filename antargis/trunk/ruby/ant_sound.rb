#!/usr/bin/env ruby

$soundLastCall={}

def playSound(name)
	sounds={"sheep"=>"data/sound/sheep44.wav","error"=>"data/sound/error.wav"}
	
	if not $soundLastCall[name]
		$soundLastCall[name]=0
	end
	s=sounds[name]
	puts $soundLastCall[name].to_s+"   "+$systemTime.to_s
	if s && $systemTime-$soundLastCall[name]>1 # at a second ago
		getSoundManager.volumeSound(0.4)
		getSoundManager.playWave(s)
		$soundLastCall[name]=$systemTime
	end
end
