-- lua init file

function allprint(a)
	print "\n"
	for k,v in pairs(a)
		do
			print(k,v)
		end
	print "\n"
end

function lua_log(content)
	local info = debug.getinfo(2, "nSl")
	log(content, info["short_src"], info["currentline"], info["name"])
end

function lua_detail_log(content)
	local info = debug.getinfo(2, "nSl")
	detail_log(content, info["short_src"], info["currentline"], info["name"])
end

function lua_debug_log(content)
	local info = debug.getinfo(2, "nSl")
	debug_log(content, info["short_src"], info["currentline"], info["name"])
end

function lua_error_log(content)
	local info = debug.getinfo(2, "nSl")
	error_log(content, info["short_src"], info["currentline"], info["name"])
end

function test()
	lua_log("just for test")
end

function HandlerInit(thread)
	print("hello [[" .. thread .. "]]")
	lua_log("hello log system in lua")
	lua_detail_log("hello log in lua detail log")
	lua_debug_log("hello log in lua debug log")
	lua_error_log("hello log in lua error log")
	test()
	return true
end

function HandlerFinally(thread)
	print("hello luademo end" .. thread)
end

function HandlerOntimer(interval)
	print("hello timer interval" .. interval)
	if interval == 30 then
		error("report an error. it have arrived 30 seconds")
	end
end

function HandlerPlayersessionProcess(session, packet)
	packet:HexLike()
	local str = packet:GetStr()
	print("playersession recv a packet [[" .. str .. "]]")
	temp = WorldPacket(0)
	temp:SetStr("hello client from server string")
	session:SendPacket(temp)
end

function HandlerPlayersessionUnInit(session)
	print("playersession uninit")
end

function HandlerPlayersessionOnTimer(session, interval)
	print("playersession timer interval" .. interval)
end

function HandlerClientsessionProcess(session, packet)
end

function HandlerClientsessionUnInit(session)
end

function HandlerClientsessionOnTimer(session, interval)
end
