
--я не буду это коментировать. как нибудь сами.
formatsTxt =
	{ ".txt", ".csv", ".tsv", ".json", ".jsonl", ".xml", ".html" },
	{ ".md", ".rst", ".yaml", ".toml", ".ini", ".cfg", ".conf", ".log" },
	{ ".sql", ".env", ".properties", ".srt", ".vtt", ".rtf" }
formatsImg = { ".jpg", ".png", ".gif", ".webp", ".svg", ".ico" }
formatsCode = { ".js", ".cpp", ".py", ".rb", ".php", ".sh", ".bash", ".bat", ".cmd", ".lua" }

nullPointTasks = { "make file", "make dir" }
objectPointTasks = { "make file", "make dir", "rename", "move", "copy", "past", "delete", "cut" }

local function inputAdd(ch)
	if ch == nil then
		return
	end
	if #ch ~= 1 then
		return
	end
	inputBuffer = inputBuffer .. ch
end

local function inputRemove()
	if #inputBuffer > 0 then
		inputBuffer = inputBuffer:sub(1, -2)
	end
end

local function inputRead()
	return inputBuffer
end

function runInput()
	remove_function("contextMenu()")
	fillRect(14, 50, 100, 20, hexTo565("#556677"))
	drawRect(14, 50, 100, 20, hexTo565(windowOutlineColor))
	drawRect(24, 55, 80, 10, hexTo565(windowOutlineColor))
	drawString(inputRead() .. "_", 26, 55)
	pressedKey = "-1"
	add_function(0, "loop", "input()")
end
function input()
	if pressedKey == "ENTER" then
		print("exit")
		pressedKey = "-1"
		remove_function("input()")
		confirmEvent()
	end
	if pressedKey == "BACKSPACE" then
		print("/b")
		inputRemove()
		fillRect(25, 56, 78, 8, hexTo565("#556677"))
		drawString(inputRead() .. "_", 26, 57)
		pressedKey = "-1"
	end
	if pressedKey ~= "-1" then
		inputAdd(pressedKey)
		fillRect(25, 56, 78, 8, hexTo565("#556677"))
		drawString(inputRead() .. "_", 26, 57)
		pressedKey = "-1"
	end
end

function mkdir(name)
	createDir(name)
end
function rmdir(name)
	local files = scanFiles(name .. "/", "*", "*")
	local dirs = scanDirs(name .. "/")
	if dirs[1] ~= nil then
		return false
	end
	for i = 1, #files do
		deleteFile(files[i])
	end
	deleteDir(name)
end

function mkfile(name)
	createFile(name)
end
function rmfile(name)
	deleteFile(name)
end
function move(name, to)
	moveFile(name, to)
end
function rename(name, newName)
	renameFile(name, newName)
end
function write(name, text)
	writeFile(name, text)
end
function clear(name)
	clearFile(name)
end
function size(name)
	return fileSize(name)
end
local function goUp(folder)
	if folder == "/proto/" then
		return folder
	end
	local pos = folder:find("/[^/]*/?$")
	if not pos or pos <= 1 then
		return "/proto/"
	end
	local parent = folder:sub(1, pos)
	if #parent < #"/proto/" then
		return "/proto/"
	end
	return parent
end
local function merge(a, b)
	local r = {}
	for i = 1, #a do
		r[#r + 1] = a[i]
	end
	for i = 1, #b do
		r[#r + 1] = b[i]
	end
	return r
end
local function replacePrefix(str, prefix, replacement)
	if str:sub(1, #prefix) == prefix then
		return replacement .. str:sub(#prefix + 1)
	end
	return str
end

function pathHead(path)
	fillRect(1, 17, 128, 12, hexTo565(windowColor))
	drawRect(1, 16, 128, 14, hexTo565("#010203"))
	setTextColor(hexTo565("#010B13"))
	drawString(replacePrefix(path, standart_folder, "~/"), 5, 20)
end

function drawFiles(folder, page, p)
	setTextColor(hexTo565("#010B13"))
	for i = 1, 13, 1 do
		local full = fileList[i + ((page - 1) * 13)]
		if full == nil then
			break
		end

		local name = replacePrefix(full, folder, "")
		local isDir = name:sub(-1) == "/"
		if isDir then
			if utf8.len(name) < (120 // 6) then
				drawString(name, 11, (i * 8) + 25)
			else
				drawString(string.sub(name, 1, (120 // 6) - 4) .. "...", 11, (i * 8) + 25)
			end
		else
			if utf8.len(name) < (120 // 6) then
				drawString(name, 11, (i * 8) + 25)
			else
				drawString(string.sub(name, 1, (120 // 6) - 4) .. "...", 11, (i * 8) + 25)
			end
		end

		local printed = false
		if isDir then
			fastPngDraw("/proto/res/folder7x7.png", 3, (i * 8) + 25)
			printed = true
		end

		if not printed then
			local ext = "." .. string.match(full, "%.(.+)$")
			for it = 1, #formatsTxt do
				if formatsTxt[it] == ext then
					fastPngDraw("/proto/res/txt7x7.png", 3, (i * 8) + 25)
					printed = true
					break
				end
			end
		end
		if not printed then
			local ext = "." .. string.match(full, "%.(.+)$")
			for it = 1, #formatsImg do
				if formatsImg[it] == ext then
					fastPngDraw("/proto/res/image7x7.png", 3, (i * 8) + 25)
					printed = true
					break
				end
			end
		end
		if not printed then
			local ext = "." .. string.match(full, "%.(.+)$")
			for it = 1, #formatsCode do
				if formatsCode[it] == ext then
					fastPngDraw("/proto/res/lua7x7.png", 3, (i * 8) + 25)
					printed = true
					break
				end
			end
		end
		if not printed then
			fastPngDraw("/proto/res/none7x7.png", 3, (i * 8) + 25)
		end
	end
	drawLine(1, (p * 8) + 25, 1, (p * 8) + 33, hexTo565("#FF0800"))
end

function printContextMenu()
	if fileList[count:read() + ((filePage - 1) * 13)] == nil then
		fillRect(24, 60, 80, 20, hexTo565("#667788"))
		drawRect(24, 60, 80, 20, hexTo565(windowOutlineColor))
		drawLine(101, (context2p:read() * 10) + 50, 101, (context2p:read() * 10) + 60, hexTo565("#FF0800"))
		for i = 1, #nullPointTasks do
			drawLine(24, (i * 10) + 60, 103, (i * 10) + 60, hexTo565(windowOutlineColor))
			drawString(nullPointTasks[i], 26, (i * 10) + 52)
		end
	else
		fillRect(24, 30, 80, 80, hexTo565("#667788"))
		drawRect(24, 30, 80, 80, hexTo565(windowOutlineColor))
		drawLine(101, (context:read() * 10) + 20, 101, (context:read() * 10) + 30, hexTo565("#FF0800"))
		for i = 1, #objectPointTasks do
			drawLine(24, (i * 10) + 30, 103, (i * 10) + 30, hexTo565(windowOutlineColor))
			drawString(objectPointTasks[i], 26, (i * 10) + 22)
		end
	end
end

function confirmEvent()
	if context:read() == 1 or context2p:read() == 1 then
		mkfile(folder .. inputRead())
	end
	if context:read() == 2 or context2p:read() == 2 then
		mkdir(folder .. inputRead())
	end
	if context:read() == 3 then
		rename(fileList[count:read() + ((filePage - 1) * 13)], inputRead())
	end
	if context:read() == 4 then
		move(fileList[count:read() + ((filePage - 1) * 13)], inputRead())
	end
	if context:read() == 5 then
		copyPath = fileList[count:read() + ((filePage - 1) * 13)]
		cutFlag = false
	end
	if context:read() == 6 then
		local name = copyPath:gsub(".*/", "")
		local parentFolder = copyPath:match("^(.*/)[^/]*/?$") or ""
		if folder == parentFolder then
			local ext = name:match("(%.[^%.]+)$") or ""
			local shortName = name:match("^(.-)%.[^%.]+$") or name
			copyFile(copyPath, folder .. shortName .. "(copy)" .. ext)
		else
			copyFile(copyPath, folder .. name)
		end
		if cutFlag then
			rmfile(copyPath)
			cutFlag = false
		end
	end
	if context:read() == 7 then
		if fileList[count:read() + ((filePage - 1) * 13)]:sub(-1) == "/" then
			rmdir(fileList[count:read() + ((filePage - 1) * 13)])
		else
			rmfile(fileList[count:read() + ((filePage - 1) * 13)])
		end
	end
	if context:read() == 8 then
		copyPath = fileList[count:read() + ((filePage - 1) * 13)]
		cutFlag = true
	end
	fillRect(1, 30, 127, 109, hexTo565(windowColor))
	inputBuffer = ""
	context:clear()
	context2p:clear()
	_fileList = scanFiles(folder, "*", "*")
	dirList = scanDirs(folder)
	for i = 1, #dirList do
		dirList[i] = dirList[i] .. "/"
	end
	fileList = merge(dirList, _fileList)
	oncePrint()
	contextMenuOpen = false
	pressedKey = "-1"
	remove_function("contextMenu()")
	context:clear()
	context2p:clear()
end

function contextMenu()
	--fileList[count:read() + ((filePage - 1) * 13)]
	if pressedKey == "TAB" then
		remove_function("contextMenu()")
		fillRect(1, 30, 127, 109, hexTo565(windowColor))
		context:clear()
		context2p:clear()
		oncePrint()
		contextMenuOpen = false
		pressedKey = "-1"
	end
	if pressedKey == "UP" then
		if fileList[count:read() + ((filePage - 1) * 13)] == nil then
			drawLine(101, (context2p:read() * 10) + 50, 101, (context2p:read() * 10) + 60, hexTo565("#667788"))
		else
			drawLine(101, (context:read() * 10) + 20, 101, (context:read() * 10) + 30, hexTo565("#667788"))
		end
		context:count(true, false)
		context2p:count(true, false)
		printContextMenu()
		pressedKey = "-1"
	end
	if pressedKey == "DOWN" then
		if fileList[count:read() + ((filePage - 1) * 13)] == nil then
			drawLine(101, (context2p:read() * 10) + 50, 101, (context2p:read() * 10) + 60, hexTo565("#667788"))
		else
			drawLine(101, (context:read() * 10) + 20, 101, (context:read() * 10) + 30, hexTo565("#667788"))
		end
		context:count(false, true)
		context2p:count(true, false)
		printContextMenu()
		pressedKey = "-1"
	end
	if pressedKey == "ENTER" then
		pressedKey = "-1"
		local hasFile = fileList[count:read() + ((filePage - 1) * 13)] ~= nil
		local idx = hasFile and context:read() or context2p:read()
		if (hasFile and (idx == 1 or idx == 2 or idx == 3 or idx == 4)) or (not hasFile and (idx == 1 or idx == 2)) then
			runInput()
			return
		end
		confirmEvent()
	end
end

function loopExplorer() end

function printExplorer()
	whole = #fileList // 13
	remainder = #fileList % 13
	if remainder > 0 then
		maximum = whole + 1
	else
		maximum = whole
	end
	if not contextMenuOpen then
		if #fileList > 13 then
			if pressedKey == "PAGE_UP" and filePage < maximum then
				filePage = filePage + 1
				fillRect(1, 30, 127, 109, hexTo565(windowColor))
				count:clear()
				if filePage == whole - 1 then
					drawFiles(folder, filePage, count:read())
					maxPunctInPage = remainder
				else
					drawFiles(folder, filePage, count:read())
					maxPunctInPage = 13
				end
				pressedKey = "-1"
			end
			if pressedKey == "PAGE_DOWN" and filePage > 1 then
				filePage = filePage - 1
				fillRect(1, 30, 127, 109, hexTo565(windowColor))
				count:clear()
				if filePage == whole - 1 then
					drawFiles(folder, filePage, count:read())
					maxPunctInPage = remainder
				else
					drawFiles(folder, filePage, count:read())
					maxPunctInPage = 13
				end
				pressedKey = "-1"
			end
		end
		if pressedKey == "UP" then
			drawLine(1, (count:read() * 8) + 25, 1, (count:read() * 8) + 33, hexTo565(windowColor))
			count:count(pressedKey == "UP", pressedKey == "DOWN")
			if filePage == whole - 1 then
				if count:read() > maxPunctInPage then
					count:clear()
				end
				drawFiles(folder, filePage, count:read())
			else
				drawFiles(folder, filePage, count:read())
			end
			pressedKey = "-1"
		end
		if pressedKey == "DOWN" then
			drawLine(1, (count:read() * 8) + 25, 1, (count:read() * 8) + 33, hexTo565(windowColor))
			count:count(pressedKey == "UP", pressedKey == "DOWN")
			if count:read() > maxPunctInPage then
				count:clear()
			end
			if filePage == whole - 1 then
				drawFiles(folder, filePage, count:read())
			else
				drawFiles(folder, filePage, count:read())
			end
			pressedKey = "-1"
		end
		if count:read() > remainder + 2 and filePage == maximum then
			drawLine(1, (count:read() * 8) + 25, 1, (count:read() * 8) + 33, hexTo565(windowColor))
			count:clear()
			drawFiles(folder, filePage, count:read())
		end
		if count:read() > remainder + 1 and filePage == maximum then
			drawLine(1, (count:read() * 8) + 25, 1, (count:read() * 8) + 33, hexTo565(windowColor))
			count:count(true, false)
			drawFiles(folder, filePage, count:read())
		end
		if pressedKey == "ENTER" then
			local file = fileList[count:read() + ((filePage - 1) * 13)]
			if file:sub(-1) == "/" then
				folder = file
				_fileList = scanFiles(folder, "*", "*")
				dirList = scanDirs(folder)
				count:clear()
				for i = 1, #dirList do
					dirList[i] = dirList[i] .. "/"
				end
				fileList = merge(dirList, _fileList)
				fillRect(1, 30, 127, 109, hexTo565(windowColor))
				pathHead(folder)
				drawFiles(folder, 1, 1)
			end
			pressedKey = "-1"
		end
		if pressedKey == "BACKSPACE" then
			count:clear()
			folder = goUp(folder)
			_fileList = scanFiles(folder, "*", "*")
			dirList = scanDirs(folder)
			for i = 1, #dirList do
				dirList[i] = dirList[i] .. "/"
			end
			fileList = merge(dirList, _fileList)
			fillRect(1, 30, 127, 109, hexTo565(windowColor))
			pathHead(folder)
			drawFiles(folder, 1, 1)
			pressedKey = "-1"
		end
	end
	if pressedKey == "TAB" then
		contextMenuOpen = true
		printContextMenu()
		add_function(0, loop, "contextMenu()")
		pressedKey = "-1"
	end
end

function oncePrint()
	setTextSize(1)
	drawFiles(folder, filePage, count:read())
	pathHead(folder)
end

function setupExplorer()
	setTextSize(1)
	standart_folder = "/proto/user/workSpace/"
	folder = ""
	inputBuffer = ""
	fileList = {}
	copyPath = ""
	cutFlag = false
	count = Counter(13, 1, false)
	context = Counter(8, 1, false)
	context2p = Counter(2, 1, false)
	filePage = 1
	maxPunctInPage = 13
	contextMenuOpen = false
	add_process(
		"fayly olega",
		"/proto/res/fayly olega.png",
		"loopExplorer()",
		"printExplorer()",
		true,
		128,
		140,
		"oncePrint()"
	)
	_fileList = scanFiles(standart_folder, "*", "*")
	dirList = scanDirs(standart_folder)
	print("dirs count: " .. #dirList)
	for i = 1, #dirList do
		print(i, dirList[i])
	end
	for i = 1, #dirList do
		dirList[i] = dirList[i] .. "/"
	end
	fileList = merge(dirList, _fileList)
	folder = standart_folder
	pathHead(folder)
	drawFiles(folder, 1, 1)
	remove_function("setupExplorer()")
end
