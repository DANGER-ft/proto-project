#pragma once
#include <LuaWrapper.h>
#pragma once
#include <LuaWrapper.h>
#include <LuaBridge/LuaBridge.h> 
using namespace luabridge;

class counter
{
public:

    counter(int maxCount, int minCount, bool invert)
    {
        maxCounterScore = maxCount;
        minCounterScore = minCount;
        invertation = invert;
        score = minCount;
    }


    int read() { return score; };

 
    int count(bool triggerPlus, bool triggerMinus)
    {
        if (triggerPlus)
            score += invertation ? 1 : -1;
        if (triggerMinus)
            score += invertation ? -1 : 1;
        if (score > maxCounterScore)
            score = minCounterScore;
        if (score < minCounterScore)
            score = maxCounterScore;
        return score;
    };


    void clear() { score = minCounterScore; }

private:
    int maxCounterScore = 0;
    int minCounterScore = 0;
    bool invertation = false;
    int score = 0;
};


void LuaCounter(LuaWrapper &lua)
{
    getGlobalNamespace(lua.lua_state)
        .beginClass<counter>("Counter")
        .addConstructor<void (*)(int, int, bool)>() 
        .addFunction("read", &counter::read)        
        .addFunction("count", &counter::count)      
        .addFunction("clear", &counter::clear)    
        .endClass();
}

class Timer
{
private:
    unsigned long startTime = 0;

public:

    Timer()
    {
        startTime = millis();
    }


    bool isReady(unsigned long time)
    {
        if (millis() - startTime >= time)
        {
            return true;
        }
        return false;
    }


    void reset()
    {
        startTime = millis();
    }
};

void LuaTimer(LuaWrapper &lua)
{
    getGlobalNamespace(lua.lua_state)
        .beginClass<Timer>("Timer")
        .addConstructor<void (*)(void)>()
        .addFunction("isReady", &Timer::isReady)
        .addFunction("reset", &Timer::reset)
        .endClass();
}

static int delayC(lua_State *lua_state)
{
    int time = luaL_checknumber(lua_state, 1);
    delay(time);
    return 0;
}
void makeDelay(LuaWrapper &lua){
    lua.Lua_register("delay", (const lua_CFunction)&delayC);
}

