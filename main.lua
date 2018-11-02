
local luatoc = {
        [0x13] = 0, [0x23] = 1, [0x33] = 2, [0x43] = 3,
        [0x14] = 4, [0x24] = 5, [0x34] = 6, [0x44] = 7,
        [0x15] = 8, [0x25] = 9, [0x35] = 10, [0x45] = 11,
        [0x16] = 12, [0x26] = 13, [0x36] = 14, [0x46] = 15,
        [0x17] = 16, [0x27] = 17, [0x37] = 18, [0x47] = 19,
        [0x18] = 20, [0x28] = 21, [0x38] = 22, [0x48] = 23,
        [0x19] = 24, [0x29] = 25, [0x39] = 26, [0x49] = 27,
        [0x1a] = 28, [0x2a] = 29, [0x3a] = 30, [0x4a] = 31,
        [0x1b] = 32, [0x2b] = 33, [0x3b] = 34, [0x4b] = 35,
        [0x1c] = 36, [0x2c] = 37, [0x3c] = 38, [0x4c] = 39,
        [0x1d] = 40, [0x2d] = 41, [0x3d] = 42, [0x4d] = 43,
        [0x11] = 44, [0x21] = 45, [0x31] = 46, [0x41] = 47,
        [0x12] = 48, [0x22] = 49, [0x32] = 50, [0x42] = 51,
        [0x4e] = 52,[0x4f] = 56,
}

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

	local arryRet = {}
        local Ret = {}
        for k, v in pairs(ret) do
                v = string_remove(v, "{")
                v = string_remove(v, "}")
                --v = string_remove(v, "%[")
                --v = string_remove(v, "%]")
                v = string_remove(v, "\"")
                ret[k] = v
		print("---------------", v)
       		if (string.find(v, "%[") ) then
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
		if string.len(aa[2]) == 2 then
			--TODO 空值		
		else
	--	print("==============",k, "======", aa[2], "===", string.len(aa[2]))
--[[		if string.len(aa[2]) == 2 then
			local value = string_remove(aa[2], "%[")
			 value = string_remove(value, "%]")
			arr_value[aa[1]] --= value
--		else
                		aa[2] = string_remove(aa[2], "%[")
                		aa[2] = string_remove(aa[2], "%]")
                	local bb = string_split(aa[2], ",")
			if bb then
				print("=-=====bb===", #bb)	
                		arr_value[aa[1]] = bb
			else
				print("=-====no bb====")	
				print(aa[2])
				arr_value[aa[1]] = aa[2] 
			end
--		end
		end
        end

	--注意：c转lua数组
        return value, arr_value
end



function valueToStr(nOwnIndex, nNowLandScore, nRank, nCardDroit, nOperation, HandCardList, NowCardsList, PutCardList)
        local str = string.format("{\"nOwnIndex\":%d,\"nNowLandScore\":%d,\"nRank\":%d,\"nCardDroit\":%d,\"nOperation\":%d,", nOwnIndex,nNowLandScore, nRank, nCardDroit, nOperation)

	local arr_str 
	if #HandCardList == 0 then
		arr_str = "\"HandCardList\":[],"
	else
		local tmpstr = "\"HandCardList\":["
		for i=1, #HandCardList do
			if(i == 1) then
				tmpstr = table.concat({tmpstr, luatoc[HandCardList[i]]})
			else
				tmpstr = table.concat({tmpstr, ","})
				tmpstr = table.concat({tmpstr, luatoc[HandCardList[i]]})
			end
		end 
		tmpstr = table.concat({tmpstr, "]"})

		arr_str = tmpstr
		print(arr_str)	
	end

	if #PutCardList == 0 then
		arr_str = table.concat({arr_str, ",\"PutCardList\":[]"})
	else
		local tmpstr = ",\"PutCardList\":["
		for i=1, #PutCardList do
			if(i == 1) then
				tmpstr = table.concat({tmpstr, luatoc[PutCardList[i]]})
			else
				tmpstr = table.concat({tmpstr, ","})
				tmpstr = table.concat({tmpstr, luatoc[PutCardList[i]]})
			end
		end 
		tmpstr = table.concat({tmpstr, "]"})

		arr_str = table.concat({arr_str,tmpstr})
	
	end

	if #NowCardsList == 0 then
		arr_str = table.concat({arr_str,",\"NowCardsList\":[]}"})

	else	
		local tmpstr = ",\"NowCardsList\":["
		for i=1, #NowCardsList do
			if(i == 1) then
				tmpstr = table.concat({tmpstr, luatoc[NowCardsList[i]]})
			else
				tmpstr = table.concat({tmpstr, ","})
				tmpstr = table.concat({tmpstr, luatoc[NowCardsList[i]]})
			end
		end 
		tmpstr = table.concat({tmpstr, "]}"})

		arr_str = table.concat({arr_str,tmpstr})
	

	end

	str =table.concat({str,arr_str})	
	return str
end

local hand_card_list = {[1] = 0x11}
local t_str = valueToStr(1,2,3,4,5,hand_card_list,{},{})
print(t_str)
print("=================")
local t 
local v
t, v = strToValue(t_str)
local a= v["HandCardList"]
for k, v in pairs(a) do
	print(k, "===========", v)
end

