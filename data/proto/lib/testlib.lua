-- /proto/lib/testlib.lua
lib = {}
lib.name = "testlib"
lib.version = "0.1.0"

function foo(x)
	return x * 2
end

function bar()
	return "hello from testlib"
end

return lib
