explorer = {}

--я не буду это коментировать. как нибудь сами.
explorer.formatsTxt =
	{ ".txt", ".csv", ".tsv", ".json", ".jsonl", ".xml", ".html" },
	{ ".md", ".rst", ".yaml", ".toml", ".ini", ".cfg", ".conf", ".log" },
	{ ".sql", ".env", ".properties", ".srt", ".vtt", ".rtf" }
explorer.formatsImg = { ".jpg", ".png", ".gif", ".webp", ".svg", ".ico" }
explorer.formatsCode = { ".js", ".cpp", ".py", ".rb", ".php", ".sh", ".bash", ".bat", ".cmd", ".lua" }

explorer.nullPointTasks = { "make file", "make dir" }
explorer.objectPointTasks = { "make file", "make dir", "rename", "move", "copy", "past", "delete", "cut" }

local function inputAdd(ch)
	if ch == nil then
		return
	end
	if #ch ~= 1 then
		return
	end
	explorer.inputBuffer = explorer.inputBuffer .. ch
end

local function inputRemove()
	if #explorer.inputBuffer > 0 then
		explorer.inputBuffer = explorer.inputBuffer:sub(1, -2)
	end
end

local function inputRead()
	return explorer.inputBuffer
end

function explorer.runInput()
	task.remove_function("explorer.contextMenu()")
	fillRect(14, 50, 100, 20, hexTo565("#556677"))
	drawRect(14, 50, 100, 20, hexTo565(kernel.windowOutlineColor))
	drawRect(24, 55, 80, 10, hexTo565(kernel.windowOutlineColor))
	drawString(inputRead() .. "_", 26, 55)
	kernel.pressedKey = "-1"
	task.add_function(0, "loop", "explorer.input()")
end

function explorer.input()
	if kernel.pressedKey == "ENTER" then
		print("exit")
		kernel.pressedKey = "-1"
		task.remove_function("explorer.input()")
		explorer.confirmEvent()
	end
	if kernel.pressedKey == "BACKSPACE" then
		print("/b")
		inputRemove()
		fillRect(25, 56, 78, 8, hexTo565("#556677"))
		drawString(inputRead() .. "_", 26, 57)
		kernel.pressedKey = "-1"
	end
	if kernel.pressedKey ~= "-1" then
		inputAdd(kernel.pressedKey)
		fillRect(25, 56, 78, 8, hexTo565("#556677"))
		drawString(inputRead() .. "_", 26, 57)
		kernel.pressedKey = "-1"
	end
end

function explorer.mkdir(name)
	createDir(name)
end
function explorer.rmdir(name)
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

function explorer.mkfile(name)
	createFile(name)
end
function explorer.rmfile(name)
	deleteFile(name)
end
function explorer.move(name, to)
	moveFile(name, to)
end
function explorer.rename(name, newName)
	renameFile(name, newName)
end
function explorer.write(name, text)
	writeFile(name, text)
end
function explorer.clear(name)
	clearFile(name)
end
function explorer.size(name)
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

function explorer.pathHead(path)
	fillRect(1, 17, 128, 12, hexTo565(kernel.windowColor))
	drawRect(1, 16, 128, 14, hexTo565("#010203"))
	setTextColor(hexTo565("#010B13"))
	drawString(replacePrefix(path, explorer.standart_folder, "~/"), 5, 20)
end

function explorer.drawFiles(folder, page, p)
	setTextColor(hexTo565("#010B13"))
	for i = 1, 13, 1 do
		local full = explorer.fileList[i + ((page - 1) * 13)]
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
			for it = 1, #explorer.formatsTxt do
				if explorer.formatsTxt[it] == ext then
					fastPngDraw("/proto/res/txt7x7.png", 3, (i * 8) + 25)
					printed = true
					break
				end
			end
		end
		if not printed then
			local ext = "." .. string.match(full, "%.(.+)$")
			for it = 1, #explorer.formatsImg do
				if explorer.formatsImg[it] == ext then
					fastPngDraw("/proto/res/image7x7.png", 3, (i * 8) + 25)
					printed = true
					break
				end
			end
		end
		if not printed then
			local ext = "." .. string.match(full, "%.(.+)$")
			for it = 1, #explorer.formatsCode do
				if explorer.formatsCode[it] == ext then
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

function explorer.printContextMenu()
	if explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)] == nil then
		fillRect(24, 60, 80, 20, hexTo565("#667788"))
		drawRect(24, 60, 80, 20, hexTo565(kernel.windowOutlineColor))
		drawLine(
			101,
			(explorer.context2p:read() * 10) + 50,
			101,
			(explorer.context2p:read() * 10) + 60,
			hexTo565("#FF0800")
		)
		for i = 1, #explorer.nullPointTasks do
			drawLine(24, (i * 10) + 60, 103, (i * 10) + 60, hexTo565(kernel.windowOutlineColor))
			drawString(explorer.nullPointTasks[i], 26, (i * 10) + 52)
		end
	else
		fillRect(24, 30, 80, 80, hexTo565("#667788"))
		drawRect(24, 30, 80, 80, hexTo565(kernel.windowOutlineColor))
		drawLine(
			101,
			(explorer.context:read() * 10) + 20,
			101,
			(explorer.context:read() * 10) + 30,
			hexTo565("#FF0800")
		)
		for i = 1, #explorer.objectPointTasks do
			drawLine(24, (i * 10) + 30, 103, (i * 10) + 30, hexTo565(kernel.windowOutlineColor))
			drawString(explorer.objectPointTasks[i], 26, (i * 10) + 22)
		end
	end
end

function explorer.confirmEvent()
	if explorer.context:read() == 1 or explorer.context2p:read() == 1 then
		explorer.mkfile(explorer.folder .. inputRead())
	end
	if explorer.context:read() == 2 or explorer.context2p:read() == 2 then
		explorer.mkdir(explorer.folder .. inputRead())
	end
	if explorer.context:read() == 3 then
		explorer.rename(explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)], inputRead())
	end
	if explorer.context:read() == 4 then
		explorer.move(explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)], inputRead())
	end
	if explorer.context:read() == 5 then
		explorer.copyPath = explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)]
		explorer.cutFlag = false
	end
	if explorer.context:read() == 6 then
		local name = explorer.copyPath:gsub(".*/", "")
		local parentFolder = explorer.copyPath:match("^(.*/)[^/]*/?$") or ""
		if explorer.folder == parentFolder then
			local ext = name:match("(%.[^%.]+)$") or ""
			local shortName = name:match("^(.-)%.[^%.]+$") or name
			copyFile(explorer.copyPath, explorer.folder .. shortName .. "(copy)" .. ext)
		else
			copyFile(explorer.copyPath, explorer.folder .. name)
		end
		if explorer.cutFlag then
			explorer.rmfile(explorer.copyPath)
			explorer.cutFlag = false
		end
	end
	if explorer.context:read() == 7 then
		if explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)]:sub(-1) == "/" then
			explorer.rmdir(explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)])
		else
			explorer.rmfile(explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)])
		end
	end
	if explorer.context:read() == 8 then
		explorer.copyPath = explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)]
		explorer.cutFlag = true
	end
	fillRect(1, 30, 127, 109, hexTo565(kernel.windowColor))
	explorer.inputBuffer = ""
	explorer.context:clear()
	explorer.context2p:clear()
	explorer._fileList = scanFiles(explorer.folder, "*", "*")
	explorer.dirList = scanDirs(explorer.folder)
	for i = 1, #explorer.dirList do
		explorer.dirList[i] = explorer.dirList[i] .. "/"
	end
	explorer.fileList = merge(explorer.dirList, explorer._fileList)
	explorer.oncePrint()
	explorer.contextMenuOpen = false
	kernel.pressedKey = "-1"
	task.remove_function("explorer.contextMenu()")
	explorer.context:clear()
	explorer.context2p:clear()
end

function explorer.contextMenu()
	--explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)]
	if kernel.pressedKey == "TAB" then
		task.remove_function("explorer.contextMenu()")
		fillRect(1, 30, 127, 109, hexTo565(kernel.windowColor))
		explorer.context:clear()
		explorer.context2p:clear()
		explorer.oncePrint()
		explorer.contextMenuOpen = false
		kernel.pressedKey = "-1"
	end
	if kernel.pressedKey == "UP" then
		if explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)] == nil then
			drawLine(
				101,
				(explorer.context2p:read() * 10) + 50,
				101,
				(explorer.context2p:read() * 10) + 60,
				hexTo565("#667788")
			)
		else
			drawLine(
				101,
				(explorer.context:read() * 10) + 20,
				101,
				(explorer.context:read() * 10) + 30,
				hexTo565("#667788")
			)
		end
		explorer.context:count(true, false)
		explorer.context2p:count(true, false)
		explorer.printContextMenu()
		kernel.pressedKey = "-1"
	end
	if kernel.pressedKey == "DOWN" then
		if explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)] == nil then
			drawLine(
				101,
				(explorer.context2p:read() * 10) + 50,
				101,
				(explorer.context2p:read() * 10) + 60,
				hexTo565("#667788")
			)
		else
			drawLine(
				101,
				(explorer.context:read() * 10) + 20,
				101,
				(explorer.context:read() * 10) + 30,
				hexTo565("#667788")
			)
		end
		explorer.context:count(false, true)
		explorer.context2p:count(true, false)
		explorer.printContextMenu()
		kernel.pressedKey = "-1"
	end
	if kernel.pressedKey == "ENTER" then
		kernel.pressedKey = "-1"
		local hasFile = explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)] ~= nil
		local idx = hasFile and explorer.context:read() or explorer.context2p:read()
		if (hasFile and (idx == 1 or idx == 2 or idx == 3 or idx == 4)) or (not hasFile and (idx == 1 or idx == 2)) then
			explorer.runInput()
			return
		end
		explorer.confirmEvent()
	end
end

function explorer.loopExplorer() end

function explorer.printExplorer()
	explorer.whole = #explorer.fileList // 13
	explorer.remainder = #explorer.fileList % 13
	if explorer.remainder > 0 then
		explorer.maximum = explorer.whole + 1
	else
		explorer.maximum = explorer.whole
	end
	if not explorer.contextMenuOpen then
		if #explorer.fileList > 13 then
			if kernel.pressedKey == "PAGE_UP" and explorer.filePage < explorer.maximum then
				explorer.filePage = explorer.filePage + 1
				fillRect(1, 30, 127, 109, hexTo565(kernel.windowColor))
				explorer.count:clear()
				if explorer.filePage == explorer.whole - 1 then
					explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
					explorer.maxPunctInPage = explorer.remainder
				else
					explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
					explorer.maxPunctInPage = 13
				end
				kernel.pressedKey = "-1"
			end
			if kernel.pressedKey == "PAGE_DOWN" and explorer.filePage > 1 then
				explorer.filePage = explorer.filePage - 1
				fillRect(1, 30, 127, 109, hexTo565(kernel.windowColor))
				explorer.count:clear()
				if explorer.filePage == explorer.whole - 1 then
					explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
					explorer.maxPunctInPage = explorer.remainder
				else
					explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
					explorer.maxPunctInPage = 13
				end
				kernel.pressedKey = "-1"
			end
		end
		if kernel.pressedKey == "UP" then
			drawLine(
				1,
				(explorer.count:read() * 8) + 25,
				1,
				(explorer.count:read() * 8) + 33,
				hexTo565(kernel.windowColor)
			)
			explorer.count:count(kernel.pressedKey == "UP", kernel.pressedKey == "DOWN")
			if explorer.filePage == explorer.whole - 1 then
				if explorer.count:read() > explorer.maxPunctInPage then
					explorer.count:clear()
				end
				explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
			else
				explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
			end
			kernel.pressedKey = "-1"
		end
		if kernel.pressedKey == "DOWN" then
			drawLine(
				1,
				(explorer.count:read() * 8) + 25,
				1,
				(explorer.count:read() * 8) + 33,
				hexTo565(kernel.windowColor)
			)
			explorer.count:count(kernel.pressedKey == "UP", kernel.pressedKey == "DOWN")
			if explorer.count:read() > explorer.maxPunctInPage then
				explorer.count:clear()
			end
			if explorer.filePage == explorer.whole - 1 then
				explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
			else
				explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
			end
			kernel.pressedKey = "-1"
		end
		if explorer.count:read() > explorer.remainder + 2 and explorer.filePage == explorer.maximum then
			drawLine(
				1,
				(explorer.count:read() * 8) + 25,
				1,
				(explorer.count:read() * 8) + 33,
				hexTo565(kernel.windowColor)
			)
			explorer.count:clear()
			explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
		end
		if explorer.count:read() > explorer.remainder + 1 and explorer.filePage == explorer.maximum then
			drawLine(
				1,
				(explorer.count:read() * 8) + 25,
				1,
				(explorer.count:read() * 8) + 33,
				hexTo565(kernel.windowColor)
			)
			explorer.count:count(true, false)
			explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
		end
		if kernel.pressedKey == "ENTER" then
			local file = explorer.fileList[explorer.count:read() + ((explorer.filePage - 1) * 13)]
			if file:sub(-1) == "/" then
				explorer.folder = file
				explorer._fileList = scanFiles(explorer.folder, "*", "*")
				explorer.dirList = scanDirs(explorer.folder)
				explorer.count:clear()
				for i = 1, #explorer.dirList do
					explorer.dirList[i] = explorer.dirList[i] .. "/"
				end
				explorer.fileList = merge(explorer.dirList, explorer._fileList)
				fillRect(1, 30, 127, 109, hexTo565(kernel.windowColor))
				explorer.pathHead(explorer.folder)
				explorer.drawFiles(explorer.folder, 1, 1)
			end
			kernel.pressedKey = "-1"
		end
		if kernel.pressedKey == "BACKSPACE" then
			explorer.count:clear()
			explorer.folder = goUp(explorer.folder)
			explorer._fileList = scanFiles(explorer.folder, "*", "*")
			explorer.dirList = scanDirs(explorer.folder)
			for i = 1, #explorer.dirList do
				explorer.dirList[i] = explorer.dirList[i] .. "/"
			end
			explorer.fileList = merge(explorer.dirList, explorer._fileList)
			fillRect(1, 30, 127, 109, hexTo565(kernel.windowColor))
			explorer.pathHead(explorer.folder)
			explorer.drawFiles(explorer.folder, 1, 1)
			kernel.pressedKey = "-1"
		end
	end
	if kernel.pressedKey == "TAB" then
		explorer.contextMenuOpen = true
		explorer.printContextMenu()
		task.add_function(0, "loop", "explorer.contextMenu()")
		kernel.pressedKey = "-1"
	end
end

function explorer.oncePrint()
	setTextSize(1)
	explorer.drawFiles(explorer.folder, explorer.filePage, explorer.count:read())
	explorer.pathHead(explorer.folder)
end

function explorer.setupExplorer()
	setTextSize(1)
	explorer.standart_folder = "/proto/user/workSpace/"
	explorer.folder = ""
	explorer.inputBuffer = ""
	explorer.fileList = {}
	explorer.copyPath = ""
	explorer.cutFlag = false
	explorer.count = Counter(13, 1, false)
	explorer.context = Counter(8, 1, false)
	explorer.context2p = Counter(2, 1, false)
	explorer.filePage = 1
	explorer.maxPunctInPage = 13
	explorer.contextMenuOpen = false
	kernel.add_process(
		"fayly olega",
		"/proto/res/fayly olega.png",
		"explorer.loopExplorer()",
		"explorer.printExplorer()",
		true,
		128,
		140,
		"explorer.oncePrint()"
	)
	explorer._fileList = scanFiles(explorer.standart_folder, "*", "*")
	explorer.dirList = scanDirs(explorer.standart_folder)
	print("add /")
	for i = 1, #explorer.dirList do
		explorer.dirList[i] = explorer.dirList[i] .. "/"
	end
	print("merge")
	explorer.fileList = merge(explorer.dirList, explorer._fileList)
	print("set folder")
	explorer.folder = explorer.standart_folder
	print("path & draw files")
	explorer.pathHead(explorer.folder)
	explorer.drawFiles(explorer.folder, 1, 1)
	print("exit setup")
	task.remove_function("explorer.setupExplorer()")
end
