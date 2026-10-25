liber = {}
local path = "/proto/lib/"
local loaded = {}
function liber.setSrc(p)
	path = p
end
function liber.load(name, version)
	version = version or "any"
	runFile(path .. name .. ".libH")
	local func, err = load(name .. ".init()")
	if not func then
		return false, err
	end
	local namespace, ver, dir = func()
	if dir == nil then
		dir = ""
	else
		dir = dir .. "/"
	end
	if version == "any" or version == ver then
		runFile(path .. dir .. name .. ".lua")
		loaded[namespace] = ver
		return true, namespace, ver
	else
		return false
	end
end

function liber.unload(name)
	local func, err = load(name .. ".exit()")
	if not func then
		return false, err
	end
	func()
	loaded[name] = nil
end
