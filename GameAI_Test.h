#pragma once



//下发到三名玩家的手牌序列，此数据只用于测试，作为AI时不会获取

struct ALLCardsList
{
	vector <int>  arrCardsList[3];
};



void  test();
//洗牌
void InitCards(vector <int> &Cards);
//洗牌（指定牌型，用于测试）
void InitCards_Appoint(vector <int> &Cards);
//发牌
void SendCards(GameSituation & clsGameSituation, ALLCardsList &uctALLCardsList);


