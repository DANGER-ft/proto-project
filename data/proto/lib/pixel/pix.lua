--вспомогательные функции
local config = {}
local baseline = {}
local objects = {}
local globalCounter = 1
local globalCounterList = 1
--список евентов
local events = {}
--обязательные параметры
baseline["window"] = { "width", "height", "name", "ico", "loop_func", "print_func"}
--обработка строк для конфигов
local function parse(str)
	local data = {}
	for line in str:gmatch("[^\r\n]+") do
		if line:match("%S") then
			local key, value = line:match("^%s*([^:]+):%s*(.-)%s*$")
			if key then
				if value == "true" then
					data[key] = true
				elseif value == "false" then
					data[key] = false
				elseif tonumber(value) then
					data[key] = tonumber(value)
				else
					data[key] = value
				end
			end
		end
	end
	return data
end
--проверка на наличие нужных пунктов
local function checkConf(header)
	local cnf = config[header]
	local req = baseline[header]
	for _, key in ipairs(req) do
		if cnf[key] == nil then
			return false
		end
	end
	return true
end

--заносит объект в список и придаёт ему позицию, значение, тип объекта
local function getId(obj, name)
	objects[globalCounterList] = name .. "." .. obj
	local id = globalCounter .. "." .. name .. "." .. obj
	globalCounter = globalCounter + 1
	globalCounterList = globalCounterList + 1
	return id
end
--поделим id
local function splitId(id)
	local n, name, obj = id:match("^(%d+)%.([^%.]+)%.([^%.]+)$")
	if not n then
		return false -- косяк
	end
	return tonumber(n), name, obj
end
--удаляем объект(без родителей)
function pix.deleteObject(id)
	for i = 1, #objects do
		if objects[i] == id then
			task.remove_from_table(objects, i)
			globalCounterList = globalCounterList - 1
			return true
		end
	end
	return false
end
--стиль конфа будет похож на css т.к. не хочу уйти в lvgl(если он вам нужен то биндинги есть, юзайте)
function pix.loadConfig(data)
	local parsed = parse(data)
	if parsed.element == nil then
		return false
	end
	local element = parsed.element
	parsed.element = nil
	config[element] = parsed
	return true
end

--это типо когда открывают
function pix.discollapsePrint()

end
--создание окна
--[[
#### обязательные пункты ####
width 
height
name
ico
loop_func
print_func

#### дополнительно ####
backscreen_color
border_color
border_size
pos_x
pos_y
title_color
title_high
title_background
title_align

]]

function pix.addWindow()
	if not checkConf("window") then
		return false
	end
	local win = config["window"]
	kernel.add_process(win.name, win.ico, win.loop_func, win.print_func, false, win.width, win.height, "pix.discollapsePrint()")
	setTextColor(hexTo565(win.title_color))
	fillRect(0, 0, sizeX, sizeY, hexTo565(kernel.windowColor))
	fillRect(1, 1, sizeX - 2, 15, hexTo565(kernel.windowHeadColor))
	drawRect(0, 0, sizeX, sizeY, hexTo565(kernel.windowOutlineColor))
	drawLine(0, 15, sizeX - 1, 15, hexTo565(kernel.windowOutlineColor))
	if utf8.len(name) < (sizeX // 6) then
		drawString(name, 4, 4)
	else
		drawString(string.sub(name, 1, (sizeX // 6) - 3) .. "...", 4, 4)
	end
	return getId("window", win.name)
end

--тик всех задач
function pix.tickEvents()
	if #events == 0 then
		return false
	end
	for i = 1, #events do
		local pos, name, obj = splitId(events[i])
		if obj == "window" then --эвент окна
		end
		if obj == "label" then --эвент окна
		end
		if obj == "button" then --кнопки
		end
		if obj == "image" then --изображения
		end
		if obj == "progress_bar" then --прогрес бара
		end
	end
end
