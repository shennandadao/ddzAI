
/*牌型组合相关函数*/

#include "GameAI_GroupType.h"
#include "GameAI_Config.h"



/*封装好的获取各类牌型组合结构函数

CardGroupType cgType：牌型
int MaxCard：决定大小的牌值
int Count：牌数

返回值：CardGroupData
*/
CardGroupData get_GroupData(CardGroupType cgType, int MaxCard, int Count)
{
	CardGroupData uctCardGroupData;

	uctCardGroupData.cgType = cgType;
	uctCardGroupData.nCount = Count;
	uctCardGroupData.nMaxCard = MaxCard;

	//不出牌型
	if (cgType == cgZERO)
		uctCardGroupData.nValue = 0;
	//单牌类型
	else if (cgType == cgSINGLE)
		uctCardGroupData.nValue = MaxCard - 10;
	//对牌类型
	else if (cgType == cgDOUBLE)
		uctCardGroupData.nValue = MaxCard - 10;
	//三条类型
	else if (cgType == cgTHREE)
		uctCardGroupData.nValue = MaxCard - 10;
	//单连类型
	else if (cgType == cgSINGLE_LINE)
		uctCardGroupData.nValue = MaxCard - 10 + 1;
	//对连类型
	else if (cgType == cgDOUBLE_LINE)
		uctCardGroupData.nValue = MaxCard - 10 + 1;
	//三连类型
	else if (cgType == cgTHREE_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1)/2;
	//三带一单
	else if (cgType == cgTHREE_TAKE_ONE)
		uctCardGroupData.nValue = MaxCard - 10;
	//三带一对
	else if (cgType == cgTHREE_TAKE_TWO)
		uctCardGroupData.nValue = MaxCard - 10;
	//三带一单连
	else if (cgType == cgTHREE_TAKE_ONE_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//三带一对连
	else if (cgType == cgTHREE_TAKE_TWO_LINE)
		uctCardGroupData.nValue = (MaxCard - 3 + 1) / 2;
	//四带两单
	else if (cgType == cgFOUR_TAKE_ONE)
		uctCardGroupData.nValue = (MaxCard - 3 ) / 2;
	//四带两对
	else if (cgType == cgFOUR_TAKE_TWO)
		uctCardGroupData.nValue = (MaxCard - 3 ) / 2;
	//炸弹类型 
	else if (cgType == cgBOMB_CARD)
		uctCardGroupData.nValue = MaxCard - 3 + 7;
	//王炸类型 
	else if (cgType == cgKING_CARD)
		uctCardGroupData.nValue = 20;
	//错误牌型
	else
		uctCardGroupData.nValue = 0;


	return uctCardGroupData;
}

/*
检查剩余的牌是否只是一手牌

是：  返回手牌类型数据
不是：返回错误类型（cgERROR）
*/
CardGroupData ins_SurCardsType(int arr[])
{

	int nCount = 0;
	for (int i = 3; i < 18; i++)
	{
		nCount += arr[i];
	}

	CardGroupData retCardGroupData;
	retCardGroupData.nCount = nCount;


	//单牌类型
	if (nCount == 1)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		for (int i = 3; i < 18; i++)
		{
			if (arr[i] == 1)
			{
				SumValue = i - 10;
				prov++;
				retCardGroupData.nMaxCard = i;
			}

		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgSINGLE;
			retCardGroupData.nValue= SumValue;
			return retCardGroupData;
		}
	}
	//对牌类型
	if (nCount == 2)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		int i = 0;
		for (i = 3; i < 16; i++)
		{
			if (arr[i] == 2)
			{
				SumValue = i - 10;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgDOUBLE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三条类型
	if (nCount == 3)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		int i = 0;
		for (i = 3; i < 16; i++)
		{
			if (arr[i] == 3)
			{
				SumValue = i - 10;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgTHREE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三带一单
	if (nCount == 4)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 18; i++)
		{
			if (arr[i] == 3)
			{
				SumValue = i - 10;
				prov1++;
				retCardGroupData.nMaxCard = i;

			}
			if (arr[i] == 1)
			{
				prov2++;
			}

		}
		if (prov1 == 1 && prov2 == 1)
		{
			retCardGroupData.cgType = cgTHREE_TAKE_ONE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三带一对
	if (nCount == 5)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 16; i++)
		{
			if (arr[i] == 3)
			{
				SumValue = i - 10;
				prov1++;
				retCardGroupData.nMaxCard = i;

			}
			if (arr[i] == 2)
			{
				prov2++;

			}
		}
		if (prov1 == 1 && prov2 == 1)
		{
			retCardGroupData.cgType = cgTHREE_TAKE_TWO;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//四带两单
	if (nCount == 6)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 18; i++)
		{
			if (arr[i] == 4)
			{
				SumValue = (i - 3) / 2;
				prov1++;
				retCardGroupData.nMaxCard = i;

			}
			if (arr[i] == 1|| arr[i] == 2)
			{
				prov2+= arr[i];
			}
		}

		if (prov1 == 1 && prov2 == 2)
		{
			retCardGroupData.cgType = cgFOUR_TAKE_ONE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//四带两对
	if (nCount == 8)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		for (int i = 3; i < 16; i++)
		{
			if (arr[i] == 4)
			{
				SumValue = (i - 3) / 2;

				prov1++;
				retCardGroupData.nMaxCard = i;
			}
			if (arr[i] == 2|| arr[i] == 4)
			{
				prov2+= arr[i]/2;

			}
		}
		
		if (prov1 == 1 && prov2 == 4)
		{
			retCardGroupData.cgType = cgFOUR_TAKE_TWO;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//炸弹类型
	if (nCount == 4)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		for (int i = 3; i < 16; i++)
		{
			if (arr[i] == 4)
			{
				SumValue += i - 3 + 7;
				prov++;
				retCardGroupData.nMaxCard = i;
			}
		}
		if (prov == 1)
		{
			retCardGroupData.cgType = cgBOMB_CARD;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//王炸类型
	if (nCount == 2)
	{
		int SumValue = 0;
		if (arr[17] > 0 && arr[16] > 0)
		{
			SumValue = 20;
			retCardGroupData.nMaxCard = 17;
			retCardGroupData.cgType = cgKING_CARD;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//单连类型
	if (nCount >= 5)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 1)
			{
				prov++;
			}
			else
			{
				if (prov != 0)
				{
					break;
				}

			}
		}
		SumValue = i - 10;

		if (prov == nCount)
		{
			retCardGroupData.nMaxCard = i-1;
			retCardGroupData.cgType = cgSINGLE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//对连类型
	if (nCount >= 6)
	{
		//用于验证的变量
		int prov = 0;
		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 2)
			{
				prov++;
			}
			else
			{
				if (prov != 0)
				{
					break;
				}

			}
		}
		SumValue = i - 10;

		if (prov * 2 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgDOUBLE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三连类型
	if (nCount >= 6)
	{
		//用于验证的变量
		int prov = 0;

		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 3)
			{
				prov++;
			}
			else
			{
				if (prov != 0)
				{
					break;
				}

			}
		}
		SumValue = (i - 3) / 2;

		if (prov * 3 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgTHREE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}
	//三带一连类型
	if (nCount >= 8)
	{
		//用于验证的变量
		int prov1 = 0;
		int SumValue = 0;
		int i;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] >= 3)
			{
				prov1++;
			}
			else
			{
				if (prov1 != 0)
				{
					break;
				}

			}
		}
		SumValue = (i - 3)/2;
		if (prov1 * 4 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgTHREE_TAKE_ONE_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}

	}
	//三带二连类型
	if (nCount >= 10)
	{
		//用于验证的变量
		int prov1 = 0;
		int prov2 = 0;
		int SumValue = 0;
		int i, j;
		for (i = 3; i < 15; i++)
		{
			if (arr[i] == 3)
			{
				prov1++;
			}
			else
			{
				if (prov1 != 0)
				{
					break;
				}
			}
		}
		for (j = 3; j < 16; j++)
		{
			if (arr[j] == 2|| arr[j] == 4)
			{
				prov2+= arr[j]/2;
			}
		}
		SumValue = (i - 3) / 2;
		if (prov1 == prov2&&prov1 * 5 == nCount)
		{
			retCardGroupData.nMaxCard = i - 1;
			retCardGroupData.cgType = cgTHREE_TAKE_TWO_LINE;
			retCardGroupData.nValue = SumValue;
			return retCardGroupData;
		}
	}

	retCardGroupData.cgType = cgERROR;
	return retCardGroupData;
}

/*
检查剩余的牌是否只是一手牌（vector重载）

是：  返回手牌类型数据
不是：返回错误类型（cgERROR）
*/
CardGroupData ins_SurCardsType(vector<int>list)
{
	int arr[18];
	memset(arr, 0, sizeof(arr));
	for (vector<int>::iterator iter = list.begin(); iter != list.end(); iter++)
	{
		arr[*iter]++;
	}
	return ins_SurCardsType(arr);
}