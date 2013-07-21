--[[**
 * Indicators script
 *
 * Press 'Q' or 'E' to toggle your vehicle's indicators.
 * Indicators automatically go off after turning in desired direction.
 *
 * Author: mabako
 *--]]

local waitingTurn = 0
local doneTurn = 0

addEvent("keyPress", 
	function(button, state)
		if state == "down" and getPlayerVehicleId(getLocalPlayer()) ~= INVALID_VEHICLE_ID then
			-- pressed Q (left indicator)?
			if(button == "q") then
				-- indicator already on?
				if waitingTurn > 0 then
					triggerServerEvent("indicators:off")
					waitingTurn = 0
				-- turn indicator on
				else
					triggerServerEvent("indicators:left")
					waitingTurn = 1
					doneTurn = 0
				end
			end
			-- pressed E (right indicator)?
			else if button == "e" then
				-- indicator already on?
				if waitingTurn > 0 then
					triggerServerEvent("indicators:off")
					waitingTurn = 0
				-- turn indicator on
				else
					triggerServerEvent("indicators:right")
					waitingTurn = 2
					doneTurn = 0
				end
			end
		end
	end
)

-- Testcode for dooropen
setPlayerDoorLockState(getLocalPlayer(),"cj_bank_door_L",-28.0, -463.0, 16.0, false, 1.0)

addEvent("frameRender", 
	function()
		-- Test draw light
		--drawLightWithRange(-374.137909, 1113.159546, 14.679585,125,125,0,20.0,30.0)
		
		if waitingTurn > 0 or doneTurn > 0 then
			local pad = getPlayerPadState(getLocalPlayer())
			local left = pad.inVehicleMove[1]
			local right = pad.inVehicleMove[2]

			if waitingTurn > 0 then
				-- waiting to turn left?
				if waitingTurn == 1 and left > 128 then
					doneTurn = 1
					waitingTurn = 0
				-- waiting to turn right?
				elseif waitingTurn == 2 and right > 128 then
					doneTurn = 2
					waitingTurn = 0
				end
			elseif doneTurn > 0 then
				-- waiting to stop turning left?
				if doneTurn == 1 or left == 128 then
					triggerServerEvent("indicators:off")
					doneTurn = 0
				-- waiting to stop turning right?
				elseif doneTurn == 2 and right == 128 then
					triggerServerEvent("indicators:off")
					doneTurn = 0
				end
			end
		end
	end
)