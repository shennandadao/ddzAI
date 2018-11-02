#include "GameAI_Config.h"
#include "GameAI_CardsNameMap.h"
#include "GameAI_Algorithm.h"
#include "GameAI_Test.h"
#include "GameAI_Strategy.h"





void  ValueTest()
{

	GameSituation clsGameSituation;
	HandCardData arrHandCardData;


	arrHandCardData.color_nHandCardList.push_back(20);
	arrHandCardData.color_nHandCardList.push_back(21);
	arrHandCardData.color_nHandCardList.push_back(22);
	arrHandCardData.color_nHandCardList.push_back(23);
	
	arrHandCardData.color_nHandCardList.push_back(24);
	arrHandCardData.color_nHandCardList.push_back(25);
	arrHandCardData.color_nHandCardList.push_back(26);
	arrHandCardData.color_nHandCardList.push_back(27);

	arrHandCardData.color_nHandCardList.push_back(28);
	arrHandCardData.color_nHandCardList.push_back(29);
	arrHandCardData.color_nHandCardList.push_back(30);
	arrHandCardData.color_nHandCardList.push_back(31);
	

	arrHandCardData.PrintAll();
	arrHandCardData.Init();
	arrHandCardData.nOwnIndex = clsGameSituation.nCardDroit = 233;
	
	HandCardValue tmpHandCardValue = get_HandCardValue(arrHandCardData);

	cout << tmpHandCardValue.SumValue << endl;
	cout << tmpHandCardValue.NeedRound << endl;
	
}

void  test() {
	
	//ValueTest();
	//return;
	GameSituation clsGameSituation;

	ALLCardsList  uctALLCardsList;

	//发牌
	SendCards(clsGameSituation, uctALLCardsList);

	HandCardData arrHandCardData[3];

	arrHandCardData[0].color_nHandCardList = uctALLCardsList.arrCardsList[0];
	arrHandCardData[1].color_nHandCardList = uctALLCardsList.arrCardsList[1];
	arrHandCardData[2].color_nHandCardList = uctALLCardsList.arrCardsList[2];

	for (int i = 0; i < 3; i++)
	{
		arrHandCardData[i].Init();
		arrHandCardData[i].nOwnIndex = i;
	}

	cout << "0号玩家牌为：" << endl;
	arrHandCardData[0].PrintAll();
	cout << "1号玩家牌为：" << endl;
	arrHandCardData[1].PrintAll();
	cout << "2号玩家牌为：" << endl;
	arrHandCardData[2].PrintAll();

	cout << "底牌为：" << endl;
	cout << get_CardsName(clsGameSituation.DiPai[0]) << ','
		 << get_CardsName(clsGameSituation.DiPai[1]) << ','
		 << get_CardsName(clsGameSituation.DiPai[2]) << endl;

	cout << endl;
	for (int i = 0; i < 3; i++)
	{
		int  tmpLandScore = LandScore(clsGameSituation, arrHandCardData[i]);
		if (tmpLandScore > clsGameSituation.nNowLandScore)
		{
			clsGameSituation.nNowLandScore = tmpLandScore;
			clsGameSituation.nNowDiZhuID = i;
		}
	}

	if (clsGameSituation.nNowDiZhuID == -1)
	{
		cout << "无人叫地主" << endl;
		return;
	}

	cout << clsGameSituation.nNowDiZhuID << "号玩家是地主，叫分为：" << clsGameSituation.nNowLandScore << endl;
	clsGameSituation.nDiZhuID=clsGameSituation.nNowDiZhuID;
	clsGameSituation.nLandScore =clsGameSituation.nNowLandScore;


	//将三张底牌给地主
	arrHandCardData[clsGameSituation.nDiZhuID].color_nHandCardList.push_back(clsGameSituation.DiPai[0]);
	arrHandCardData[clsGameSituation.nDiZhuID].color_nHandCardList.push_back(clsGameSituation.DiPai[1]);
	arrHandCardData[clsGameSituation.nDiZhuID].color_nHandCardList.push_back(clsGameSituation.DiPai[2]);

	//地主手牌刷新
	arrHandCardData[clsGameSituation.nDiZhuID].Init();

	//出牌玩家ID
	int indexID= clsGameSituation.nDiZhuID;

	cout << endl;


	cout << "0号玩家牌为：" << endl;
	arrHandCardData[0].PrintAll();
	cout << "1号玩家牌为：" << endl;
	arrHandCardData[1].PrintAll();
	cout << "2号玩家牌为：" << endl;
	arrHandCardData[2].PrintAll();
	//当前控手玩家先为地主
	clsGameSituation.nCardDroit = indexID;





	while (!clsGameSituation.Over)
	{
		get_PutCardList_2(clsGameSituation, arrHandCardData[indexID]);//获取出牌序列
		arrHandCardData[indexID].PutCards();
		cout << indexID << "号玩家出牌：" << endl;

		for (vector<int>::iterator iter = arrHandCardData[indexID].color_nPutCardList.begin();
		    iter != arrHandCardData[indexID].color_nPutCardList.end(); iter++)
			cout << get_CardsName(*iter) << (iter == arrHandCardData[indexID].color_nPutCardList.end() - 1 ? '\n' : ',');
		cout << endl;

		if (arrHandCardData[indexID].nHandCardCount == 0)
		{
			clsGameSituation.Over = true;

			if (indexID == clsGameSituation.nDiZhuID)
			{
				cout << "地主" << indexID << "号玩家获胜" << endl;
			}
			else
			{
				for (int i = 0; i < 3; i++) {
					if (i != clsGameSituation.nDiZhuID)
					{
						cout << "农民" << i << "号玩家获胜" << endl;
					}
				}
			}
			
		}
        
		if (arrHandCardData[indexID].uctPutCardType.cgType != cgZERO)
		{
			clsGameSituation.nCardDroit = indexID;
			clsGameSituation.uctNowCardGroup = arrHandCardData[indexID].uctPutCardType;
		}

		indexID == 2 ? indexID = 0 : indexID++;

	}






}










//洗牌
void InitCards(vector <int> &Cards)
{
	//先清空Cards
	Cards.clear();

	vector <int> tmpCards;
	int i;

	//大王56，小王52，没有53，54，55号牌
	for (i = 0; i < 53; i++) {
		tmpCards.push_back(i);
	}
	tmpCards.push_back(56);


	//顺序随机打乱
	for (i = tmpCards.size(); i>0; i--) {
		srand(unsigned(time(NULL)));
		// 选中的随机下标
		int index = rand() % i;
		Cards.push_back(tmpCards[index]);
		tmpCards.erase(tmpCards.begin() + index);
	}

}



//洗牌（指定牌型，用于测试）
void InitCards_Appoint(vector <int> &Cards)
{
	//先清空Cards
	 Cards.clear();

	 /***********飞机与炸弹连续拆分逻辑测试**********/
	
	 Cards.push_back(48); Cards.push_back(50); Cards.push_back(49);
	 Cards.push_back(44); Cards.push_back(47); Cards.push_back(35);
	 Cards.push_back(40); Cards.push_back(46); Cards.push_back(34);
	 Cards.push_back(36); Cards.push_back(45); Cards.push_back(33);
	 Cards.push_back(23); Cards.push_back(43); Cards.push_back(31);
     Cards.push_back(22); Cards.push_back(11);	Cards.push_back(30);
	 Cards.push_back(21); Cards.push_back(10); Cards.push_back(29);
	 Cards.push_back(19); Cards.push_back(9); Cards.push_back(27);
	 Cards.push_back(18); Cards.push_back(8); Cards.push_back(26);
	 Cards.push_back(17); Cards.push_back(7); Cards.push_back(25);
     Cards.push_back(15); Cards.push_back(6);	Cards.push_back(20);
	 Cards.push_back(14); Cards.push_back(5); Cards.push_back(16);
	 Cards.push_back(13); Cards.push_back(4); Cards.push_back(12);
	 Cards.push_back(42); Cards.push_back(3); Cards.push_back(37);
	 Cards.push_back(41); Cards.push_back(2); Cards.push_back(32);
	 Cards.push_back(39); Cards.push_back(1); Cards.push_back(28);
	 Cards.push_back(38); Cards.push_back(0); Cards.push_back(24);

	 Cards.push_back(51); Cards.push_back(52); Cards.push_back(56);
	

}


//发牌
void SendCards(GameSituation & clsGameSituation, ALLCardsList &uctALLCardsList)
{
	//洗牌
	vector <int> Cards;
	InitCards(Cards);
	//InitCards_Appoint(Cards);
	int i, j, k;
	j = 0;
	for (k = 0; k < 17; k++) {
		for (i = 0; i < 3; i++,j++)
		{
			uctALLCardsList.arrCardsList[i].push_back(Cards[j]);
		}
	}

	//三张底牌
	clsGameSituation.DiPai[0] = Cards[j];
	clsGameSituation.DiPai[1] = Cards[j+1];
	clsGameSituation.DiPai[2] = Cards[j+2];
		
	return;
}
