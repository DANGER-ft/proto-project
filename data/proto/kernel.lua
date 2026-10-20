--чтобы vs:code не ругался. но я во featherPad-е пишу, это забота о вас
--...
--...
--...
--я так подумал хватит заботы UwU, коментарии будут позже
---@diagnostic disable: undefined-global
local hot_bar_list = {}
local hot_bar_busy = { 0, 0, 0, 0, 0, 0 }
local path_to_func = {}

ui_widgets_name = {}
ui_widgets_pos = {}
ui_widgets_path = {}

local guiIsClose = true
local time = Timer()

local used = {}

pressedKey = ""

local gui_max_punct = 7
local changed_punct = 1
local gui_punct = Counter(gui_max_punct, 1, true)
local gui_elements = { "restart", "fayly olega", "PelzigerFemj", "settings", "ptView", "you're gay", "trololo.mp3" }
local gui_pathes = { "restart()", "setupExplorer()", "ex()" }

local main_x = Counter(3, 1, true)
local main_y = Counter(4, 1, false)
local _main_x = 1
local _main_y = 1

local STRIDE = 7
local active_win = 0

opened_window = {}

local widget = {
	{ "", "", "" },
	{ "", "", "" },
	{ "", "", "" },
	{ "", "", "" },
}
local cursor_change = ""

local function win_base(idx)
	return (idx - 1) * STRIDE + 1
end

local function win_count()
	local n = 0
	while opened_window[win_base(n + 1)] ~= nil do
		n = n + 1
	end
	return n
end

local function win_get(idx, f)
	return opened_window[win_base(idx) + f]
end
local function win_set(idx, f, v)
	opened_window[win_base(idx) + f] = v
end

local function win_remove(idx)
	local n = win_count()
	if idx < 1 or idx > n then
		return
	end
	local b = win_base(idx)
	local total = n * STRIDE
	for i = b, total - STRIDE do
		opened_window[i] = opened_window[i + STRIDE]
	end
	for i = total - STRIDE + 1, total do
		opened_window[i] = nil
	end
end

function load_link()
	local links = {}
	pathes_to_img = {}
	for i = 1, 12 do
		links[i] = readConfig("/proto/user/config/userLinks.conf", "link" .. i)
		if links[i] ~= nil then
			widget[i] = "/proto/user/links/" .. links[i]
		end
	end
	print("next step")
	for i = 1, #links do
		pathes_to_img[i] = readConfig("/proto/user/links/" .. links[i], "img")
	end
end

function load_conf()
	labelBaseOutline = load_ui_config("labelBaseOutline")
	hotBarColor = load_ui_config("hotBarColor")
	labelOutline = load_ui_config("labelOutline")
	pathToBackscreen = load_ui_config("pathToBackscreen")
	guiChangeColor = load_ui_config("guiChangeColor")
	guiBackColor = load_ui_config("guiBackColor")
	guiTextColor = load_ui_config("guiTextColor")
	windowColor = load_ui_config("windowColor")
	windowHeadColor = load_ui_config("windowHeadColor")
	windowOutlineColor = load_ui_config("windowOutlineColor")
end

function ex()
	returned = 4
	global_while = false
	remove_all()
	fillScreen(rgbTo565(0, 0, 0))
end

function restart()
	drawFsImage(0, 0, "/proto/res/protoIco.png")
	setTextColor(rgbTo565(42, 82, 190))
	drawCentreString("reloading...", 64, 140)
	delay(700)
	returned = 2
	global_while = false
	remove_all()
	fillScreen(rgbTo565(0, 0, 0))
end

function once(text)
	if not used[text] then
		used[text] = true
		return true
	else
		return false
	end
end

function load_ui_config(key)
	_out = readConfig("/proto/user/config/uiConfig.conf", key)
	if _out ~= nil then
		out = _out
	else
		out = readConfig("/proto/config/defUiConfig.conf", key)
	end
	return out
end

function remove_from_table(tbl, index)
	local removed = tbl[index]
	for i = index, #tbl - 1 do
		tbl[i] = tbl[i + 1]
	end
	tbl[#tbl] = nil
	return removed
end

function kernel_loop_setup()
	print("boot starting")
	drawFsImage(0, 0, "/proto/res/protoIco.png")
	drawRect(32, 140, 64, 10, hexTo565("#319177"))
	i = 0
	setTextSize(1)
	while true do
		fillRect(33, 141, i, 8, hexTo565("#2A52BE"))
		i = i + 1
		delay(1)
		if i == 63 then
			break
		end
		if i == 18 then
			load_conf()
		end
		if i == 40 then
			load_link()
		end
	end
	fillScreen(rgbTo565(0, 0, 0))
	make_hot_bar()
	print_backscreen()
	cursor_change = "main"
	add_function(1, "loop", "kernel_loop()")
	add_function(0, "loop", "gui()")
	add_function(0, "loop", "navigation_for_windows()")
	add_function(0, "loop", "main_navigation()")
end

function main_navigation()
	if cursor_change == "main" then
		if pressedKey == "DOWN" then
			drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(labelBaseOutline))
			_main_y = main_y:count(pressedKey == "UP", pressedKey == "DOWN")
			pressedKey = "-1"
		end
		if pressedKey == "UP" then
			drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(labelBaseOutline))
			_main_y = main_y:count(pressedKey == "UP", pressedKey == "DOWN")
			pressedKey = "-1"
		end
		if pressedKey == "LEFT" then
			drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(labelBaseOutline))
			_main_x = main_x:count(pressedKey == "RIGHT", pressedKey == "LEFT")
			pressedKey = "-1"
		end
		if pressedKey == "RIGHT" then
			drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(labelBaseOutline))
			_main_x = main_x:count(pressedKey == "RIGHT", pressedKey == "LEFT")
			pressedKey = "-1"
		end
		drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(labelOutline))
		if pressedKey == "ENTER" then
			local atr = readConfig(widget[main_x:read() + ((main_y:read() - 1) * 4)], "attribute")
			if art ~= nil then
				_G[atr] = readConfig(widget[main_x:read() + ((main_y:read() - 1) * 4)], "var")
			end
			add_function(0, "single", readConfig(widget[main_x:read() + ((main_y:read() - 1) * 4)], "func"))
			pressedKey = "-1"
		end
	end
end

function gui_navigation()
	if pressedKey == "UP" then
		drawRect(0, 160 - ((changed_punct * 14) + 20), 80, 15, hexTo565(hotBarColor))
		changed_punct = gui_punct:count(pressedKey == "UP", pressedKey == "DOWN")
		pressedKey = "-1"
	end
	if pressedKey == "DOWN" then
		drawRect(0, 160 - ((changed_punct * 14) + 20), 80, 15, hexTo565(hotBarColor))
		changed_punct = gui_punct:count(pressedKey == "UP", pressedKey == "DOWN")
		pressedKey = "-1"
	end
	drawRect(0, 160 - ((changed_punct * 14) + 20), 80, 15, hexTo565(guiChangeColor))
	if pressedKey == "ENTER" then
		add_function(0, "loop", gui_pathes[changed_punct])
		kill_gui()
		remove_function("gui_navigation()")
		pressedKey = "-1"
	end
end

function gui()
	if pressedKey == "[GUI]" and guiIsClose then
		if active_win > 0 and win_get(active_win, 4) == 0 then
			collapse_process(win_get(active_win, 1))
			win_set(active_win, 4, 1)
		end
		changed_punct = 1
		gui_punct:clear()
		open_gui()
		add_function(0, "loop", "gui_navigation()")
		pressedKey = "-1"
	end
	if pressedKey == "[GUI]" and guiIsClose == false then
		remove_function("gui_navigation()")
		kill_gui()
		gui_punct:clear()
		pressedKey = "-1"
	end
end

function open_gui()
	cursor_temp = cursor_change
	cursor_change = "gui"
	guiColor = hexTo565(hotBarColor)
	fillRect(0, 43, 80, 98, hexTo565(guiBackColor))
	drawRect(0, 42, 80, 100, guiColor)
	setTextColor(hexTo565(guiTextColor))
	for i = 1, 7, 1 do
		drawLine(0, (i * 14) + 42, 80, (i * 14) + 42, guiColor)
		drawString(gui_elements[8 - i], 1, (i * 14) + 32)
	end
	guiIsClose = false
end

function navigation_for_windows()
	if pressedKey == "HOME" then
		if active_win > 0 and win_get(active_win, 4) == 0 then
			collapse_process(win_get(active_win, 1))
			win_set(active_win, 4, 1)
			cursor_change = "main"
		end
		pressedKey = "-1"
	end
	if pressedKey == "F2" then
		local n = win_count()
		if n > 0 then
			if active_win > 0 and win_get(active_win, 4) == 0 then
				collapse_process(win_get(active_win, 1))
				win_set(active_win, 4, 1)
			end
			active_win = (active_win % n) + 1
			discollapse_process(
				win_get(active_win, 0),
				win_get(active_win, 1),
				win_get(active_win, 2),
				win_get(active_win, 3),
				win_get(active_win, 6)
			)
			win_set(active_win, 4, 0)
		end
		pressedKey = "-1"
	end
	if pressedKey == "ESC" then
		if active_win > 0 then
			local b = win_base(active_win)
			local func_to_print = opened_window[b + 1]
			local func = opened_window[b + 5]
			kill_process(func_to_print, func)
			win_remove(active_win)
			local n = win_count()
			if n == 0 then
				active_win = 0
				cursor_change = "main"
				print_backscreen()
			else
				if active_win > n then
					active_win = n
				end
				discollapse_process(
					win_get(active_win, 0),
					win_get(active_win, 1),
					win_get(active_win, 2),
					win_get(active_win, 3),
					win_get(active_win, 6)
				)
				win_set(active_win, 4, 0)
			end
		end
		pressedKey = "-1"
	end
end

function kill_gui()
	gui_punct:clear()
	cursor_change = cursor_temp
	print_backscreen()
	guiIsClose = true
end

function kernel_loop()
	if time:isReady(10) then
		pressedKey = get_key()
		time:reset()
	end
end

function make_window(name, sizeX, sizeY, func_loop)
	setTextColor(0, 0, 0)
	fillRect(0, 0, sizeX, sizeY, hexTo565(windowColor))
	fillRect(1, 1, sizeX - 2, 15, hexTo565(windowHeadColor))
	drawRect(0, 0, sizeX, sizeY, hexTo565(windowOutlineColor))
	drawLine(0, 15, sizeX - 1, 15, hexTo565(windowOutlineColor))
	if utf8.len(name) < (sizeX // 6) then
		drawString(name, 4, 4)
	else
		drawString(string.sub(name, 1, (sizeX // 6) - 3) .. "...", 4, 4)
	end
	cursor_change = name
	add_function(0, "loop", func_loop)
end

function kill_window(func)
	cursor_change = "main"
	remove_function(func)
	print_backscreen()
end

function print_backscreen()
	fastPngDraw(pathToBackscreen, 0, 0)
	base_color_outline_widget = hexTo565(labelBaseOutline)
	for y = 0, 3 do
		for x = 0, 2 do
			drawRect(x * 43 + 1, y * 35 + 1, 40, 35, base_color_outline_widget)
			if pathes_to_img[x + (y * 3) + 1] ~= nil then
				fastPngDraw(pathes_to_img[x + (y * 3) + 1], (x * 43) + 4, (y * 35) + 2)
			end
		end
	end
end

function make_hot_bar()
	drawFsImage(2, 143, "/proto/res/logo.png")
	drawRect(0, 141, 127, 19, hexTo565(hotBarColor))
	drawLine(18, 142, 18, 160, hexTo565(hotBarColor))
	drawLine(26, 142, 26, 160, hexTo565(hotBarColor))
end

function add_hot_bar_punct(path_to_ico, path_func)
	i = 1
	while true do
		if hot_bar_busy[i] == 1 then
			i = i + 1
		else
			if i < 7 then
				hot_bar_busy[i] = 1
				hot_bar_list[i] = path_to_ico
				path_to_func[i] = path_func
				add_element_to_hot_bar(i, path_to_ico)
				return 0
			else
				return 1
			end
		end
	end
end

function add_element_to_hot_bar(pos, ico)
	if ico ~= "" then
		local position = 26 + ((pos - 1) * 17)
		drawFsImage(position + 1, 142, ico)
		drawLine(position, 142, position, 160, hexTo565(hotBarColor))
		drawLine(position + 17, 142, position + 17, 160, hexTo565(hotBarColor))
	else
		print("killing...")
		fillRect(1, 141, 127, 19, rgbTo565(0, 0, 0))
		drawFsImage(2, 143, "/proto/res/logo.png")
		drawRect(1, 141, 127, 19, hexTo565(hotBarColor))
		drawLine(18, 142, 18, 160, hexTo565(hotBarColor))
		drawLine(26, 142, 26, 160, hexTo565(hotBarColor))

		for i = 1, #hot_bar_list do
			if hot_bar_list[i] ~= nil then
				local position = 26 + ((i - 1) * 17)
				drawFsImage(position + 1, 142, hot_bar_list[i])
				drawLine(position, 142, position, 160, hexTo565(hotBarColor))
				drawLine(position + 17, 142, position + 17, 160, hexTo565(hotBarColor))
			end
		end
	end
end

function kill_hot_bur_element(path)
	print("hotbar element kill")
	for i = 1, #hot_bar_list do
		print("searching " .. path)
		if path_to_func[i] == path then
			print("removing...")
			remove_from_table(hot_bar_list, i)
			remove_from_table(hot_bar_busy, i)
			remove_from_table(path_to_func, i)
			print("clearing...")
			add_element_to_hot_bar(0, "")
			print("hotbar element killed")
			return 0
		else
			print("can not find")
		end
	end
	return 1
end

function add_process(name, ico, func, func_to_print, windowed, sizeX, sizeY, once_print_func)
	print("added process")
	if ico == "null" then
		ico = "/proto/res/none.png"
	end

	local idx = win_count() + 1
	local b = win_base(idx)
	opened_window[b + 0] = name
	opened_window[b + 1] = func_to_print
	opened_window[b + 2] = sizeX
	opened_window[b + 3] = sizeY
	opened_window[b + 4] = windowed and 0 or 1
	opened_window[b + 5] = func
	opened_window[b + 6] = once_print_func

	if windowed == true then
		for i = 1, idx - 1 do
			if win_get(i, 4) == 0 then
				collapse_process(win_get(i, 1))
				win_set(i, 4, 1)
			end
		end
		make_window(name, sizeX, sizeY, func_to_print, once_print_func)
		active_win = idx
	end

	add_hot_bar_punct(ico, func)
	add_function(0, "loop", func)
	changed_punct = name
end

function collapse_process(func_to_print)
	print("collapsed process")
	kill_window(func_to_print)
	cursor_change = "main"
	print_backscreen()
end

function discollapse_process(name, func_to_print, sizeX, sizeY, once_print_func)
	print("process discollapse")
	make_window(name, sizeX, sizeY, func_to_print)
	if once_print_func then
		add_function(0, "single", once_print_func)
	end
	cursor_change = name
end

function kill_process(func_to_print, func)
	print("process killed")
	print("killing " .. func .. " " .. func_to_print)
	kill_window(func_to_print)
	kill_hot_bur_element(func)
	remove_function(func)
	changed_punct = "main"
end
