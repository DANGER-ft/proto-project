task = {}

--да тут вообще 3 независимых массива и 1 рассинхрон и привет еррор.
task.tasks = {}
task.taskType = {}
task.taskStatus = {}
task.deleted = {}
task.newFunc = 0
--см. ниже почему так
function task.add_to_table(tbl, value)
	tbl[#tbl + 1] = value
end

--почему то table.remove() не работает, хз почему вроде должен быть
function task.remove_from_table(tbl, index)
	local removed = tbl[index]
	for i = index, #tbl - 1 do
		tbl[i] = tbl[i + 1]
	end
	tbl[#tbl] = nil
	return removed
end
--активация и удаление еденичных
function task.activate(name, position)
	runString(name)
	if task.taskType[position] == "single" then
		local removedName = task.tasks[position]
		task.remove_from_table(task.tasks, position)
		task.remove_from_table(task.taskType, position)
		task.remove_from_table(task.taskStatus, position)
		task.add_to_table(task.deleted, removedName)
		if #task.deleted > 10 then
			task.remove_from_table(task.deleted, 1)
		end
	end
end

--тут приоритетное выполнение, но пометки на английском лень переписывать
function task.run_task_manager()
	--ватчдоги: работают всегда
	for i = #task.taskStatus, 1, -1 do
		if task.taskStatus[i] == 3 then
			task.activate(task.tasks[i], i)
		end
	end
	--монополические(вместо while true)
	for i = #task.taskStatus, 1, -1 do
		if task.taskStatus[i] == 2 then
			task.activate(task.tasks[i], i)
			return
		end
	end
	--обработка новых привилегорованых лошар
	if task.newFunc > 0 and task.taskStatus[task.newFunc] == 1 then
		task.activate(task.tasks[task.newFunc], task.newFunc)
		task.newFunc = 0
	end
	--привелегированые лошары
	for i = #task.taskStatus, 1, -1 do
		if task.taskStatus[i] == 1 then
			task.activate(task.tasks[i], i)
		end
	end
	--базовые лошары
	for i = #task.taskStatus, 1, -1 do
		if task.taskStatus[i] == 0 then
			task.activate(task.tasks[i], i)
		end
	end
end

--добавить задачу (в дальнейшем называю "функция")
function task.add_function(privelegion, type, name)
	task.add_to_table(task.tasks, name)
	task.add_to_table(task.taskType, type)
	task.add_to_table(task.taskStatus, privelegion)
	task.newFunc = #task.tasks
end

--удалить функцию по имени
function task.remove_function(name)
	for i = #task.tasks, 1, -1 do
		if task.tasks[i] == name then
			task.remove_from_table(task.tasks, i)
			task.remove_from_table(task.taskType, i)
			task.remove_from_table(task.taskStatus, i)
		end
	end
end
--реинициализация
function task.remove_all()
	task.tasks = {}
	task.taskType = {}
	task.taskStatus = {}
	task.deleted = {}
	task.newFunc = 0
end
