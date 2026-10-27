kernel = {}

--чтобы vs:code не ругался. но я во featherPad-е пишу, это забота о вас
--...
--...
--...
--я так подумал хватит заботы UwU, коментарии будут позже
---@diagnostic disable: undefined-global
local hot_bar_list = {}
local hot_bar_busy = { 0, 0, 0, 0, 0, 0 }
local path_to_func = {}

kernel.ui_widgets_name = {}
kernel.ui_widgets_pos = {}
kernel.ui_widgets_path = {}

local guiIsClose = true
local time = Timer()

local used = {}

kernel.pressedKey = ""

local gui_max_punct = 7
local changed_punct = 1
local gui_punct = Counter(gui_max_punct, 1, true)
local gui_elements = { "restart", "fayly olega", "PelzigerFemj", "settings", "ptView", "you're gay", "trololo.mp3" }
local gui_pathes = { "kernel.restart()", "explorer.setupExplorer()", "kernel.ex()" }

local main_x = Counter(3, 1, true)
local main_y = Counter(4, 1, false)
local _main_x = 1
local _main_y = 1

local STRIDE = 8
local active_win = 0

kernel.opened_window = {}

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
	while kernel.opened_window[win_base(n + 1)] ~= nil do
		n = n + 1
	end
	return n
end

local function win_get(idx, f)
	return kernel.opened_window[win_base(idx) + f]
end
local function win_set(idx, f, v)
	kernel.opened_window[win_base(idx) + f] = v
end

local function win_remove(idx)
	local n = win_count()
	if idx < 1 or idx > n then
		return
	end
	local b = win_base(idx)
	local total = n * STRIDE
	for i = b, total - STRIDE do
		kernel.opened_window[i] = kernel.opened_window[i + STRIDE]
	end
	for i = total - STRIDE + 1, total do
		kernel.opened_window[i] = nil
	end
end
--событие процессов останавливающее их и выделающее время на функцию и сервисы
function kernel.task_event(func)
	local tasks = task.tasks
	local taskType = task.taskType
	local taskStatus = task.taskStatus
	local deleted = task.deleted
	local newFunc = task.newFunc
	task.remove_all()
	task.add_function(1, "loop", "kernel.kernel_loop()")
	if kernel.pressedKey == "INSERT" then
		task.tasks = tasks
		task.taskType = taskType
		task.taskStatus = taskStatus
		task.deleted = deleted
		task.newFunc = newFunc
		kernel.pressedKey = "-1"
	end
end

--подгрузка ярлыков
function kernel.load_link()
	local links = {}
	kernel.pathes_to_img = {}
	for i = 1, 12 do
		links[i] = readConfig("/proto/user/config/userLinks.conf", "link" .. i)
		if links[i] ~= nil then
			widget[i] = "/proto/user/links/" .. links[i]
		end
	end
	print("next step")
	for i = 1, #links do
		kernel.pathes_to_img[i] = readConfig("/proto/user/links/" .. links[i], "img")
	end
end
--подгрузка конфигов
function kernel.load_conf()
	kernel.labelBaseOutline = kernel.load_ui_config("labelBaseOutline")
	kernel.hotBarColor = kernel.load_ui_config("hotBarColor")
	kernel.labelOutline = kernel.load_ui_config("labelOutline")
	kernel.pathToBackscreen = kernel.load_ui_config("pathToBackscreen")
	kernel.guiChangeColor = kernel.load_ui_config("guiChangeColor")
	kernel.guiBackColor = kernel.load_ui_config("guiBackColor")
	kernel.guiTextColor = kernel.load_ui_config("guiTextColor")
	kernel.windowColor = kernel.load_ui_config("windowColor")
	kernel.windowHeadColor = kernel.load_ui_config("windowHeadColor")
	kernel.windowOutlineColor = kernel.load_ui_config("windowOutlineColor")
end
--типо первый ли раз ключ
function kernel.once(text)
	if not used[text] then
		used[text] = true
		return true
	else
		return false
	end
end
--теперь это для тестов просто
function kernel.ex()
	kernel.add_process("ex", "null", "kernel.ex_loop()", "kernel.ex_print()", false, 128, 140, "kernel.ex_once_print()")
	task.remove_function("kernel.ex()")
end

--всякая шляпа
function kernel.ex_loop()
	if kernel.once("loop") then
		print("loop")
	end
end
function kernel.ex_print()
	if kernel.once("print") then
		print("print")
	end
end
function kernel.ex_once_print()
	print("once")
end
--перезагрузка (exit code 2)
function kernel.restart()
	drawFsImage(0, 0, "/proto/res/protoIco.png")
	setTextColor(rgbTo565(42, 82, 190))
	drawCentreString("reloading...", 64, 140)
	delay(700)
	returned = 2
	root.global_while = false
	task.remove_all()
	fillScreen(rgbTo565(0, 0, 0))
end

function kernel.load_ui_config(key)
	_out = readConfig("/proto/user/config/uiConfig.conf", key)
	if _out ~= nil then
		out = _out
	else
		out = readConfig("/proto/config/defUiConfig.conf", key)
	end
	return out
end

function kernel.remove_from_table(tbl, index)
	local removed = tbl[index]
	for i = index, #tbl - 1 do
		tbl[i] = tbl[i + 1]
	end
	tbl[#tbl] = nil
	return removed
end

function kernel.kernel_loop_setup()
	print("boot starting")
	drawFsImage(0, 0, "/proto/res/protoIco.png")
	drawRect(32, 140, 64, 10, hexTo565("#319177"))
	i = 0
	setTextSize(1)
	while true do
		fillRect(33, 141, i, 8, hexTo565("#2A52BE"))
		i = i + 1
		if i == 63 then
			break
		end
		if i == 18 then
			kernel.load_conf()
		end
		if i == 40 then
			kernel.load_link()
		end
	end
	fillScreen(rgbTo565(0, 0, 0))
	kernel.make_hot_bar()
	kernel.print_backscreen()
	cursor_change = "main"
	task.add_function(1, "loop", "kernel.kernel_loop()")
	task.add_function(0, "loop", "kernel.gui()")
	task.add_function(0, "loop", "kernel.navigation_for_windows()")
	task.add_function(0, "loop", "kernel.main_navigation()")
end

function kernel.main_navigation()
	if cursor_change == "main" then
		if kernel.pressedKey == "DOWN" then
			drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(kernel.labelBaseOutline))
			_main_y = main_y:count(kernel.pressedKey == "UP", kernel.pressedKey == "DOWN")
			kernel.pressedKey = "-1"
		end
		if kernel.pressedKey == "UP" then
			drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(kernel.labelBaseOutline))
			_main_y = main_y:count(kernel.pressedKey == "UP", kernel.pressedKey == "DOWN")
			kernel.pressedKey = "-1"
		end
		if kernel.pressedKey == "LEFT" then
			drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(kernel.labelBaseOutline))
			_main_x = main_x:count(kernel.pressedKey == "RIGHT", kernel.pressedKey == "LEFT")
			kernel.pressedKey = "-1"
		end
		if kernel.pressedKey == "RIGHT" then
			drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(kernel.labelBaseOutline))
			_main_x = main_x:count(kernel.pressedKey == "RIGHT", kernel.pressedKey == "LEFT")
			kernel.pressedKey = "-1"
		end
		drawRect(((_main_x - 1) * 43) + 1, ((_main_y - 1) * 35) + 1, 40, 35, hexTo565(kernel.labelOutline))
		if kernel.pressedKey == "ENTER" then
			local atr = readConfig(widget[main_x:read() + ((main_y:read() - 1) * 4)], "attribute")
			if atr ~= nil then
				_G[atr] = readConfig(widget[main_x:read() + ((main_y:read() - 1) * 4)], "var")
			end
			task.add_function(0, "single", readConfig(widget[main_x:read() + ((main_y:read() - 1) * 4)], "func"))
			kernel.pressedKey = "-1"
		end
	end
end

function kernel.gui_navigation()
	if kernel.pressedKey == "UP" then
		drawRect(0, 160 - ((changed_punct * 14) + 20), 80, 15, hexTo565(kernel.hotBarColor))
		changed_punct = gui_punct:count(kernel.pressedKey == "UP", kernel.pressedKey == "DOWN")
		kernel.pressedKey = "-1"
	end
	if kernel.pressedKey == "DOWN" then
		drawRect(0, 160 - ((changed_punct * 14) + 20), 80, 15, hexTo565(kernel.hotBarColor))
		changed_punct = gui_punct:count(kernel.pressedKey == "UP", kernel.pressedKey == "DOWN")
		kernel.pressedKey = "-1"
	end
	drawRect(0, 160 - ((changed_punct * 14) + 20), 80, 15, hexTo565(kernel.guiChangeColor))
	if kernel.pressedKey == "ENTER" then
		task.add_function(0, "loop", gui_pathes[changed_punct])
		kernel.kill_gui()
		task.remove_function("kernel.gui_navigation()")
		kernel.pressedKey = "-1"
	end
end

function kernel.gui()
	if kernel.pressedKey == "[GUI]" and guiIsClose then
		if active_win > 0 and win_get(active_win, 4) == 0 then
			kernel.collapse_process(win_get(active_win, 1))
			win_set(active_win, 4, 1)
		end
		changed_punct = 1
		gui_punct:clear()
		kernel.open_gui()
		task.add_function(0, "loop", "kernel.gui_navigation()")
		kernel.pressedKey = "-1"
	end
	if kernel.pressedKey == "[GUI]" and guiIsClose == false then
		task.remove_function("kernel.gui_navigation()")
		kernel.kill_gui()
		gui_punct:clear()
		kernel.pressedKey = "-1"
	end
end

function kernel.open_gui()
	kernel.cursor_temp = cursor_change
	cursor_change = "gui"
	kernel.guiColor = hexTo565(kernel.hotBarColor)
	fillRect(0, 43, 80, 98, hexTo565(kernel.guiBackColor))
	drawRect(0, 42, 80, 100, kernel.guiColor)
	setTextColor(hexTo565(kernel.guiTextColor))
	for i = 1, 7, 1 do
		drawLine(0, (i * 14) + 42, 80, (i * 14) + 42, kernel.guiColor)
		drawString(gui_elements[8 - i], 1, (i * 14) + 32)
	end
	guiIsClose = false
end

function kernel.navigation_for_windows()
	if kernel.pressedKey == "HOME" then
		if active_win > 0 and win_get(active_win, 4) == 0 then
			kernel.collapse_process(win_get(active_win, 1))
			win_set(active_win, 4, 1)
			cursor_change = "main"
		end
		kernel.pressedKey = "-1"
	end
	if kernel.pressedKey == "F2" then
		local n = win_count()
		if n > 0 then
			if active_win > 0 and win_get(active_win, 4) == 0 then
				kernel.collapse_process(win_get(active_win, 1))
				win_set(active_win, 4, 1)
			end
			active_win = (active_win % n) + 1
			kernel.discollapse_process(
				win_get(active_win, 0),
				win_get(active_win, 1),
				win_get(active_win, 2),
				win_get(active_win, 3),
				win_get(active_win, 6),
				win_get(active_win, 7)
			)
			win_set(active_win, 4, 0)
		end
		kernel.pressedKey = "-1"
	end
	if kernel.pressedKey == "ESC" then
		if active_win > 0 then
			local b = win_base(active_win)
			local func_to_print = kernel.opened_window[b + 1]
			local func = kernel.opened_window[b + 5]
			kernel.kill_process(func_to_print, func)
			win_remove(active_win)
			local n = win_count()
			if n == 0 then
				active_win = 0
				cursor_change = "main"
				kernel.print_backscreen()
			else
				if active_win > n then
					active_win = n
				end
				kernel.discollapse_process(
					win_get(active_win, 0),
					win_get(active_win, 1),
					win_get(active_win, 2),
					win_get(active_win, 3),
					win_get(active_win, 6),
					win_get(active_win, 7)
				)
				win_set(active_win, 4, 0)
			end
		end
		kernel.pressedKey = "-1"
	end
end

function kernel.kill_gui()
	gui_punct:clear()
	cursor_change = kernel.cursor_temp
	kernel.print_backscreen()
	guiIsClose = true
end

function kernel.kernel_loop()
	if time:isReady(10) then
		kernel.pressedKey = get_key()
		time:reset()
	end
end

function kernel.make_window(name, sizeX, sizeY, func_loop)
	setTextColor(0, 0, 0)
	fillRect(0, 0, sizeX, sizeY, hexTo565(kernel.windowColor))
	fillRect(1, 1, sizeX - 2, 15, hexTo565(kernel.windowHeadColor))
	drawRect(0, 0, sizeX, sizeY, hexTo565(kernel.windowOutlineColor))
	drawLine(0, 15, sizeX - 1, 15, hexTo565(kernel.windowOutlineColor))
	if utf8.len(name) < (sizeX // 6) then
		drawString(name, 4, 4)
	else
		drawString(string.sub(name, 1, (sizeX // 6) - 3) .. "...", 4, 4)
	end
	cursor_change = name
	task.add_function(0, "loop", func_loop)
end

function kernel.kill_window(func)
	cursor_change = "main"
	task.remove_function(func)
	kernel.print_backscreen()
end

function kernel.print_backscreen()
	fastPngDraw(kernel.pathToBackscreen, 0, 0)
	kernel.base_color_outline_widget = hexTo565(kernel.labelBaseOutline)
	for y = 0, 3 do
		for x = 0, 2 do
			drawRect(x * 43 + 1, y * 35 + 1, 40, 35, kernel.base_color_outline_widget)
			if kernel.pathes_to_img[x + (y * 3) + 1] ~= nil then
				fastPngDraw(kernel.pathes_to_img[x + (y * 3) + 1], (x * 43) + 4, (y * 35) + 2)
			end
		end
	end
end

function kernel.make_hot_bar()
	drawFsImage(2, 143, "/proto/res/logo.png")
	drawRect(0, 141, 127, 19, hexTo565(kernel.hotBarColor))
	drawLine(18, 142, 18, 160, hexTo565(kernel.hotBarColor))
	drawLine(26, 142, 26, 160, hexTo565(kernel.hotBarColor))
end

function kernel.add_hot_bar_punct(path_to_ico, path_func)
	i = 1
	while true do
		if hot_bar_busy[i] == 1 then
			i = i + 1
		else
			if i < 7 then
				hot_bar_busy[i] = 1
				hot_bar_list[i] = path_to_ico
				path_to_func[i] = path_func
				kernel.add_element_to_hot_bar(i, path_to_ico)
				return 0
			else
				return 1
			end
		end
	end
end

function kernel.add_element_to_hot_bar(pos, ico)
	if ico ~= "" then
		local position = 26 + ((pos - 1) * 17)
		drawFsImage(position + 1, 142, ico)
		drawLine(position, 142, position, 160, hexTo565(kernel.hotBarColor))
		drawLine(position + 17, 142, position + 17, 160, hexTo565(kernel.hotBarColor))
	else
		print("killing...")
		fillRect(1, 141, 127, 19, rgbTo565(0, 0, 0))
		drawFsImage(2, 143, "/proto/res/logo.png")
		drawRect(1, 141, 127, 19, hexTo565(kernel.hotBarColor))
		drawLine(18, 142, 18, 160, hexTo565(kernel.hotBarColor))
		drawLine(26, 142, 26, 160, hexTo565(kernel.hotBarColor))

		for i = 1, #hot_bar_list do
			if hot_bar_list[i] ~= nil then
				local position = 26 + ((i - 1) * 17)
				drawFsImage(position + 1, 142, hot_bar_list[i])
				drawLine(position, 142, position, 160, hexTo565(kernel.hotBarColor))
				drawLine(position + 17, 142, position + 17, 160, hexTo565(kernel.hotBarColor))
			end
		end
	end
end

function kernel.kill_hot_bur_element(path)
	print("hotbar element kill")
	for i = 1, #hot_bar_list do
		print("searching " .. path)
		if path_to_func[i] == path then
			print("removing...")
			kernel.remove_from_table(hot_bar_list, i)
			kernel.remove_from_table(hot_bar_busy, i)
			kernel.remove_from_table(path_to_func, i)
			print("clearing...")
			kernel.add_element_to_hot_bar(0, "")
			print("hotbar element killed")
			return 0
		else
			print("can not find")
		end
	end
	return 1
end

function kernel.add_process(name, ico, func, func_to_print, windowed, sizeX, sizeY, once_print_func)
	print("added process")
	if ico == "null" or ico == "nil" then
		ico = "/proto/res/none.png"
	end

	local idx = win_count() + 1
	local b = win_base(idx)
	kernel.opened_window[b + 0] = name
	kernel.opened_window[b + 1] = func_to_print
	kernel.opened_window[b + 2] = sizeX
	kernel.opened_window[b + 3] = sizeY
	kernel.opened_window[b + 4] = windowed and 0 or 1
	kernel.opened_window[b + 5] = func
	kernel.opened_window[b + 6] = once_print_func
	kernel.opened_window[b + 7] = windowed
	if windowed == true then
		for i = 1, idx - 1 do
			if win_get(i, 4) == 0 then
				kernel.collapse_process(win_get(i, 1))
				win_set(i, 4, 1)
			end
		end
		kernel.make_window(name, sizeX, sizeY, func_to_print, once_print_func)
		active_win = idx
	end

	kernel.add_hot_bar_punct(ico, func)
	task.add_function(0, "loop", func)
	changed_punct = name
end

function kernel.collapse_process(func_to_print)
	print("collapsed process")
	kernel.kill_window(func_to_print)
	cursor_change = "main"
	kernel.print_backscreen()
end

function kernel.discollapse_process(name, func_to_print, sizeX, sizeY, once_print_func, windowed)
	print("process discollapse")
	print(windowed)
	if windowed then
		kernel.make_window(name, sizeX, sizeY, func_to_print)
	else
		task.add_function(0, "loop", func_to_print)
	end
	if once_print_func then
		task.add_function(0, "single", once_print_func)
	end
	cursor_change = name
end

function kernel.kill_process(func_to_print, func)
	print("process killed")
	print("killing " .. func .. " " .. func_to_print)
	kernel.kill_window(func_to_print)
	kernel.kill_hot_bur_element(func)
	task.remove_function(func)
	changed_punct = "main"
end
