tasks = {}
taskType = {}
taskStatus = {}
deleted = {}
newFunc = 0
function add_to_table(tbl, value)
	tbl[#tbl + 1] = value
end

function remove_from_table(tbl, index)
	local removed = tbl[index]
	for i = index, #tbl - 1 do
		tbl[i] = tbl[i + 1]
	end
	tbl[#tbl] = nil
	return removed
end

function activate(name, position)
	local out = runString(name)
	if taskType[position] == "single" then
		local removedName = tasks[position]
		remove_from_table(tasks, position)
		remove_from_table(taskType, position)
		remove_from_table(taskStatus, position)
		add_to_table(deleted, removedName)
		if #deleted > 10 then
			remove_from_table(deleted, 1)
		end
	end
end

function run_task_manager()
	if newFunc > 0 and taskStatus[newFunc] == 1 then
		activate(tasks[newFunc], newFunc)
		newFunc = 0
		return
	end
	for i = #taskStatus, 1, -1 do
		if taskStatus[i] == 1 then
			activate(tasks[i], i)
		end
	end
	for i = #taskStatus, 1, -1 do
		if taskStatus[i] ~= 1 then
			activate(tasks[i], i)
		end
	end
end

function add_function(privelegion, type, name)
	add_to_table(tasks, name)
	add_to_table(taskType, type)
	add_to_table(taskStatus, privelegion)
	newFunc = #tasks
end

function remove_function(name)
	for i = #tasks, 1, -1 do
		if tasks[i] == name then
			remove_from_table(tasks, i)
			remove_from_table(taskType, i)
			remove_from_table(taskStatus, i)
		end
	end
end
function remove_all()
	for i = 0, #tasks do
		remove_from_table(tasks, i)
		remove_from_table(taskType, i)
		remove_from_table(taskStatus, i)
	end
end
