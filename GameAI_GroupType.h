#pragma once
#include "GameAI_Config.h"
/*牌型组合相关函数*/



/*封装好的获取各类牌型组合结构函数

CardGroupType cgType：牌型
int MaxCard：决定大小的牌值
int Count：牌数

因为已经是预打出的牌了，所以计算的Value不再考虑拆分的价值。

返回值：CardGroupData
*/
CardGroupData get_GroupData(CardGroupType cgType, int MaxCard, int Count);


/*
检查剩余的牌是否只是一手牌

是：  返回手牌类型数据
不是：返回错误类型（cgERROR）
*/
CardGroupData ins_SurCardsType(int arr[]);
CardGroupData ins_SurCardsType(vector<int>list);

