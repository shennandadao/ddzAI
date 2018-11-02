
#include "GameAI_Algorithm.h"
#include "GameAI_Strategy.h"
#include "GameAI_GroupType.h"
/*算法类函数*/



/*
获取叫分函数
*/

int LandScore(GameSituation &clsGameSituation, HandCardData &clsHandCardData)
{
	int SumValue = 0;

	clsHandCardData.uctHandCardValue=get_HandCardValue(clsHandCardData);

	SumValue = clsHandCardData.uctHandCardValue.SumValue;
	
	cout << "SumValue is :" << SumValue << ",";

	cout << "NeedRound is :" << clsHandCardData.uctHandCardValue.NeedRound << endl;

	if (SumValue<10) 
	{
		return 0;
	}
	else if (SumValue < 15)
	{
		return 1;
	}
	else if (SumValue < 20)
	{
		return 2;
	}
	else 
	{
		return 3;
	}
}


/*
根据场上形势决定当前预打出手牌的档次
*/
int get_CardsGrade(GameSituation &clsGameSituation, HandCardData &clsHandCardData)
{
	return 0;
}



/*
通过回溯dp的方式获取手牌价值
与get_PutCardList作为交替递归调用
返回：价值结构体HandCardValue
权值的计算规则参考头文件评分逻辑思维
*/

HandCardValue get_HandCardValue(HandCardData &clsHandCardData)
{

	//首先清空出牌队列，因为剪枝时是不调用get_PutCardList的
	clsHandCardData.ClearPutCardList();

	HandCardValue uctHandCardValue;
    //出完牌了，其实这种情况只限于手中剩下四带二且被动出牌的情况，因为四带二剪枝做了特殊处理。
	if (clsHandCardData.nHandCardCount == 0)
	{
		uctHandCardValue.SumValue = 0;
		uctHandCardValue.NeedRound = 0;
		return uctHandCardValue;
	}
	//————以下为剪枝：判断是否可以一手出完牌
	CardGroupData uctCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
	//如果能一次性出去且没有炸弹，因为有炸弹的话权值可能会更大
	if (uctCardGroupData.cgType != cgERROR&& !HasBoom(clsHandCardData.value_aHandCardList))
	{
		uctHandCardValue.SumValue = uctCardGroupData.nValue;
		uctHandCardValue.NeedRound = 1;
		return uctHandCardValue;
	}

	//非剪枝操作，即非一手能出完的牌

	/*只是获取出牌的序列，即clsHandCardData.value_nPutCardList及clsHandCardData.uctPutCardType
	其他成员均无改变，也不会调用出牌函数，get_PutCardList返回最优方案*/
	get_PutCardList_2(clsHandCardData);

	//要保存当前的clsHandCardData.value_nPutCardList及clsHandCardData.uctPutCardType用于回溯
	CardGroupData NowPutCardType = clsHandCardData.uctPutCardType;
	vector<int> NowPutCardList = clsHandCardData.value_nPutCardList;

	if (clsHandCardData.uctPutCardType.cgType == cgERROR)
	{
		cout << "PutCardType ERROR!" << endl;
	}



	//---回溯↓
	for (vector<int>::iterator iter = NowPutCardList.begin();
		iter != NowPutCardList.end(); iter++)
	{
		clsHandCardData.value_aHandCardList[*iter]--;
	}
	clsHandCardData.nHandCardCount -= NowPutCardType.nCount;
	//---回溯↑
	HandCardValue tmp_SurValue = get_HandCardValue(clsHandCardData);//递归剩余牌价值
			
    //---回溯↓	
	for (vector<int>::iterator iter = NowPutCardList.begin();
		iter != NowPutCardList.end(); iter++)
	{
		clsHandCardData.value_aHandCardList[*iter]++;
	}
	clsHandCardData.nHandCardCount += NowPutCardType.nCount;
	//---回溯↑

	uctHandCardValue.SumValue = NowPutCardType.nValue + tmp_SurValue.SumValue;
	uctHandCardValue.NeedRound = tmp_SurValue.NeedRound + 1;
	




	return uctHandCardValue;

}


/*封装好的  将所有的牌都打出*/
void Put_All_SurCards(GameSituation &clsGameSituation, HandCardData &clsHandCardData, CardGroupData SurCardGroupData)
{
	/*全部出完*/
	for (int i = 0; i < 18; i++)
		for (int j = 0; j< clsHandCardData.value_aHandCardList[i]; j++)
			clsHandCardData.value_nPutCardList.push_back(i);
	/**********/
	clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = SurCardGroupData;
	return;
}

void Put_All_SurCards( HandCardData &clsHandCardData, CardGroupData SurCardGroupData)
{
	/*全部出完*/
	for (int i = 0; i < 18; i++)
		for (int j = 0; j< clsHandCardData.value_aHandCardList[i]; j++)
			clsHandCardData.value_nPutCardList.push_back(i);
	/**********/
	clsHandCardData.uctPutCardType  = SurCardGroupData;
	return;
}


/*判断是否有炸弹*/
bool HasBoom(int arr[])
{
	for (int i = 3; i < 18; i++)
	{
		if (arr[i] >= 4)
		{
			return true;
		}
	}
	return false;
}
