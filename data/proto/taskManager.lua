task = {}

--да тут вообще 3 независимых массива и 1 рассинхрон и привет синтакс еррор в сериале.
task.tasks = {}
task.taskType = {}
task.taskStatus = {}
task.deleted = {}
task.newFunc = 0

function task.add_to_table(tbl, value)
	tbl[#tbl + 1] = value
end

function task.remove_from_table(tbl, index)
	local removed = tbl[index]
	for i = index, #tbl - 1 do
		tbl[i] = tbl[i + 1]
	end
	tbl[#tbl] = nil
	return removed
end

function task.activate(name, position)
	local out = runString(name)
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

function task.run_task_manager()
	if task.newFunc > 0 and task.taskStatus[task.newFunc] == 1 then
		task.activate(task.tasks[task.newFunc], task.newFunc)
		task.newFunc = 0
		return
	end
	for i = #task.taskStatus, 1, -1 do
		if task.taskStatus[i] == 1 then
			task.activate(task.tasks[i], i)
		end
	end
	for i = #task.taskStatus, 1, -1 do
		if task.taskStatus[i] ~= 1 then
			task.activate(task.tasks[i], i)
		end
	end
end

function task.add_function(privelegion, type, name)
	task.add_to_table(task.tasks, name)
	task.add_to_table(task.taskType, type)
	task.add_to_table(task.taskStatus, privelegion)
	task.newFunc = #task.tasks
end

function task.remove_function(name)
	for i = #task.tasks, 1, -1 do
		if task.tasks[i] == name then
			task.remove_from_table(task.tasks, i)
			task.remove_from_table(task.taskType, i)
			task.remove_from_table(task.taskStatus, i)
		end
	end
end

function task.remove_all()
	for i = 1, #task.tasks, -1 do
		task.remove_from_table(task.tasks, i)
		task.remove_from_table(task.taskType, i)
		task.remove_from_table(task.taskStatus, i)
	end
end
