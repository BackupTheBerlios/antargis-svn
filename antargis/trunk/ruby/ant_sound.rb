#!/usr/bin/env ruby

$soundLastCall={}
$soundInited=false

def getVolume(type)
	v=getConfig.get("#{type}Volume")
	if v==""
		if type=="music"
			v=0.2
		else
			v=1
		end
	end
	
	v.to_f
end

def playSoundGlobal(name,volume,minDiff=0.5)
	sounds={"sheep"=>"data/sound/sheep44.wav",
		"error"=>"data/sound/error.wav",
		"hackaxe"=>"data/sound/tree_chop.wav",
		"wind"=>"data/sound/wind.wav",
		"pick"=>"data/sound/new_pick.wav",
		"sword"=>"data/sound/sword1.wav",
		"fire"=>"data/sound/fire.wav",
		"ugh_end"=>"data/sound/ugh_end.wav",
		"won"=>"data/sound/yay.wav",
		"mylord"=>"data/sound/yes_mylord.wav",
		"ugh"=>["data/sound/person_ugh2.wav","data/sound/person_ugh4.wav","data/sound/ugh.wav"],
		"die"=>["data/sound/person_ugh1.wav","data/sound/person_ugh3.wav","data/sound/ugh2.wav"],
		"construct"=>["data/sound/workshop.wav"]
		}

	if not $soundLastCall.keys.member?(name)
		$soundLastCall[name]=-minDiff-1
		#puts "RESET:"+$soundLastCall[name].to_s
	end
	s=sounds[name]
	if not s
		s=name
	elsif s.class==Array
		s=s.shuffle[0]
	end
	#dputs $soundLastCall[name].to_s+"   "+$systemTime.to_s
	if s && $systemTime-$soundLastCall[name]>minDiff # at a second ago
		if not $soundInited
			setNormalVolumeWave
			$soundInited=true
		end
		getSoundManager.playWave(s,volume)
		$soundLastCall[name]=$systemTime
	end
end

$loopSounds={}
def playLoopSoundGlobal(id,name,pos,volume)
	s={"fire"=>"data/sound/fire.wav"}[name]
	return nil if s.nil?
	return nil if $app.nil? or $app.getScene.nil?

	d=(($app.getScene.getCamera.dim2-pos).length-INNER_VOL_SIZE)
	vol=1
	if d>0
		vol=[(OUTER_VOL_SIZE-d)/OUTER_VOL_SIZE,0].max
	end
	handle=getSoundManager.loopPlay(s,volume*vol)
	puts "HANDLE #{handle}"
	puts "--------------------------------------------------"
	$loopSounds[id]=[handle,name,pos,volume]
	id
end

def stopLoopSound(id)
	a=$loopSounds[id]
	return if a.nil?
	getSoundManager.stopChannel(a[0])
	$loopSounds.delete(id)
end
def updateSoundPos
	$loopSounds.each{|id,a|
		pos=a[2]
		d=(($app.getScene.getCamera.dim2-pos).length-INNER_VOL_SIZE)
		vol=1
		if d>0
			vol=[(OUTER_VOL_SIZE-d)/OUTER_VOL_SIZE,0].max
		end
		getSoundManager.volumeSound(a[0],a[3]*vol)
	}
end


def ambientSound(time)
	return nil if $app.nil?
	if not $ambientSound
		$ambientSound=getSoundManager.loopPlay("data/sound/wind_loop.wav",getVolume("ambient")*0.2)
		getSoundManager.stopMp3
		$ambientMusic=getSoundManager.playMp3("data/music/in-game1.ogg")
		#raise 1
		getSoundManager.volumeMusic(getVolume("music"))
		#getSoundManager.playMp3(
		#getSoundManager.playMp3("data/music/ant2.ogg")
	end
	if (not getSoundManager.isMusicPlaying)
		$ambientMusic=getSoundManager.playMp3("data/music/in-game1.ogg")
	end

end

def setNormalVolumeWave
	getSoundManager.volumeSound(0.4*getVolume("sound"))
	getSoundManager.volumeMusic(getVolume("music"))
	$normalVolume=true
end
def setQuietVolumeWave
	getSoundManager.volumeSound(0.1*getVolume("sound"))
	getSoundManager.volumeMusic(0.25*getVolume("music"))
	$normalVolume=false
end

def updateVolumes
	getSoundManager.volumeMusic(getVolume("music"))
	if $normalVolume
		setNormalVolumeWave
	else
		setQuietVolumeWave
	end
	if $ambientSound
		getSoundManager.stopChannel($ambientSound)
		$ambientSound=nil
	end
	ambientSound(1)
end