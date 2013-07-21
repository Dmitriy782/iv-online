local White = 0xFFFFFFFF;

-- Function hooking example that adds all created vehicles to
-- an array then deletes them when the script unloads
local vehicles = {}
local createVehicle_Original = createVehicle
local blip1, blip2
local blip = createBlip(75, 0.0, 0.0, 0.0,true)
setBlipName(blip,"ILoveIVMP")
local cp1
local actor1
local blipcolortest

function createVehicle(model, x, y, z, rx, ry, rz, c1, c2, c3, c4)
	local respawn_delay = -1
	local vehicleId = createVehicle_Original(model, x, y, z, rx, ry, rz, c1, c2, c3, c4, respawn_delay)

	if vehicleId ~= INVALID_VEHICLE_ID then
		--log("Created a vehicle with id " .. vehicleId .. " (Model " .. model .. " (" .. getVehicleName(model) .. "))")
		vehicles[#vehicles] = vehicleId
	end
	return vehicleId
end

function onScriptError(name, callstack, locals)
	log("<scriptError (" .. name .. ")>")
	log("<Callstack>")
	for name, info in callstack do
		log("<" .. name .. " (" .. info[0] .. ", " .. info[1] .. ")>")
	end
	log("</Callstack>")
	log("<Locals>")
	for name, type in locals do
		log("<" .. name .. " (" .. type .. ")>")
	end
	log("</Locals>")
	log("</scriptError>")
	return 0; -- Don't print error to console
end
addEvent("scriptError", onScriptError)

function onScriptInit()
	createVehicle(50, 1682.030884, 415.455414, 28.492632, 359.998413, 358.788818, 47.536011, 1, 1, 1, 1, 30000) -- car1
	createVehicle(27, 1667.659546, 432.233398, 28.631674, 0.607056, 351.466797, 88.324951, 1, 1, 1, 1) -- car2
	createVehicle(37, 1666.811768, 421.175903, 28.643723, 359.297852, 351.982788, 268.549927, 0, 0, 0, 0) -- car3
	createVehicle(40, 1657.118408, 421.462982, 28.569500, 359.828613, 352.884033, 267.583008, 0, 0, 0, 0) -- car4
	createVehicle(93, 1649.273560, 421.382935, 28.761145, 0.001465, 351.280945, 270.491577, 1, 1, 1, 1) -- car5
	createVehicle(28, 1641.365967, 421.592712, 28.691023, 359.782715, 352.587891, 269.953735, 131, 131, 131, 131) -- car6
	createVehicle(95, 1631.719482, 420.940094, 28.562838, 1.065430, 352.694153, 277.920654, 0, 0, 0, 0) -- car7
	createVehicle(91, 1628.712646, 430.695221, 28.632729, 359.860657, 352.000366, 96.860474, 0, 0, 0, 0) -- car8
	--createVehicle(12, 1644.682373, 443.919067, 28.959641, 0.186646, 359.772217, 181.766296, 1, 1, 1, 1) -- car9
	createVehicle(90, 1655.603516, 439.904358, 28.416683, 359.700562, 352.003357, 0.547791, 0, 0, 0, 0) -- car10
	createVehicle(18, 1655.166138, 448.823395, 28.452307, 0.021606, 352.756836, 358.443848, 0, 0, 0, 0) -- car11
	createVehicle(106, 1650.725464, 411.433807, 30.725628, 0.017944, 0.080933, 28.687622, 0, 0, 0, 0) -- car12
	createVehicle(108, 1647.794922, 411.647644, 30.763536, 359.298218, 2.161499, 335.835205, 1, 1, 1, 1) -- car13
	cp1 = createCheckpoint(3, -343.447662, 1171.119263, 14.146016, -343.447662, 1176.119263, 14.146016, 3.0)
	--getVehicleDimension(vehicles[0])
	--actor1 = createActor(242, -341.36, 1142.80, 14.79, 5.0)
	log(_VERSION)
	local config = getConfig()
	log("Port = " .. config["port"])
	log(SCRIPT_NAME .. " sucessfully loaded")
	blipcolortest = createBlip(78,1000.0,1000.0,0.0,true)
	setBlipColor(blipcolortest,0xE59338FF,-1)
	setBlipName(blipcolortest, "idk")
	return 1
end
addEvent("scriptInit", onScriptInit)

function onScriptExit()	
	log("scriptExit();")
	for i = 0, #vehicles-1, 1 do
		deleteVehicle(vehicles[i])
	end
	log("done scriptExit();")
	return 1
end
addEvent("scriptExit", onScriptExit)

function onPlayerAuth(playerID, playerName, playerIP, playerSerial, bHasModdedGameFiles)
	-- Don't use natives here, the player sends only the request, he's NOT connected to the server!
	sendMessageToAll(playerName .. " (" .. playerID .. ") is connecting.", White)
	if bHasModdedGameFiles then
	--	return 0; -- Don't allow modified game files!
	end
	return 1
end
addEvent("playerAuth", onPlayerAuth)

function onPlayerConnect(playerid, playerName)
	-- Now you can use the natives, because the player is registered at the server
	setPlayerSpawnLocation(playerid, --[[-341.36, 1144.80, 14.79, 40.114815--]]1649.508179, 395.261627, 38.151573, -2.931725)
	sendPlayerMessage(playerid, "Welcome to Brooks", White)
	sendPlayerMessage(playerid, "Press [FFFF0000]'I'[FFFFFFAA] to stop the music!",0xFFFFFFAA,true)
	setPlayerTime(playerid, 6, 50)
end
addEvent("playerConnect", onPlayerConnect)

function onPlayerJoin(playerid)
	-- After he has downloaded all files, we're checking his gta files
	sendMessageToAll(getPlayerName(playerid) .. " (" .. playerid .. ") has successfully connected.", White)
	-- Check files
	log("FileChecksum from user "..getPlayerName(playerid).." handling.dat: 0x"..getPlayerFileChecksum(playerid,0))
	log("FileChecksum from user "..getPlayerName(playerid).." gta.dat: 0x"..getPlayerFileChecksum(playerid,1))
end
addEvent("playerJoin", onPlayerJoin)

function onPlayerDisconnect(playerid, reason)
	local strreason = "Quit"

	if reason == 1 then
		strreason = "Timeout/Crash"
	end
	sendMessageToAll(getPlayerName(playerid) .. " (" .. playerid .. ") has left the server (" .. strreason .. ").", White)
	deletePlayerBlip(playerid)
	return 1
end
addEvent("playerDisconnect", onPlayerDisconnect)

function onSpawn(playerid)
	--setPlayerModel(playerid, 7)
	createPlayerBlip(playerid, 3)
	togglePlayerBlipShortRange(playerid,true)
	
	-- Request here all animations, which you use
	-- ATTENTION: It can take 1 up to 5 seconds to load the animation(s) successfully!
	requestPlayerAnimations(playerid,"amb@smoking_spliff")
	requestPlayerAnimations(playerid,"amb@taxi_wait_i_f_b")
	requestPlayerAnimations(playerid,"amb@taxi_hail_f")
	
	-- NOTE: IF you USED the animations and don't need it anymore -> unrequestplayeranimations
	-- ATTENTION: You must load at all players the animations, if you force it!
	--unRequestPlayerAnimations(playerid,"amb@smoking_spliff")
end
addEvent("playerSpawn", onSpawn)

function onPlayerCommand(playerid, command)

	local cmd = split(command, " ")

	if cmd[0] == "/label" then
		if table.length(cmd) < 2 then 
			sendPlayerMessage(playerid, "USAGE: /label [labeltext]", 0xFFFFFFAA)
		else 
			local pos = getPlayerCoordinates(playerid)
			local labeltext = command.slice(#cmd[0] + 1)
			local keks = create3DLabel(labeltext, pos[0], pos[1], pos[2], 0xFF0000FF, true, 600.0)
			sendPlayerMessage(playerid, "Added successfull label "..keks.." with display string "..labeltext.."...",0xFFFFFFAA)
		end
	end
	if cmd[0] == "/weather" then
		setWeather(tonumber(cmd[1]))
	end
	if cmd[0] == "/moveobject" then
		moveObject(tonumber(cmd[1]), 1727.337769, 705.455688, 25.831680, 5000.0)
	end
	if cmd[0] == "/getwantedlevel" then
		sendPlayerMessage(playerid,"WANTEDLEVEL:"..getPlayerWantedLevel(playerid))
	end
	if cmd[0] == "/togglenametag" then
		if tonumber(cmd[3]) == 1  then
			togglePlayerNametagForPlayer(tonumber(cmd[1]),tonumber(cmd[2]), true)
		else
			togglePlayerNametagForPlayer(tonumber(cmd[1]),tonumber(cmd[2]), false)
		end
	end
	if cmd[0] == "/repairvehicle" then
		repairVehicle(getPlayerVehicleId(playerid))
	end
	if cmd[0] == "/world" then
		setPlayerDimension(playerid, 1)
	end
	if cmd[0] == "/world0" then
		setPlayerDimension(playerid, 0xFF-1)
	end
	if cmd[0] == "/clothes" then
		setPlayerClothes(playerid, tonumber(cmd[1]), tonumber(cmd[2]))
	end
	if cmd[0] == "/attachobjecttoped" then
		attachObjectToPlayer(playerid,tonumber(cmd[1]),0.5,0.3,0.5,0.0,90.0,0.0)
	end
	if cmd[0] == "/attachobjecttovehicle" then
		attachObjectToVehicle(tonumber(cmd[1]),tonumber(cmd[2]),0.0,0.0,1.0,0.0,0.0,0.0)
	end
	if cmd[0] == "/detachobject" then
		detachObject(tonumber(cmd[1]))
	end
	if cmd[0] == "/createobject" then
		local pos = getPlayerCoordinates(playerid)
		local object = createObject(tonumber(cmd[1]), pos[0]+2.0, pos[1]+2.0, pos[2], 0.0, 0.0, 0.0)
		sendPlayerMessage(playerid, "Object: "..object, 0xFFFFFFAA)
	end
	if cmd[0] == "/moveobject" then
		moveObject(tonumber(cmd[1]), 1727.337769, 705.455688, 25.831680, 5000.0)
	end
	if cmd[0] == "/moveobjectrot" then
		moveObject(tonumber(cmd[1]), 1727.337769, 705.455688, 25.831680, 5000.0, 0, 0, 0)
	end
	if cmd[0] == "/rotateobject" then
		rotateObject(tonumber(cmd[1]), 0, 0, 0, 5000.0)
	end
	if cmd[0] == "/toggleblip" then
		if tonumber(cmd[1]) == 1 then
			switchBlipIcon(blip,true)
		elseif tonumber(cmd[1]) == 0 then
			switchBlipIcon(blip,false)
		end
	end
	if cmd[0] == "/driveautomatic" then
		letPlayerDriveAutomaticAtCoords(playerid,getPlayerVehicleId(playerid),1044.286743, 255.983231, 31.037477,15.0,0)
	end
	if cmd[0] == "/driveactor" then
		driveActorToCoordinates(tonumber(cmd[1]),1044.286743, 255.983231, 31.037477)
	end
	if cmd[0] == "/sayspeech" then
		sayPlayerSpeech(playerid,"JIMMY_PEGORINO", "GET_OUT_OF_HERE")
	end
	if cmd[0] == "/actorspeech" then
		sayActorSpeech(tonumber(cmd[1]),"JIMMY_PEGORINO", "GET_OUT_OF_HERE")
	end
	if cmd[0] == "/gotovehicle" then
		local pos = getVehicleCoordinates(tonumber(cmd[1]))
		setPlayerCoordinates(playerid,pos[0],pos[1],pos[2])
	end
	if cmd[0] == "/alarm" then
		setVehicleAlarm(getPlayerVehicleId(playerid),tonumber(cmd[1]))
	end
	if cmd[0] == "/mobilephone" then
		if tonumber(cmd[1]) == 1  then
			setPlayerUseMobilePhone(playerid,true)
		elseif tonumber(cmd[1]) == 0 then
			setPlayerUseMobilePhone(playerid,false)
		end
	end
	if cmd[0] == "/setvehpos" then
		setVehicleCoordinates(tonumber(cmd[1]),cmd[2].tofloat(),cmd[3].tofloat(),cmd[4].tofloat())
	end
	if cmd[0] == "/atest" then
		setActorName(actor1, "adamixcock")
		toggleActorNametag(actor1, true)
		toggleActorHelmet(actor1, false)
		toggleActorFrozen(actor1, false)
		--warpActorIntoVehicle(actor1, 2, 2)
	end
	if cmd[0] == "/behind" then
		setCameraBehindPlayer(playerid)
	end
	if cmd[0] == "/lockvehicle" then
		setVehicleLocked(tonumber(cmd[1]),tonumber(cmd[2]))
	end
	if cmd[0] == "/setammo" then
		setPlayerAmmo(playerid,tonumber(cmd[1]),tonumber(cmd[2]))
	end
	if cmd[0] == "/12" then
		sendPlayerMessage(playerid, getPlayerIp(playerid))
	end	
	if cmd[0] == "/setammoinclip" then
		setPlayerAmmoInClip(playerid,tonumber(cmd[1]))
	end
	if cmd[0] == "/displayHudMessage" then
		displayHudNotification(playerid,1,"UNREAD_MESSAGES")
	end

	if cmd[0] == "/vehiclefollow" then
		-- Offset like as taxi-passenger in singleplayer
		-- NOTE: You have to set the coordinates for the vehicle! (width, lenght, height)(FROM THE VEHICLE)
		setPlayerFollowVehicleOffset(playerid,getPlayerVehicleId(playerid),0.325,-1.0,0.0)
		setPlayerFollowVehicleMode(playerid,0)
	end
	if cmd[0] == "/vehiclefollow2" then
		-- Offset like as taxi-passenger in singleplayer
		-- NOTE: You have to set the coordinates for the vehicle! (width, lenght, height)(FROM THE VEHICLE)
		setPlayerFollowVehicleOffset(playerid,getPlayerVehicleId(playerid),0.325,-1.75,0.0)
		setPlayerFollowVehicleMode(playerid,0)
	end
	if cmd[0] == "/variation" then
		setVehicleVariation(getPlayerVehicleId(playerid),tonumber(cmd[1]))
	end
	if cmd[0] == "/vehicleego" then
		setPlayerFollowVehicleMode(playerid,1)
		setPlayerFollowVehicleOffset(playerid,getPlayerVehicleId(playerid),0.5,-4.0,0.0)
	end
	if cmd[0] == "/attachcameratovehicle" then
		attachPlayerCameraToVehicle(playerid,tonumber(cmd[1]), tonumber(cmd[2]), 0.0, -5.0, 2.0)
	end
	if cmd[0] == "/attachcameratoplayer" then
		attachPlayerCameraToPlayer(playerid,tonumber(cmd[1]), tonumber(cmd[2]), 0.0, -2.5, 1.5)
	end
	if cmd[0] == "/respawn" then
		setPlayerSpawnLocation(playerid, 1649.508179, 395.261627, 38.151573, -2.931725)
		respawnPlayer(playerid)
	end
	if cmd[0] == "/name" then
		sendPlayerMessage(playerid,"VEHILCENAME:"..getVehicleName(getVehicleModel(getPlayerVehicleId(playerid))))
	end
	if cmd[0] == "/fadein" then
		fadePlayerScreenIn(playerid,5)
	end
	
	if cmd[0] == "/fadeout" then
		fadePlayerScreenOut(playerid, 5)
	end
	if cmd[0] == "/policereport" then
		triggerPlayerPoliceReport(playerid,"SCRIPTED_REPORTS_C4")
	end
	if cmd[0] == "/mission" then
		triggerPlayerMissionSound(playerid,6)
	end
	if cmd[0] == "/sound" then
		triggerPlayerGameSound(playerid,"MOBILE_PHONE_SMS_RECIEVE")
	end
	if cmd[0] == "/gps" then
		if isPlayerInAnyVehicle(playerid) then
			-- NOTE: The route(on radar) will have the same color as the blip!
			setBlipColor(blip,0xE59338FF,playerid)
			sendPlayerMessage(playerid,"ROUTE",White)
			setVehicleGpsState(getPlayerVehicleId(playerid),true)
			toggleBlipRoute(blip,true,playerid)
		end
	end
	
	if cmd[0] == "/gps2" then
		if isPlayerInAnyVehicle(playerid) then
			-- NOTE: The route(on radar) will have the same color as the blip!
			sendPlayerMessage(playerid,"ROUTE 2!",White)
			setVehicleGpsState(getPlayerVehicleId(playerid),true)
			toggleBlipRoute(blipcolortest,true,playerid)
		end
	end
	if cmd[0] == "/changeblip" then
		switchBlipIconForPlayer(blip,playerid,true)
	end
	if cmd[0] == "/changeblip2" then
		switchBlipIconForPlayer(blip,playerid,false)
	end
	if cmd[0] == "/forceanimplayer" then
		forcePlayerPlayAnimation(playerid,"amb@smoking_spliff","create_spliff")
	end
	if cmd[0] == "/forcetaxi1" then
		forcePlayerPlayAnimation(playerid,"amb@taxi_wait_i_f_b","look_around")
	end
	if cmd[0] == "/forcetaxi2" then
		forcePlayerPlayAnimation(playerid,"amb@taxi_hail_f","hail_right")
	end
	if cmd[0] == "/createfire" then
		local pos = getPlayerCoordinates(playerid)
		createFire(pos[0]+10.0,pos[1]+10.0,pos[2],10.0)
	end
	
	if cmd[0] == "/deletefire" then
		deleteFire(tonumber(cmd[1]))
	end
	if cmd[0] == "/createexplosion" then
		local pos = getPlayerCoordinates(playerid)
		createExplosion(pos[0]+10.0,pos[1]+10.0,pos[2],10.0)
	end
	if cmd[0] == "/remoteon" then
		setVehicleEngineState(tonumber(cmd[1]), true)
	end
	if cmd[0] == "/remoteoff" then
		setVehicleEngineState(tonumber(cmd[1]),false)
	end
	if cmd[0] == "/lights" then
		setVehicleLights(getPlayerVehicleId(playerid),true)
	end
	if cmd[0] == "/lightsoff" then
		setVehicleLights(getPlayerVehicleId(playerid),false)
	end
	
	if cmd[0] == "/ccd" then
		controlCarDoors(getPlayerVehicleId(playerid),tonumber(cmd[1]),true,cmd[2].tofloat())
	end
	if cmd[0] == "/taxi" then
		setVehicleTaxiLights(getPlayerVehicleId(playerid),true)
	end
	if cmd[0] == "/taxioff" then
		setVehicleTaxiLights(getPlayerVehicleId(playerid),false)
	end
	if cmd[0] == "/engine" then
		setVehicleEngineState(getPlayerVehicleId(playerid),true)
	end
	if cmd[0] == "/engineoff" then
		setVehicleEngineState(getPlayerVehicleId(playerid),false)
	end
	
	if cmd[0] == "/createactors" then
		for i = 0, tonumber(cmd[1])-1, 1 do
			local pos = getPlayerCoordinates(playerid)
			local actor = createActor(8,pos[0],pos[1],pos[2],90.0)
			toggleActorHelmet(actor,false)
			toggleActorFrozen(actor,false)
			setActorName(actor,"JeNkStAX")
			toggleActorNametag(actor,true)
			forceAnimationAtActor(actor,"amb@smoking_spliff","create_spliff")
			
			--
			local actorcount = getActorCount()
			sendPlayerMessage(playerid,"Actorcount: "..actorcount,0xFFFFFFAA)
		end
	end
	if cmd[0] == "/createactor" then
		local pos = getPlayerCoordinates(playerid)
		local actor = createActor(8,pos[0],pos[1],pos[2],90.0)
		toggleActorHelmet(actor,false)
		toggleActorFrozen(actor,false)
		setActorName(actor,"JeNkStAX")
		toggleActorNametag(actor,true)
		forceAnimationAtActor(actor,"amb@smoking_spliff","create_spliff")
		
		--
		local actorcount = getActorCount()
		sendPlayerMessage(playerid,"Actorcount: "..actorcount,0xFFFFFFAA)
	end
	if cmd[0] == "/createactor2" then
		local pos = getPlayerCoordinates(playerid)
		local actor = createActor(21,pos[0],pos[1],pos[2],90.0)
		toggleActorHelmet(actor,false)
		toggleActorFrozen(actor,false)
		setActorName(actor,"JeNkStAX")
		toggleActorNametag(actor,true)
		forceAnimationAtActor(actor,"amb@smoking_spliff","create_spliff")
		
		--
		local actorcount = getActorCount()
		sendPlayerMessage(playerid,"Actorcount: "..actorcount,0xFFFFFFAA)
	end
	if cmd[0] == "/toghelmet1" then
		toggleActorHelmet(0,true)
	end
	if cmd[0] == "/toghelmet2" then
		toggleActorHelmet(0,false)
	end
	if cmd[0] == "/togfrozen1" then
		toggleActorFrozen(0,true)
	end
	if cmd[0] == "/togfrozen2" then
		toggleActorFrozen(0,false)
	end
	if cmd[0] == "/walk" then
		actorWalkToCoordinatesForPlayer(playerid,0,0.0,0.0,0.0,1)
	end
	if cmd[0] == "/setname" then
		setActorName(0,"Fred")
	end
	if cmd[0] == "/togname1" then
		toggleActorNametag(0,true)
	end
	if cmd[0] == "/togname2" then
		toggleActorNametag(0,false)
	end
	if cmd[0] == "/forceanim" then
		forceActorAnimation(0,"hello", "gestures@niko")
	end
	if cmd[0] == "/grav" then
		setPlayerGravity(playerid, 0)
		return 1
	end
	
	if cmd[0] == "/givehelm" then
		givePlayerHelmet(playerid)
		return 1
	end
	
	if cmd[0] == "/removehelm" then
		removePlayerHelmet(playerid)
		return 1
	end
	
	if cmd[0] == "/grav1" then
		setPlayerGravity(playerid, 1)
		return 1
	end	
	
	if cmd[0] == "/kill" then
		setPlayerHealth(playerid, -1)
		return 1
	end
	
	if cmd[0] == "/model" then
		if table.length(cmd) == 2 then
			if setPlayerModel(playerid, tonumber(cmd[1])) then
				sendPlayerMessage(playerid, "Model set to " .. cmd[1], White)
			end
		end
		return 1
	end
	
	if cmd[0] == "/testtime" then
		setPlayerTime(playerid, 23, 0)
		return 1
	end
	
	if cmd[0] == "/testweapon" then
		givePlayerWeapon(playerid, 9, 20)
		return 1
	end

	if cmd[0] == "/time" then
		if table.length(cmd) == 3 then
			setPlayerTime(playerid, tonumber(cmd[1]), tonumber(cmd[2]))
		end
		return 1
	end

	if cmd[0] == "/wep" then
		if table.length(cmd) == 3 then
			givePlayerWeapon(playerid, tonumber(cmd[1]), tonumber(cmd[2]))
		end
		return 1
	end

	if cmd[0] == "/h" or cmd[0] == "/health" then
		if table.length(cmd) == 2 then
			if tonumber(cmd[1]) >= 0 and tonumber(cmd[1]) <= 200 then
				setPlayerHealth(playerid, tonumber(cmd[1]))
			end
		end
		return 1
	end

	if cmd[0] == "/a" or cmd[0] == "/armour" then
		if table.length(cmd) == 2 then
			if tonumber(cmd[1]) > 0 and tonumber(cmd[1]) <= 200 then
				sendPlayerMessage(playerid, "Armour change to " .. cmd[1])
				setPlayerArmour(playerid, tonumber(cmd[1]))
			end
		end
		return 1
	end

	if cmd[0] == "/autoaim" then
		if table.length(cmd) == 2 then
			if cmd[1] == "true" then
				togglePlayerAutoAim(playerid, true)
			else 
				togglePlayerAutoAim(playerid, false)
			end
		end
		return 1
	end

	if cmd[0] == "/v" then
		if table.length(cmd) == 2 then
			local model = tonumber(cmd[1])
			local pos
			if isPlayerInAnyVehicle(playerid) then
				pos = getVehicleCoordinates(getPlayerVehicleId(playerid))
			else
				pos = getPlayerCoordinates(playerid)
			end
			local heading = getPlayerHeading(playerid)
			local veh = createVehicle(model, pos[0], pos[1], pos[2], 0.0, 0.0, heading, 1, 1, 1, 1)
			if veh ~= INVALID_VEHICLE_ID then
				warpPlayerIntoVehicle(playerid, veh)
				sendPlayerMessage(playerid, getVehicleName(model) .. " spawned at your position (ID " .. veh .. ").")
			end
		elseif table.length(cmd) == 6 then
			local model = tonumber(cmd[1])
			local pos
			if isPlayerInAnyVehicle(playerid) then
				pos = getVehicleCoordinates(getPlayerVehicleId(playerid))
			else
				pos = getPlayerCoordinates(playerid)
			end
			local heading = getPlayerHeading(playerid)
			local veh = createVehicle(model, pos[0], pos[1], pos[2], 0.0, 0.0, heading, tonumber(cmd[2]),tonumber(cmd[3]),tonumber(cmd[4]),tonumber(cmd[5]))
			if veh ~= INVALID_VEHICLE_ID then
				warpPlayerIntoVehicle(playerid, veh)
				sendPlayerMessage(playerid, getVehicleName(model) .. " spawned at your position (ID " .. veh .. ").")
			end
		end
		return 1
	end

	if cmd[0] == "/dv" then
		if isPlayerInAnyVehicle(playerid) then
			local vehicleId = getPlayerVehicleId(playerid)
			deleteVehicle(vehicleId)
			sendPlayerMessage(playerid, "Your vehicle has been deleted.")
		else
			sendPlayerMessage(playerid, "You are not in a vehicle.")
		end
		return 1
	end
	
	if cmd[0] == "/out" then
		sendPlayerMessage(playerid, "You are being warped out of your vehicle.")
		removePlayerFromVehicle(playerid)
		return 1
	end

	if cmd[0] == "/out2" then
		sendPlayerMessage(playerid, "You are leaving your vehicle.")
		removePlayerFromVehicle(playerid, true)
		return 1
	end

	if cmd[0] == "/testmessage" then
		sendPlayerMessage(playerid, "Hello [FF0000FF]World", White, true)
		return 1
	end

	if cmd[0] == "/testobject" then
		-- http://www.gtamodding.com/index.php?title=Category:Static_Model_Hashes
		createObject(0xB4FC2C1B, -341.36, 1144.80, 14.79, 0.0, 0.0, 0.0)
		return 1
	end

	if cmd[0] == "/money" then
		if table.length(cmd) == 2 then
			setPlayerMoney(playerid, tonumber(cmd[1]))
			sendPlayerMessage(playerid, "Money set", White)
		end
		return 1
	end
	
	if cmd[0] == "/text" then
		if table.length(cmd) == 3 then
			displayPlayerText(playerid, 0.1, 0.1, tostring(cmd[1]), tonumber(cmd[2]))
		end
		return 1
	end
	
	if cmd[0] == "/infotext" then
		if table.length(cmd) == 3 then
			displayPlayerInfoText(playerid, tostring(cmd[1]), tonumber(cmd[2]))
		end
		return 1
	end

	if cmd[0] == "/texttoall" then
		if table.length(cmd) == 3 then
			displayTextToAll(0.1, 0.1, tostring(cmd[1]), tonumber(cmd[2]))
		end
		return 1
	end

	if cmd[0] == "/physics" then
		if table.length(cmd) == 2 then
			togglePlayerPhysics(playerid, tonumber(cmd[1]))
		end
		return 1
	end

	if cmd[0] == "/controls" then
		if table.length(cmd) == 2 then
			togglePlayerControls(playerid, tonumber(cmd[1]) and true or false)
		end
		return 1
	end

	if cmd[0] == "/wanted" then
		if table.length(cmd) == 2 then
			setPlayerWantedLevel(playerid, tonumber(cmd[1]))
		end
		return 1
	end

	if cmd[0] == "/goto" then
		if table.length(cmd) == 2 then
			local toPlayerId = tonumber(cmd[1])

			if not isPlayerConnected(toPlayerId) then
				sendPlayerMessage(playerid, "That player is not connected.")
				return 1
			end

			local pos
			if isPlayerInAnyVehicle(toPlayerId) then
				pos = getVehicleCoordinates(getPlayerVehicleId(toPlayerId))
			else
				pos = getPlayerCoordinates(toPlayerId)
			end
			if isPlayerInAnyVehicle(playerid) then
				setVehicleCoordinates(getPlayerVehicleId(playerid), pos[0], pos[1], pos[2])
			else
				setPlayerCoordinates(playerid, pos[0], pos[1], pos[2])
			end
			sendPlayerMessage(playerid, "You have been teleported.")
		end

		return 1
	end

	if cmd[0] == "/cptest" then
		hideCheckpointForAll(cp1)
	end
	
	if cmd[0] == "/testcheckpoint" then
		if table.length(cmd) == 2 then
			for i = 0, 31, 1 do
				hideCheckpointForAll(i)
				deleteCheckpoint(i)
			end
			local pos
			if isPlayerInAnyVehicle(playerid) then
				pos = getVehicleCoordinates(getPlayerVehicleId(playerid))
			else
				pos = getPlayerCoordinates(playerid)
			end
			local checkpointid = createCheckpoint(tonumber(cmd[1]), pos[0], pos[1], pos[2], pos[0], pos[1], (pos[2] + 10), 5.0)
			showCheckpointForAll(checkpointid)
			sendPlayerMessage(playerid, "Checkpoint created with id " .. checkpointid)
		end
	end
		
	if cmd[0] == "/vh" then
		if table.length(cmd) == 2 then
			if isPlayerInAnyVehicle(playerid) then
				local vehicleid = getPlayerVehicleId(playerid)
				local oldhealth = getVehicleEngineHealth(vehicleid)
				local health = tonumber(cmd[1])
				setVehicleEngineHealth(vehicleid, health)
				sendPlayerMessage(playerid, "Vehicle health set from " .. oldhealth .. " to " .. health)
			end
		end
	end

	if cmd[0] == "/getrot" then
		if isPlayerInAnyVehicle(playerid) then
			local rot = getVehicleRotation(getPlayerVehicleId(playerid))
			sendPlayerMessage(playerid, "Rot(X: " .. rot[0] .. ", Y: " .. rot[1] .. ", Z: " .. rot[2] .. ")")
		end
	end

	if cmd[0] == "/setrot" then
		if table.length(cmd) == 4 then
			if isPlayerInAnyVehicle(playerid) then
				setVehicleRotation(getPlayerVehicleId(playerid), cmd[1].tofloat(), cmd[2].tofloat(), cmd[3].tofloat())
				sendPlayerMessage(playerid, "Rotation set")
			end
		end
	end

	if cmd[0] == "/ind" then
		if table.length(cmd) == 3 then
			if isPlayerInAnyVehicle(playerid) then
				local index = tonumber(cmd[1])

				if index < 0 or index > 3 then
					sendPlayerMessage(playerid, "Invalid indicator index.")
				else
					local vehicle = getPlayerVehicleId(playerid)
					local indicators = getVehicleIndicators(vehicle)
					local toggle = ((tonumber(cmd[2]) == 1) and true or false)
					indicators[index] = toggle
					setVehicleIndicators(vehicle, indicators[0], indicators[1], indicators[2], indicators[3])
					sendPlayerMessage(playerid, "Indicator index " .. index .. " set to " .. toggle .. ".")
				end
			end
		end
	end
end
addEvent("playerCommand", onPlayerCommand)

function onPlayerDeath(playerid, killerid, killerweapon, killervehicle)
	if killerid ~= INVALID_PLAYER_ID then
		sendMessageToAll("[FF0000FF]" .. getPlayerName(playerid) .. " [FFFFFFFF](" .. playerid .. ") was killed by [FFFF0000]" .. getPlayerName(killerid) .. " [FFFFFFFF](" .. killerid .. ")(Weapon: "..killerweapon..").", White, true)
	else
		sendMessageToAll(getPlayerName(playerid) .. " died")
	end
	return 1
end
addEvent("playerDeath", onPlayerDeath)


function onWebRequest(uri, remote_ip, remote_method)
	log(string.format("Web request: %s [%s] [%s]", uri, remote_ip, remote_method))
end
addEvent("webRequest", onWebRequest)

function onVehicleDamage(vehicleid,oldhealth,oldpetrol,newhealth,newpetrol)
	--log(format("VEHICLE DAMAGE: %d, %d, %d, %d, %d",vehicleid,oldhealth,oldpetrol,newhealth,newpetrol))
end
addEvent("vehicleDamage",onVehicleDamage)

function onPlayerShot(playerid,x,y,z,xl,yl,zl,shot)
	--if(shot)
	--	log(format("WEP: %d SHOT(%f, %f, %f)|LOOKAT(%f, %f, %f)",playerid,x,y,z,xl,yl,zl))
	--else
	--	log(format("WEP: %d AIM(%f, %f, %f)|LOOKAT(%f, %f, %f)",playerid,x,y,z,xl,yl,zl));
end
addEvent("playerShot",onPlayerShot)

function onVehicleDeath(vehicleid)
	log("Vehicle Death "..vehicleid)
end
addEvent("vehicleDeath",onVehicleDeath)

function onVehicleRespawn(vehicleid)
	log("Vehicle Respawn "..vehicleid)
	return 1
end
addEvent("vehicleRespawn",onVehicleRespawn)

function onHeadMovement(playerid, oldx, oldy, oldz, newx, newy, newz)
	--log(format("HEAD: %d OLD(%f, %f, %f) | NEW (%f, %f, %f)",playerid,oldx,oldy,oldz,newx,newy,newz))
end
addEvent("headMove",onHeadMovement)

function onVehicleRequest(playerid,vehicleid,seat)
	return 1
end
addEvent("vehicleEntryRequest", onVehicleRequest)

function onSwitchEngine(playerid)
	if not isPlayerOnFoot(playerid) and getPlayerSeatId(playerid) == 0 then
		local enginestate = getVehicleEngineState(getPlayerVehicleId(playerid))
		setVehicleEngineState(getPlayerVehicleId(playerid), not enginestate)
		sendPlayerMessage(playerid,"Switched Engine",0xFFFFFFAA)
		triggerClientEvent(playerid, "engineResponse")
	end
end		
addEvent("switchEngine", onSwitchEngine)

function onVehicleEntryComplete( playerid, vehicleid, seatid )
	log("| Player:"..playerid.."| vehhicleid:"..vehicleid.."| seatid:"..seatid.." |")
	
	if seatid == 0 then
		sendPlayerMessage(playerid,"Press Y to turn the engine on/off",0xFFFFFFAA)
	end
end
addEvent("vehicleEntryComplete", onVehicleEntryComplete)
