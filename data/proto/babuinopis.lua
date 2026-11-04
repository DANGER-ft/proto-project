txt = {}
local function toolsPanel() end
function txt.loop() end
function txt.print() end
function txt.printOpen() end

function txt.setup()
	kernel.add_process("babuinopis", "null", "txt.loop()", "txt.print()", true, 128, 140, "txt.printOpen()")
	graph.input("ahh") --<- вот это так вызывать

	task.remove_function("txt.setup()")
end
