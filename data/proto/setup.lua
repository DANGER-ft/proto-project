root = {}
--загрузка
---@diagnostic disable: undefined-global
runFile("/proto/taskManager.lua")
print("/proto/taskManager.lua")
runFile("/proto/kernel.lua")
print("/proto/kernel.lua")
runFile("/proto/explorer.lua")
print("/proto/explorer.lua")
task.add_function(1, "single", "kernel.kernel_loop_setup()")
root.global_while = true
--это батя, его старайтесь не убивать
while root.global_while do
	task.run_task_manager()
end
