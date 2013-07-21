function onPlayerCommand(playerid, command)
	local cmd = split(command, " ");

	if cmd[0] == "/w" or cmd[0] == "/whisper" then
		if table.length(cmd) < 3 then
			sendPlayerMessage(playerid, "Syntax: (/w)hisper [playerid] [text]")
			return 1
		end

		if not isPlayerConnected(tonumber(cmd[1])) then
			sendPlayerMessage(playerid, "Player " + cmd[1].tointeger() + " is not connected")
			return 1
		end

		local text = string.sub(command, #cmd[0] + #cmd[1] + 2)
		sendPlayerMessage(tonumber(cmd[1]), "Whisper from " .. getPlayerName(playerid) .. " [" .. playerid .. "]: " .. text)
		sendPlayerMessage(playerid, "Whisper sent to " .. getPlayerName(tonumber(cmd[1])) .. " [" .. tonumber(cmd[1]) .. "]: " .. text)
		log("[Whisper] " .. getPlayerName(tonumber(cmd[1])) .. " to " .. getPlayerName(tonumber(cmd[1])) .. ": " .. text)
		callEvent("playerWhisper", 1, playerid, tonumber(cmd[1]), text)
		return 1
	end
	return 1;
end
addEvent("playerCommand", onPlayerCommand)