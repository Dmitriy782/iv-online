--audio.nut

--Definitions
-- Audio(url or resource, replay, url to file or add only name from the ressource)
local url = "http:--dl.dropbox.com/u/28171641/sounds/the_boondocks_-_riley%27s_theme.mp3" -- "http:--files.seti.ee/music/one/SETI_ee_ti_feat_rihanna_-_live_your_life.mp3"
local audiourl = Audio.constructor(true,false,url)
local audiourlposition = Audio.constructor(true,false,url)
audiourl:play()

function onKeyPress(key, status)
	if key == "u" then
		if  status == "down" then
			audiourl:play()
			addChatMessage("Music stream starts to play...")
		end
	elseif key == "i" then
		if status == "down" then
			audiourl:stop()
			addChatMessage("Stopped music stream")
		end
	elseif key == "o" then
		if  status == "down" then
			audiourlposition:usePositionSystem(true)
			audiourlposition:setPosition(1649.508179, 395.261627, 38.151573, 30.0)
			audiourlposition:play()
			addChatMessage("Music stream with position system starts to play...")
		end
	elseif key == "p" then
		if status == "down" then
			audiourlposition:stop()
			addChatMessage("Stopped music stream with position system")
		end
	elseif key == "n" then
		if status == "down" then
			tuneRadio(true,19)
		end
	elseif key == "m" then
		if status == "down" then
			tuneRadio(false,19)
		end
	elseif key == "b" then -- testcode
		if status == "down" then
			importAndLoadGameFile("main/hud.dat")
		end
	elseif key == "v" then -- testcode
		if status == "down" then
			importAndLoadGameFile("main/RadioLogo.dat")
		end
	end
end
addEvent("keyPress", onKeyPress)
