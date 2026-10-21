--kernel.add_process(name, ico, func, func_to_print, windowed, sizeX, sizeY, once_print_func)
pix = {}
local structs = {}


local function newStruct(id, name, ico, func, func_to_print, windowed, sizeX, sizeY, once_print_func)
	local s = {
		id = id,
		name = name,
		ico = ico,
		func = func,
		func_to_print = func_to_print,
		windowed = windowed,
		sizeX = sizeX,
		sizeY = sizeY,
		once_print_func = once_print_func,
	}
	structs[id] = s
	return s
end


function pix.addWindow(name, ico, func, func_to_print, windowed, sizeX, sizeY, once_print_func)


end
