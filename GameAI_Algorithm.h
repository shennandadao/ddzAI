#pragma once
#include "GameAI_Config.h"








//叫分逻辑函数
int LandScore(GameSituation &clsGameSituation, HandCardData &clsHandCardData);


/*
根据场上形势决定当前预打出手牌的档次
*/
int get_CardsGrade(GameSituation &clsGameSituation, HandCardData &clsHandCardData);



/*
通过回溯dp的方式获取手牌价值
与get_PutCardList作为交替递归调用
返回：价值结构体HandCardValue
权值的计算规则参考头文件评分逻辑思维
*/

HandCardValue get_HandCardValue(HandCardData &clsHandCardData);




/*评分逻辑思维：

0.由于新策略引入手牌轮次参数，所以不再考虑拆分价值。
1.牌力基础价值：我们认为10属于中等位置，即<10单牌价值为负，大于10的单牌价值为正
2.控手的价值定义：我们认为一次控手权可以抵消一次手中最小牌型，最小牌型（3）的价值为-7，即我们定义一次控手权的价值为7
3.单牌的价值定义：该牌的基础价值
4.对牌的价值定义：我们认为对牌与单牌价值相等（均可以被三牌带出）故其价值为该牌的基础价值
5.三牌的价值定义：
  三不带：     该牌的基础价值
  三带一：     我们认为通常带出去的牌价值一定无正价值故其价值为该牌的基础价值
  三带二：     我们认为通常带出去的牌价值一定无正价值故其价值为该牌的基础价值
6.四牌的价值定义：
  炸弹：       我们认为炸弹会享有一次控手权利且炸弹被管的概率极小，故其无负价值，非负基础价值+7
  四带二单：   我们认为四带二单管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)
  四带二对：   我们认为四带二对管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)
7.王炸的价值定义：已知炸2价值为15-3+7=19分，故王炸价值为20分。
8.顺子的价值定义：
  单顺：       我们认为单顺的价值等于其最大牌的单体价值，且2不参与顺子，故顺子的权值依次提升1
  双顺：       我们认为双顺的价值等于其最大牌的单体价值，且2不参与顺子，故顺子的权值依次提升1
  飞机不带：   由于飞机牌型管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)
  飞机带双翅： 由于飞机牌型管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)
  飞机带单翅： 由于飞机牌型管人与被管的概率极小，故其无负价值，其价值为非负基础价值/2（该值最大为6小于一个轮次7)
9.根据数值分布，我们暂定：   <10不叫分，10-14叫一分，15-19叫两分，20以上叫三分

PS.以上逻辑纯属扯淡，谁信谁SB。。。。。
*/


/*封装好的  将所有的牌都打出*/
void Put_All_SurCards(GameSituation &clsGameSituation, HandCardData &clsHandCardData, CardGroupData SurCardGroupData);
void Put_All_SurCards(HandCardData &clsHandCardData, CardGroupData SurCardGroupData);


bool HasBoom(int arr[]);
