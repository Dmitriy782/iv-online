addEvent("scriptInit", 
	function()
		-- Ensure the client script is loaded
		loadClientScript("main/c_runcode.lua")
	end
)

addEvent("scriptExit", 
	function()
		-- Ensure the client script is unloaded
		unloadClientScript("main/c_runcode.lua")
	end
)

function runCode(code)
	local script = load(code)

	if script then
		script()
		print("Command executed: " .. code) --TODO: result or error
	else
		print("Command failed: " .. code)
	end
end

function onConsoleInput(input)
	local splitstring = split(input, " ")

	if splitstring[0] == "runcode" then
		if table.length(splitstring) > 1 then
			runCode(string.sub(input, 8))
		end
	elseif splitstring[0] == "cruncode" then
		if table.length(splitstring) > 2 then
			local playerid = tonumber(splitstring[1])
			local code = string.sub(input, 10 + #splitstring[1])
			triggerClientEvent(playerid, "runCode", code)
		end
	end
end
addEvent("consoleInput", onConsoleInput)
