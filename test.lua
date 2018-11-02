local sam = require "GameAI"


function dump(obj)
    local getIndent, quoteStr, wrapKey, wrapVal, dumpObj
    getIndent = function(level)
        return string.rep("\t", level)
    end
    quoteStr = function(str)
        return '"' .. string.gsub(str, '"', '\\"') .. '"'
    end
    wrapKey = function(val)
        if type(val) == "number" then
            return "[" .. val .. "]"
        elseif type(val) == "string" then
            return "[" .. quoteStr(val) .. "]"
        else
            return "[" .. tostring(val) .. "]"
        end
    end
    wrapVal = function(val, level)
        if type(val) == "table" then
            return dumpObj(val, level)
        elseif type(val) == "number" then
            return val
        elseif type(val) == "string" then
            return quoteStr(val)
        else
            return tostring(val)
        end
    end
    dumpObj = function(obj, level)
        if type(obj) ~= "table" then
            return wrapVal(obj)
        end
        level = level + 1
        local tokens = {}
        tokens[#tokens + 1] = "\n{"
        for k, v in pairs(obj) do
            tokens[#tokens + 1] = getIndent(level) .. wrapKey(k) .. " = " .. wrapVal(v, level) .. ","
        end
        tokens[#tokens + 1] = getIndent(level - 1) .. "}"
        return table.concat(tokens, "\n")
    end
    return dumpObj(obj, 0)
end



--   "nOperation" : 1，                      #操作类型：0——叫分 1——出牌     
--   "nRank":2,                              #AI等级，当前支持参数1和2      
--   "nCardDroit" : 1,                       #当前控手的玩家座位id （若与nOwnIndex相等视为主动出牌）
--   "nNowLandScore" : 0,                    #当前叫地主阶段叫的分值（调用叫地主操作返回数据）
--   "NowCardsList" : [ 56, 52 ],            #当前打出的牌数组 
--   "nOwnIndex" : 2,                        #机器人自己的座位id   （若与nCardDroit相等视为主动出牌）
--   "HandCardList" : [ 20, 6, 5, 4 ],       #玩家手牌数组
--   "PutCardList" : []                      #玩家打出的牌数组（调用出牌操作返回数据）
--押牌
str = "{\"nOwnIndex\": 3, \"HandCardList\":[20,4,5,6,16,52,56],\"NowCardsList\" : [0,1,2,3], \"PutCardList\" : [], \"nNowLandScore\" : 0, \"nRank\" : 2, \"nCardDroit\" : 2, \"nOperation\" : 0}"
print(str)
local retString = Interface(str)
print(retString)

--字符串以某个字符分割
function string_split(str, split)
    local lcSubStrTab = {}
    while true do
        local lcPos = string.find(str,split)
        if not lcPos then
            lcSubStrTab[#lcSubStrTab+1] =  str
            break
        end
        local lcSubStr  = string.sub(str,1,lcPos-1)
        lcSubStrTab[#lcSubStrTab+1] = lcSubStr
        str = string.sub(str,lcPos+1,#str)
    end
    return lcSubStrTab
end

--去掉字符串中的某个字符
function string_remove(str, remove)
    local lcSubStrTab = {}
    while true do
        local lcPos = string.find(str,remove)
        if not lcPos then
            lcSubStrTab[#lcSubStrTab+1] =  str
            break
        end
        local lcSubStr  = string.sub(str,1,lcPos-1)
        lcSubStrTab[#lcSubStrTab+1] = lcSubStr
        str = string.sub(str,lcPos+1,#str)
    end
    local lcMergeStr =""
    local lci = 1
    while true do
        if lcSubStrTab[lci] then
            lcMergeStr = lcMergeStr .. lcSubStrTab[lci]
            lci = lci + 1
        else
            break
        end
    end
    return lcMergeStr
end


function strToValue(str)
        local ret = string_split(str, ",\"")

	for k, v in pairs(ret) do
		print(k, "====", v)
	end	

        local arryRet = {}
        local Ret = {}
        for k, v in pairs(ret) do
                v = string_remove(v, "{")
                v = string_remove(v, "}")
        --        v = string_remove(v, "%[")
          --      v = string_remove(v, "%]")
                v = string_remove(v, "\"")
                ret[k] = v
        if (string.find(v, ",") ) then
                arryRet[#arryRet + 1] = v
        else
		
                Ret[#Ret + 1] = v
        end
        end

        --单值
        local value = {}
        for k, v in pairs(Ret) do
                local aa = string_split(v, ":")
                value[aa[1]] = aa[2]
        end

        --表值
        local arr_value = {}
        for k, v in pairs(arryRet) do
                local aa = string_split(v, ":")
                local bb = string_split(aa[2], ",")
                arr_value[aa[1]] = bb
        end


        return value, arr_value
end


local a, v = strToValue(str)
print(a)
local tmp = v["HandCardList"]
print(dump(tmp))

















