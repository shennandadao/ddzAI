#pragma once
#include "GameAI_Config.h"



/*
包含各版本获取出牌信息的策略函数

处理结果主要储存在
clsHandCardData.uctPutCardType      出牌类型
clsHandCardData.value_nPutCardList  出牌序列

*/



/*

get_PutCardList与get_HandCardValue的关系

交替递归调用 get_PutCardList枚举若干种牌型组合并选出此牌型组合与剩余牌的get_HandCardValue值总和最大的策略
get_HandCardValue计算权值时需要通过 get_PutCardList获取当前最优的一种牌型，若可以直接出，则直接出手

层次越深，手牌越少，当手牌满足可以直接全部出去时即为出口


*/


/*
1.0版本策略 主动出牌 出一张最大的牌，被动出牌不出
*/

void get_PutCardList_1(GameSituation&clsGameSituation, HandCardData&clsHandCardData);


/*
2.0版本策略 根据场上形势决定当前预打出的手牌——被动出牌
*/
void get_PutCardList_2_limit(GameSituation &clsGameSituation, HandCardData &clsHandCardData);

/*
2.0版本策略 根据场上形势决定当前预打出的手牌——主动出牌
*/
void get_PutCardList_2_unlimit(GameSituation &clsGameSituation, HandCardData &clsHandCardData);

/*
2.0版本策略 根据场上形势决定当前预打出的手牌——分支处理
*/
void get_PutCardList_2(GameSituation &clsGameSituation, HandCardData &clsHandCardData);

/*
2.0版本策略 重载函数，只用于计算自己手牌需要几个轮次可以出完即其权值情况 
*/
void get_PutCardList_2(HandCardData &clsHandCardData);


/*
3.0版本策略 加入了角色判断，当队友出牌的权值为正值，则不管
*/

/*
4.0版本策略 加入了算牌策略，判断场上现存牌及当前牌型价值情况
*/

/*
5.0版本策略 加入了残局分析，例如：最后两手牌时 天牌先出，敌人仅剩单双牌的情况
*/

/*
6.0版本策略 天人合一 无神无我
*/