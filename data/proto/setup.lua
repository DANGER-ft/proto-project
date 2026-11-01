root = {}
--загрузка
---@diagnostic disable: undefined-global
runFile("/proto/taskManager.lua")
print("task manager")
runFile("/proto/kernel.lua")
print("kernel")
runFile("/proto/explorer.lua")
print("files")
runFile("/proto/liber.lua")
print("liber")
runFile("/proto/graphUtils.lua")
print("graph utils")
runFile("/proto/babuinopis.lua")
print("text")
task.add_function(1, "single", "kernel.kernel_loop_setup()")
root.global_while = true
--это батя, его старайтесь не убивать
while root.global_while do
	task.run_task_manager()
end
