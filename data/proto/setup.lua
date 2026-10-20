--загрузка
---@diagnostic disable: undefined-global
runFile("/proto/taskManager.lua")
print("/proto/taskManager.lua")
runFile("/proto/kernel.lua")
print("/proto/kernel.lua")
runFile("/proto/explorer.lua")
print("/proto/explorer.lua")
add_function(1, "single", "kernel_loop_setup()")
global_while = true
--это батя, его старайтесь не убивать
while global_while do
	run_task_manager()
end
