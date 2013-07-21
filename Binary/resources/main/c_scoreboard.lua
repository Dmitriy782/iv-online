--scoreboard.nut
--Made by Sebihunter
--www.iv-multiplayer.com


--Definitions
local maxPlayers = 48 --This needs to be changed if your server allows more than 48 players!
local serverName = "IV: Multiplayer" --Change it to your server name
local onlinePlayers = 0
local screen = guiGetScreenSize()
local showscoreboard = false
local boldfont = GUIFont.constructor("tahoma-bold")
local font = GUIFont.constructor("tahoma")
local headerfont = GUIFont.constructor("bankgothic", 12)

--Gets the playercount on the server
function getPlayersOnServer()
	local players = 0
	for ply = 0, maxPlayers-1, 1 do
		if isPlayerConnected(ply) then
			players = players+1
		end
	end
	return players
end

--Draw the scoreboard
function onFrameRender()
	if showscoreboard == true then
		local y = screen[1]/2-(15*onlinePlayers)/2
		guiDrawRectangle(screen[0]/2-200, y, 400.0, 15.0, 0x50505080, false)
		y = y+15
		guiDrawRectangle(screen[0]/2-200, y, 400.0, 15.0+onlinePlayers*15, 0x00000080, false)
		boldfont:drawText(screen[0]/2-200+1, y,"ID", false)
		boldfont:drawText(screen[0]/2-200+50, y,"Name", false)
		boldfont:drawText(screen[0]/2-200+250, y,"Ping", false)	
		y = y+15
		local players = 0
		for ply = 0, maxPlayers-1, 1 do
			if isPlayerConnected(ply) then
				font:drawText(screen[0]/2-200+1, y, tostring(ply), false)
				font:drawText(screen[0]/2-200+50, y, getPlayerName(ply), false)
				local ping = getPlayerPing(ply)
				if ping < 100 then
					font:drawText(screen[0]/2-200+250, y, "[00FF33FF]"..ping, false)
				elseif ping < 200 then
					font:drawText(screen[0]/2-200+250, y, "[FF7D40FF]"..ping, false)
				else
					font:drawText(screen[0]/2-200+250, y, "[CD0000FF]"..ping, false)
				end	
				y = y+13
				players = players+1
			end
		end
		headerfont:drawText(screen[0]/2-200+1,screen[1]/2-(15*onlinePlayers)/2, serverName.." | Players: "..tostring(players), false)
		onlinePlayers = players
	end	
end
addEvent("frameRender", onFrameRender)

function onKeyPress(key, status)
	if key == "tab" then
		if status == "down" then
			toggleChatWindow(false)
			onlinePlayers = getPlayersOnServer()
			showscoreboard = true
		else
			toggleChatWindow(true)
			showscoreboard = false
		end
		addChatMessage("script name: " .. getScriptName())
	end
	
	-- engine start code...
	if key == "y" and status == "down" then
		triggerServerEvent("switchEngine")
	end
end
addEvent("keyPress", onKeyPress)

function onEngineResponse()
	addChatMessage("Client got Engine packet!",0x0000FFFF)
end
addEvent("engineResponse", onEngineResponse)