print("S.S.S. started")
print([[


  ____  ____  ____  _____  ____    ____  ____  ____     _  _____ ____  _____
 /  __\/  __\/  _ \/__ __\/  _ \  /  __\/  __\/  _ \   / |/  __//   _\/__ __\
 |  \/||  \/|| / \|  / \  | / \|  |  \/||  \/|| / \|   | ||  \  |  /    / \
 |  __/|    /| \_/|  | |  | \_/|  |  __/|    /| \_/|/\_| ||  /_ |  \_   | |
 \_/   \_/\_\\____/  \_/  \____/  \_/   \_/\_\\____/\____/\____\\____/  \_/




]])
returned = 1

function wait_key_event(t, key)
	local time2 = Timer()
	local time = Timer()
	local pressedKey
	while not time:isReady(t) do
		lv_timer_handler()
		if time2:isReady(5) then
			lv_tick_inc(5)
		end
		pressedKey = get_key()
		if pressedKey == key then
			return true
		end
	end
	time:reset()
end

function wait(t)
	wait_key_event(t, "-1")
end

function open_boot_config()
	drawImage("/furre_kartinka.bmp", 0, 0, 0)
	wait(10000)
end

local function die()
	print("bad config")
	drawImage("/criticalError.png", 0, 0, 0)
	if wait_key_event(10000, "F5") then
		open_boot_config()
	end
	drawImage("/rip.png", 0, 0, 0)
end

drawImage("/boot.png", 0, 0, 0)
wait(2000)
print("next step of loading..")

local path_to_boot = readConfig("/run.conf", "sys")
if path_to_boot == nil then
	die()
else
	path_to_sys = readConfig(path_to_boot, "boot")
	if path_to_sys == nil then
		die()
	end
end

print("run sys..")
print("booting: " .. readConfig(path_to_boot, "name"))
print("version: " .. readConfig(path_to_boot, "version"))
print("first setup file: " .. readConfig(path_to_boot, "boot"))
print(runFile(readConfig(path_to_boot, "boot")))

if returned == 0 then
	print("good exit, block")
	while true do
	end
end

if returned == 1 then
	print("bad exit, crash")
	drawImage("die.png", 0, 0, 0)
end
if returned == 2 then
	print("good exit, reboot")
	reboot()
end
if returned == 3 then
	print("good exit, go to run config")
	open_boot_config()
end
if returned == 4 then
	print("good exit, end")
end
