graph = {}
local inputLabel = ""
local color = ""
local btnColor = ""
local text = ""
local textColor = ""
--отсюда текст получать
graph.exitText = ""
--вывел в отдельныю, зач не знаю
local function getInput()
	return text:sub(-12) .. "_"
end
--маин процесс для обработки ввода
function graph.inputTick()
	if #text == 0 then
		drawString(inputLabel, 26, 57)
	else
		drawString(getInput(), 26, 57)
	end
	if kernel.pressedKey == "ESC" then
		print("exit")
		kernel.pressedKey = "-1"
		graph.exitText = ""
		task.remove_function("graph.inputTick()")
	end
	if kernel.pressedKey == "ENTER" then
		print("return")
		kernel.pressedKey = "-1"
		graph.exitText = text
		task.remove_function("graph.inputTick()")
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
		fillRect(25, 56, 78, 9, hexTo565(color))
		kernel.pressedKey = "-1"
	end
end
--инициализация
function graph.input(label, col, btnCol, textCol)
	print("input activate")
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
	
	if btnCol then
		btnColor = btnCol
	else
		btnColor = "#38444f"

	if textCol then
		textColor = = textCol
	else
		textColor = = "#0e1013"
	end

	text = ""
	--setTextColor(hexTo565(textColor))
	fillRect(14, 50, 100, 40, hexTo565(color))
	drawRect(14, 50, 100, 40, hexTo565(kernel.windowOutlineColor))
	drawRect(24, 55, 80, 12, hexTo565(kernel.windowOutlineColor))
	drawString(inputLabel, 26, 57)
	fillRect(24, 75, 30, 11, hexTo565(btnColor))
	fillRect(73, 75, 30, 11, hexTo565(btnColor))
	drawRect(24, 75, 30, 11, hexTo565(kernel.windowOutlineColor))
	drawRect(73, 75, 30, 11, hexTo565(kernel.windowOutlineColor))
	drawString("ESC", 31, 77)
	drawString("ENT", 80, 77)
	--Теперь лучше
	task.add_function(2, "loop", "graph.inputTick()")
end
