--[[
 * Indicators script
 *
 * Server-side part that handles all events.
 *
 * Author: mabako
 */
--]]
addEvent("indicators:left",
	function(playerid)
		if isPlayerInAnyVehicle(playerid) and getPlayerSeatId(playerid) == 0 then
			local vehicleid = getPlayerVehicleId(playerid)
			local indicators = getVehicleIndicators(vehicleid)
			setVehicleIndicators(vehicleid, true, false, true, false)
		end
	end
)

addEvent("indicators:right",
	function(playerid)
		if isPlayerInAnyVehicle(playerid) and getPlayerSeatId(playerid) == 0 then
			local vehicleid = getPlayerVehicleId(playerid)
			local indicators = getVehicleIndicators(vehicleid)
			setVehicleIndicators(vehicleid, false, true, false, true)
		end
	end
);

addEvent("indicators:off",
	function(playerid)
		if isPlayerInAnyVehicle(playerid) and getPlayerSeatId(playerid) == 0 then
			local vehicleid = getPlayerVehicleId(playerid)
			setVehicleIndicators(vehicleid, false, false, false, false)
		end
	end
);

-- make sure the client-side script is loaded
addEvent("scriptInit",
	function()
		loadClientScript("main/c_indicators.lua")
	end
);

-- make sure all indicators are unset
addEvent("scriptExit",
	function()
		unloadClientScript("main/c_indicators.lua")

		for vehicleid = 0, MAX_VEHICLES-1, 1 do
			local indicators = getVehicleIndicators(vehicleid)
			if indicators then
				if indicators[0] or indicators[1] or indicators[2] or indicators[3] then
					setVehicleIndicators(vehicleid, false, false, false, false)
				end
			end
		end
	end
);
