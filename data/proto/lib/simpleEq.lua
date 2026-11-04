simEq = {}
local function m_abs(x)
	if x < 0 then
		return -x
	else
		return x
	end
end

local function m_floor(x)
	local i = 0
	if x >= 0 then
		while i + 1 <= x do
			i = i + 1
		end
	else
		i = -1
		while i - 1 >= x do
			i = i - 1
		end
	end
	return i
end

local function sub_char(s, i)
	if type(s) ~= "string" then
		return ""
	end
	return s:sub(i, i)
end

local function sub_str(s, a, b)
	if type(s) ~= "string" then
		return ""
	end
	return s:sub(a, b)
end

local function isInt(x)
	return m_abs(x - m_floor(x + 0.5)) < 1e-9
end

local function fmt(n)
	if type(n) ~= "number" then
		return tostring(n)
	end
	if isInt(n) then
		return tostring(m_floor(n + 0.5))
	end
	local neg = n < 0
	n = m_abs(n)
	local scaled = m_floor(n * 10000 + 0.5)
	local intPart = m_floor(scaled / 10000)
	local fracPart = scaled - intPart * 10000
	local fracStr = tostring(fracPart)
	while #fracStr < 4 do
		fracStr = "0" .. fracStr
	end
	while #fracStr > 0 and sub_char(fracStr, #fracStr) == "0" do
		fracStr = sub_str(fracStr, 1, #fracStr - 1)
	end
	local s = tostring(intPart)
	if #fracStr > 0 then
		s = s .. "." .. fracStr
	end
	if neg then
		s = "-" .. s
	end
	return s
end

local function m_sqrt(x)
	if x < 0 then
		return nil
	end
	if x == 0 then
		return 0
	end
	local g = x
	for _ = 1, 50 do
		g = (g + x / g) / 2
	end
	return g
end

local function remove_spaces(s)
	local out = ""
	for i = 1, #s do
		local c = sub_char(s, i)
		if c ~= " " and c ~= "\t" and c ~= "\n" then
			out = out .. c
		end
	end
	return out
end
local function newPoly()
	return {}
end

local function polyAdd(a, b)
	local r = {}
	for k, v in pairs(a) do
		r[k] = (r[k] or 0) + v
	end
	for k, v in pairs(b) do
		r[k] = (r[k] or 0) + v
	end
	return r
end

local function polyMul(a, b)
	local r = {}
	for ka, va in pairs(a) do
		for kb, vb in pairs(b) do
			r[ka + kb] = (r[ka + kb] or 0) + va * vb
		end
	end
	return r
end

local function polyScale(a, k)
	local r = {}
	for deg, v in pairs(a) do
		r[deg] = v * k
	end
	return r
end

local function polyDegree(p)
	local d = -1
	for deg in pairs(p) do
		if deg > d then
			d = deg
		end
	end
	return d
end

local function polyToStr(p)
	local degs = {}
	for d, v in pairs(p) do
		if m_abs(v) > 1e-12 then
			degs[#degs + 1] = d
		end
	end
	for i = 1, #degs do
		for j = i + 1, #degs do
			if degs[j] > degs[i] then
				degs[i], degs[j] = degs[j], degs[i]
			end
		end
	end

	local out = ""
	for idx = 1, #degs do
		local d = degs[idx]
		local v = p[d]
		local sign = "+"
		if v < 0 then
			sign = "-"
			v = -v
		end
		if idx == 1 and sign == "+" then
			sign = ""
		end

		local coefStr = ""
		if v == 1 and d > 0 then
			coefStr = ""
		else
			coefStr = fmt(v)
		end

		local xStr = ""
		if d == 0 then
			xStr = ""
		elseif d == 1 then
			xStr = "x"
		else
			xStr = "x^" .. d
		end

		out = out .. sign .. coefStr .. xStr
	end
	if out == "" then
		return "0"
	end
	return out
end

local function tokenize(s)
	s = remove_spaces(s)
	local tokens = {}
	local i = 1
	while i <= #s do
		local c = sub_char(s, i)
		if c >= "0" and c <= "9" or c == "." then
			local num = ""
			while i <= #s do
				local cc = sub_char(s, i)
				if (cc >= "0" and cc <= "9") or cc == "." then
					num = num .. cc
					i = i + 1
				else
					break
				end
			end
			tokens[#tokens + 1] = { type = "num", value = tonumber(num) }
		elseif c == "x" then
			tokens[#tokens + 1] = { type = "x" }
			i = i + 1
		elseif c == "+" or c == "-" or c == "*" or c == "/" or c == "^" or c == "(" or c == ")" then
			tokens[#tokens + 1] = { type = c }
			i = i + 1
		else
			error("Неизвестный символ: " .. c)
		end
	end
	return tokens
end

local Parser = {}
Parser.__index = Parser

function Parser.new(tokens)
	return setmetatable({ tokens = tokens, pos = 1 }, Parser)
end

function Parser:peek()
	return self.tokens[self.pos]
end
function Parser:next()
	local t = self.tokens[self.pos]
	self.pos = self.pos + 1
	return t
end

function Parser:parseExpr()
	local left = self:parseTerm()
	while true do
		local p = self:peek()
		if not p then
			break
		end
		if p.type ~= "+" and p.type ~= "-" then
			break
		end
		local op = self:next().type
		local right = self:parseTerm()
		if op == "+" then
			left = polyAdd(left, right)
		else
			left = polyAdd(left, polyScale(right, -1))
		end
	end
	return left
end

function Parser:parseTerm()
	local left = self:parseFactor()
	while true do
		local p = self:peek()
		if not p then
			break
		end
		if p.type ~= "*" and p.type ~= "/" then
			break
		end
		local op = self:next().type
		local right = self:parseFactor()
		if op == "*" then
			left = polyMul(left, right)
		else
			local isConst = true
			local k = nil
			for deg, v in pairs(right) do
				if deg ~= 0 then
					isConst = false
					break
				end
				k = v
			end
			if not isConst or k == 0 then
				error("Деление на выражение с x не поддерживается")
			end
			left = polyScale(left, 1 / k)
		end
	end
	return left
end

function Parser:parseFactor()
	local base = self:parsePower()
	while true do
		local p = self:peek()
		if not p then
			break
		end
		if p.type == "num" or p.type == "x" or p.type == "(" then
			local right = self:parsePower()
			base = polyMul(base, right)
		else
			break
		end
	end
	return base
end

function Parser:parsePower()
	local base = self:parseAtom()
	local p = self:peek()
	if p and p.type == "^" then
		self:next()
		local e = self:next()
		if e.type ~= "num" or not isInt(e.value) or e.value < 0 then
			error("Степень должна быть целой неотрицательной")
		end
		local n = m_floor(e.value + 0.5)
		local r = { [0] = 1 }
		for _ = 1, n do
			r = polyMul(r, base)
		end
		return r
	end
	return base
end

function Parser:parseAtom()
	local t = self:peek()
	if not t then
		error("Неожиданный конец")
	end
	if t.type == "num" then
		self:next()
		return { [0] = t.value }
	elseif t.type == "x" then
		self:next()
		return { [1] = 1 }
	elseif t.type == "(" then
		self:next()
		local e = self:parseExpr()
		local p = self:peek()
		if not p or p.type ~= ")" then
			error("Ожидалась )")
		end
		self:next()
		return e
	elseif t.type == "-" then
		self:next()
		return polyScale(self:parseAtom(), -1)
	elseif t.type == "+" then
		self:next()
		return self:parseAtom()
	end
	error("Неожиданный токен: " .. tostring(t.type))
end

local function parseExpr(s)
	local tokens = tokenize(s)
	local p = Parser.new(tokens)
	local r = p:parseExpr()
	if p.pos <= #tokens then
		error("Лишние символы")
	end
	return r
end

local function gcd(a, b)
	a = m_abs(a)
	b = m_abs(b)
	while b > 0 do
		a, b = b, a - m_floor(a / b) * b
	end
	return a
end

local function solveQuadratic(a, b, c)
	print("Квадратное: " .. fmt(a) .. "x^2 + " .. fmt(b) .. "x + " .. fmt(c) .. " = 0")
	if a == 0 then
		print("a = 0 → x = " .. fmt(-c / b))
		return
	end
	local D = b * b - 4 * a * c
	print("Шаг 1. D = b² - 4ac = " .. fmt(b) .. "² - 4·" .. fmt(a) .. "·" .. fmt(c) .. " = " .. fmt(D))
	if D < 0 then
		print("D < 0 → действительных корней нет")
	elseif D == 0 then
		print("D = 0 → x = -b/(2a) = " .. fmt(-b / (2 * a)))
	else
		local sd = m_sqrt(D)
		print("Шаг 2. √D = " .. fmt(sd))
		local x1 = (-b + sd) / (2 * a)
		local x2 = (-b - sd) / (2 * a)
		print("Шаг 3. x1 = " .. fmt(x1))
		print("        x2 = " .. fmt(x2))
		print("Виет: сумма = " .. fmt(x1 + x2) .. ", произведение = " .. fmt(x1 * x2))
		if isInt(x1) and isInt(x2) and D > 0 then
			print("Разложение: " .. fmt(a) .. "(x - " .. fmt(x1) .. ")(x - " .. fmt(x2) .. ")")
		end
	end
	print()
end

local function solveEquation(equation)
	print("=== Уравнение: " .. equation .. " ===")

	local eqPos = nil
	for i = 1, #equation do
		if sub_char(equation, i) == "=" then
			eqPos = i
			break
		end
	end
	if not eqPos then
		error("Нет знака =")
	end
	local left = sub_str(equation, 1, eqPos - 1)
	local right = sub_str(equation, eqPos + 1, #equation)

	local L = parseExpr(left)
	local R = parseExpr(right)
	local diff = polyAdd(L, polyScale(R, -1))
	for k, v in pairs(diff) do
		if m_abs(v) < 1e-12 then
			diff[k] = nil
		end
	end

	local d = polyDegree(diff)
	print("Приведённый вид: " .. polyToStr(diff) .. " = 0")

	if d == 0 then
		print(m_abs(diff[0] or 0) < 1e-12 and "∞ решений" or "нет решений")
	elseif d == 1 then
		local a = diff[1]
		local b = diff[0] or 0
		print("Линейное. x = -b/a = " .. fmt(-b) .. "/" .. fmt(a) .. " = " .. fmt(-b / a))
		print("Ответ: x = " .. fmt(-b / a))
	elseif d == 2 then
		solveQuadratic(diff[2], diff[1] or 0, diff[0] or 0)
	else
		print("Степень > 2 не поддерживается")
	end
	print()
end

local function solveSystem(a1, b1, c1, a2, b2, c2)
	print("=== Система ===")
	print("  " .. fmt(a1) .. "x + " .. fmt(b1) .. "y = " .. fmt(c1))
	print("  " .. fmt(a2) .. "x + " .. fmt(b2) .. "y = " .. fmt(c2))
	local det = a1 * b2 - a2 * b1
	print("Δ = " .. fmt(det))
	if det == 0 then
		print("Нет решений или ∞")
		print()
		return
	end
	local dx = c1 * b2 - c2 * b1
	local dy = a1 * c2 - a2 * c1
	print("Ответ: x = " .. fmt(dx / det) .. ", y = " .. fmt(dy / det))
	print()
end

local function reduceFraction(num, den)
	print("=== Сокращение: " .. num .. "/" .. den .. " ===")
	if den == 0 then
		print("На 0 нельзя")
		print()
		return
	end
	local g = gcd(num, den)
	print("НОД = " .. fmt(g))
	print("Ответ: " .. fmt(num / g) .. "/" .. fmt(den / g))
	print()
end

local function simplify(exprStr)
	print("=== Упростить: " .. exprStr .. " ===")
	local p = parseExpr(exprStr)
	print("Ответ: " .. polyToStr(p))
	print()
end
local function bestOf(a, b)
	if a == nil then
		return b
	end
	if b == nil then
		return a
	end
	if slen(a) <= slen(b) then
		return a
	else
		return b
	end
end

local function formCanonical(p)
	return polyToStr(p)
end

local function formSquare(p)
	if p[2] == nil or p[2] ~= 1 then
		return nil
	end
	local b = p[1]
	local c = p[0]
	if b == nil or c == nil then
		return nil
	end
	local k = b / 2
	if m_abs(k * k - c) > 1e-9 then
		return nil
	end

	local sign = "+"
	local kAbs = k
	if k < 0 then
		sign = "-"
		kAbs = -k
	end

	if kAbs == 0 then
		return "x^2"
	end
	local kStr = (kAbs == 1) and "" or fmt(kAbs)
	return "(x" .. sign .. kStr .. ")^2"
end

local function formDiffSquares(p)
	if p[2] == nil or p[2] ~= 1 then
		return nil
	end
	if p[1] ~= nil and m_abs(p[1]) > 1e-9 then
		return nil
	end
	local c = p[0]
	if c == nil or c >= 0 then
		return nil
	end
	local cAbs = -c
	local root = m_sqrt(cAbs)
	if not isInt(root) then
		return nil
	end
	return "(x-" .. fmt(root) .. ")(x+" .. fmt(root) .. ")"
end

local function formCommonFactor(p)
	local degs = {}
	local g = 0
	for d, v in pairs(p) do
		if m_abs(v) > 1e-12 then
			degs[#degs + 1] = d
			g = gcd(m_floor(m_abs(v) + 0.5), g)
		end
	end
	if g <= 1 then
		return nil
	end

	local inner = {}
	for d, v in pairs(p) do
		inner[d] = v / g
	end
	local innerStr = polyToStr(inner)
	local gStr = fmt(g)
	if #degs == 1 then
		return gStr .. "*" .. innerStr
	end
	return gStr .. "(" .. innerStr .. ")"
end

local function formSquareWithFactor(p)
	if p[2] == nil then
		return nil
	end
	local a = p[2]
	if a == 1 then
		return nil
	end
	local b = (p[1] or 0) / a
	local c = (p[0] or 0) / a
	local k = b / 2
	if m_abs(k * k - c) > 1e-9 then
		return nil
	end
	local sign = "+"
	local kAbs = k
	if k < 0 then
		sign = "-"
		kAbs = -k
	end
	local kStr = (kAbs == 1) and "" or fmt(kAbs)
	local aStr = (a == -1) and "-" or fmt(a)
	return aStr .. "(x" .. sign .. kStr .. ")^2"
end
local function simply(exprStr)
	print("=== Упростить: " .. exprStr .. " ===")

	local p = parseExpr(exprStr)

	for k, v in pairs(p) do
		if m_abs(v) < 1e-12 then
			p[k] = nil
		end
	end

	local candidates = {}

	local canon = formCanonical(p)
	candidates[#candidates + 1] = { name = "приведение подобных", value = canon }

	local sq = formSquare(p)
	if sq then
		candidates[#candidates + 1] = { name = "свёртка в квадрат", value = sq }
	end

	local ds = formDiffSquares(p)
	if ds then
		candidates[#candidates + 1] = { name = "разность квадратов", value = ds }
	end

	local cf = formCommonFactor(p)
	if cf then
		candidates[#candidates + 1] = { name = "вынос общего множителя", value = cf }
	end

	local sqf = formSquareWithFactor(p)
	if sqf then
		candidates[#candidates + 1] = { name = "квадрат с множителем", value = sqf }
	end

	local best = candidates[1]
	for i = 2, #candidates do
		if slen(candidates[i].value) < slen(best.value) then
			best = candidates[i]
		end
	end

	print("Варианты:")
	for i = 1, #candidates do
		local mark = (candidates[i] == best) and "  ← короче" or ""
		print("  " .. candidates[i].value .. "   (" .. candidates[i].name .. ")" .. mark)
	end

	print("Ответ: " .. best.value)
	print()
	return best.value
end

function simEq.eq(equation)
	local eqPos = nil
	for i = 1, #equation do
		if sub_char(equation, i) == "=" then
			eqPos = i
			break
		end
	end
	if not eqPos then
		return false
	end

	local ok, L = pcall(parseExpr, sub_str(equation, 1, eqPos - 1))
	if not ok then
		return false
	end
	local ok2, R = pcall(parseExpr, sub_str(equation, eqPos + 1, #equation))
	if not ok2 then
		return false
	end

	local diff = polyAdd(L, polyScale(R, -1))
	for k, v in pairs(diff) do
		if m_abs(v) < 1e-12 then
			diff[k] = nil
		end
	end

	local d = polyDegree(diff)

	if d == 0 then
		return (m_abs(diff[0] or 0) < 1e-12) and "x = any" or false
	elseif d == 1 then
		local a, b = diff[1], diff[0] or 0
		return "x = " .. fmt(-b / a)
	elseif d == 2 then
		local a, b, c = diff[2], diff[1] or 0, diff[0] or 0
		local D = b * b - 4 * a * c
		if D < 0 then
			return false
		end
		if D == 0 then
			return "x = " .. fmt(-b / (2 * a))
		end
		local sd = m_sqrt(D)
		local x1 = (-b + sd) / (2 * a)
		local x2 = (-b - sd) / (2 * a)
		return "x = " .. fmt(x1) .. ", " .. fmt(x2)
	end
	return false
end
function simEq.simply(exprStr)
	local ok, p = pcall(parseExpr, exprStr)
	if not ok then
		return false
	end
	for k, v in pairs(p) do
		if m_abs(v) < 1e-12 then
			p[k] = nil
		end
	end

	local candidates = { formCanonical(p) }
	local sq = formSquare(p)
	if sq then
		candidates[#candidates + 1] = sq
	end
	local ds = formDiffSquares(p)
	if ds then
		candidates[#candidates + 1] = ds
	end
	local cf = formCommonFactor(p)
	if cf then
		candidates[#candidates + 1] = cf
	end
	local sqf = formSquareWithFactor(p)
	if sqf then
		candidates[#candidates + 1] = sqf
	end

	local best = candidates[1]
	for i = 2, #candidates do
		if #candidates[i] < #best then
			best = candidates[i]
		end
	end
	return best
end
function simEq.system(a1, b1, c1, a2, b2, c2)
	local det = a1 * b2 - a2 * b1
	if det == 0 then
		if a1 * c2 == a2 * c1 and b1 * c2 == b2 * c1 then
			return "x = any, y = any"
		end
		return false
	end
	local dx = c1 * b2 - c2 * b1
	local dy = a1 * c2 - a2 * c1
	return "x = " .. fmt(dx / det) .. ", y = " .. fmt(dy / det)
end

function simEq.frac(num, den)
	if den == 0 then
		return false
	end
	local g = gcd(num, den)
	return fmt(num / g) .. "/" .. fmt(den / g)
end
