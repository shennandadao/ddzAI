
#include "GameAI_Config.h"
#include "GameAI_CardsNameMap.h"
#include <algorithm>


/*配置类函数*/

/*降序排序对比*/
int cmp(int a, int b) { return a > b ? 1 : 0; }


void HandCardData::SortAsList(vector <int> & arr )
{
	sort(arr.begin(), arr.end(), cmp);
	return;
}

void HandCardData::ClearPutCardList()
{
	color_nPutCardList.clear();

	value_nPutCardList.clear();

	uctPutCardType.cgType = cgERROR;
	uctPutCardType.nCount = 0;
	uctPutCardType.nMaxCard = -1;
	uctPutCardType.nValue = 0;

	return;
}


/*

HandCardData::PutCards

vector <int> value_nPutCardList  无花色列表
vector <int>&color_nPutCardList  有花色列表

*/


bool  HandCardData::PutCards()
{
	for (vector<int>::iterator iter = value_nPutCardList.begin(); iter != value_nPutCardList.end(); iter++)
	{
		int color_nCard = -1;
		if (PutOneCard(*iter, color_nCard))
		{
			color_nPutCardList.push_back(color_nCard);
		}
		else
		{
			return false;
		}
	}

	nHandCardCount -= value_nPutCardList.size();
	return true;
}
/*
HandCardData::PutOneCard

int value_nCard    无花色牌
int &color_nCard   有花色牌


*/
bool  HandCardData::PutOneCard(int value_nCard, int &color_nCard)
{
	bool ret = false;



	//value状态数组处理

	value_aHandCardList[value_nCard]--;

	if (value_aHandCardList[value_nCard] < 0)
	{
		return false;
	}


	//value列表数组处理
	for (vector<int>::iterator iter = value_nHandCardList.begin(); iter != value_nHandCardList.end(); iter++)
	{
		if (*iter == value_nCard)
		{
			value_nHandCardList.erase(iter);
			ret = true;
			break;
		}
	}


	// color列表数组处理

	int k = (value_nCard - 3) * 4;      //数值转换

	for (vector<int>::iterator iter = color_nHandCardList.begin(); iter != color_nHandCardList.end(); iter++)
	{

		for (int i = k; i < k + 4; i++) 
		{
			if (*iter == i)
			{
				color_nCard = i;
				color_nHandCardList.erase(iter);
				return ret;
				
			}
		}
	}
	return false;
}



void HandCardData::get_valueHandCardList()
{
	//清零
	value_nHandCardList.clear();
	memset(value_aHandCardList, 0,sizeof(value_aHandCardList));
	
	for (vector<int>::iterator iter = color_nHandCardList.begin(); iter != color_nHandCardList.end(); iter++)
	{
		value_nHandCardList.push_back((*iter / 4)+3);	
		value_aHandCardList[(*iter / 4) + 3]++;
	}

	
}




void HandCardData::Init()
{
	//根据花色手牌获取权值手牌
	get_valueHandCardList();

	//手牌 排序
	SortAsList(color_nHandCardList);
	SortAsList(value_nHandCardList);
	
	//当前手牌个数
	nHandCardCount = value_nHandCardList.size();

}

void HandCardData::PrintAll()
{


	cout << "color_nHandCardList:" << endl;
	for (vector<int>::iterator iter = color_nHandCardList.begin(); iter != color_nHandCardList.end(); iter++)
		cout << get_CardsName(*iter) << (iter == color_nHandCardList.end() - 1 ? '\n' : ',');

	cout << endl;
	/*
	cout << "value_nHandCardList：" << endl;
	for (vector<int>::iterator iter = value_nHandCardList.begin(); iter != value_nHandCardList.end(); iter++)
		cout << *iter << (iter == value_nHandCardList.end() - 1 ? '\n' : ',');

	cout << endl;

	cout << "value_aHandCardList：" << endl;
	for (int i = 0; i < 18; i++)
	{
		cout << value_aHandCardList[i] << (i == 17 ? '\n' : ',');
	}

	cout << endl;


	cout << "nHandCardCount:" << nHandCardCount << endl;

	cout << endl;

	cout << "nGameRole:" << nGameRole << endl;

	cout << endl;
	*/
}
