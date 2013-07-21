function onPlayerNameCheck(playerid, name)
	for i = 1, #name, 1 do
			-- Allow 'a' to 'z'
		if (name:sub(i,i) >= 'a' and name:sub(i,i) <= 'z') or

			-- Allow 'A' to 'Z'
			(name:sub(i,i) >= 'A' and name:sub(i,i) <= 'Z' ) or

			-- Allow '0' to '9'
			(name:sub(i,i) >= '0' and name:sub(i,i) <= '9' ) or

			-- Allow '[' and ']'
			(name:sub(i,i) == '[' or name:sub(i,i) == ']' ) or

			-- Allow '_'
			(name:sub(i,i) == '_' ) then
			-- allow
		else
			-- Disallow all other characters
			return 0
		end
	end

	return 1
end
addEvent("playerNameCheck", onPlayerNameCheck)