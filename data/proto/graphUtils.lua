graph = {}
local inputLabel = ""
local color
local text = ""
local function getInput()
return text:sub(-12) .. "_"
end
function graph.input(label, col)
	if label then
		inputLabel = label
	else
		inputLabel = "print..."
	end
		if col then
		color = col
	else
		color = "#556677"
	end
	text = ""
	fillRect(14, 50, 100, 40, hexTo565(color))
	drawRect(14, 50, 100, 40, hexTo565(kernel.windowOutlineColor))
	drawRect(24, 55, 80, 11, hexTo565(kernel.windowOutlineColor))
	drawString(inputLabel, 26, 55)
	local tasks = task.tasks
	local taskType = task.taskType
	local taskStatus = task.taskStatus
	local deleted = task.deleted
	local newFunc = task.newFunc
	task.remove_all()
	task.add_function(1, "loop", "kernel.kernel_loop()")
	task.add_function(1, "loop", "graph.inputTick()")
end
function graph.getInput()
return text
end
function graph.inputTick()
	fillRect(14, 50, 100, 20, hexTo565(color))
	drawRect(14, 50, 100, 20, hexTo565(kernel.windowOutlineColor))
	drawRect(24, 55, 80, 11, hexTo565(kernel.windowOutlineColor))
	if #text == 0 then
	drawString(inputLabel, 26, 55)
	else
	drawString(getInput(), 26, 55)
	end
	if kernel.pressedKey == "ENTER" then
		print("exit")
		task.remove_all()
		task.tasks = tasks
		task.taskType = taskType
		task.taskStatus = taskStatus
		task.deleted = deleted
		task.newFunc = newFunc
		kernel.pressedKey = "-1"
		return
	end
	if kernel.pressedKey == "BACKSPACE" then
		print("/b")
		if #text > 0 then
		text = text:sub(1, -2)
		end
		fillRect(25, 56, 78, 9, hexTo565(color))
		kernel.pressedKey = "-1"
	end
	if kernel.pressedKey ~= "-1" then
		text = text .. kernel.pressedKey
		fillRect(25, 56, 78, 9, hexTo565("#556677"))
		kernel.pressedKey = "-1"
	end
end
