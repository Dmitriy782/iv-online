function split(str, sep)
	local t = {}
	i = 0
	for v in string.gmatch(str, "[^"..sep.."]+") do
		t[i] = v
		i = i+1
	end
	return t
end

table.length = function (T)
	local count = 0
	for _ in pairs(T) do count = count + 1 end
	return count
end

