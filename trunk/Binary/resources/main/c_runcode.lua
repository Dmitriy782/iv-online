function runCode(codeString)
	local script = load(codeString)

	if script then
		script()
		print("Command executed: " .. codeString) --TODO: result or error
	else
		print("Command failed: " .. codeString)
	end
end

addEvent("runCode", runCode);


