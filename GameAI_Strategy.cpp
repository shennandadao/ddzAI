
#include "GameAI_Config.h"
#include "GameAI_Strategy.h"
#include "GameAI_Algorithm.h"
#include "GameAI_GroupType.h"
/*策略类函数*/


/*1.0版本策略 主动出牌 出一张最大的牌，被动出牌不出 */
void get_PutCardList_1(GameSituation&clsGameSituation, HandCardData&clsHandCardData)
{
	clsHandCardData.ClearPutCardList();
	if (clsGameSituation.nCardDroit == clsHandCardData.nOwnIndex)
	{
		
		clsHandCardData.value_nPutCardList.push_back(clsHandCardData.value_nHandCardList[0]);

		clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgSINGLE, clsHandCardData.value_nHandCardList[0], 1);
		
	}	
	else
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
	}
	return;
}



/*
2.0版本策略 重载函数，只用于计算自己手牌需要几个轮次可以出完即其权值情况 
*/
void get_PutCardList_2(HandCardData &clsHandCardData)
{
	
	clsHandCardData.ClearPutCardList();

	//剪枝：如果能出去最后一手牌直接出
	CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
	//如果能一次性出去且没有炸弹，因为有炸弹的话权值可能会更大
	if (SurCardGroupData.cgType != cgERROR&&!HasBoom(clsHandCardData.value_aHandCardList))
	{
			Put_All_SurCards(clsHandCardData, SurCardGroupData);
			return;
	}

	/*王炸——当前策略只处理王炸作为倒数第二手的优先出牌逻辑，后续版本会在此基础上优化*/
	if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
	{

		clsHandCardData.value_aHandCardList[17] --;
		clsHandCardData.value_aHandCardList[16] --;
		clsHandCardData.nHandCardCount -= 2;
		HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
		clsHandCardData.value_aHandCardList[16] ++;
		clsHandCardData.value_aHandCardList[17] ++;
		clsHandCardData.nHandCardCount += 2;
		if (tmpHandCardValue.NeedRound == 1)
		{
			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);
			clsHandCardData.uctPutCardType = get_GroupData(cgKING_CARD, 17, 2);
			return;
		}
	}

	//暂存最佳的价值
	HandCardValue BestHandCardValue;
	BestHandCardValue.NeedRound = 20;
	BestHandCardValue.SumValue = MinCardsValue;
	//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
	BestHandCardValue.NeedRound += 1;

	//暂存最佳的组合
	CardGroupData BestCardGroup;

	//带出去的牌
	int tmp_1 = 0;
	int tmp_2 = 0;
	int tmp_3 = 0;
	int tmp_4 = 0;

	/*
	优先处理三牌、飞机等牌，因为这种牌型可以把小牌带出,其实这里对比较大的三带一不是很公平
	后续版本可以在此处做分支处理，比如说三带一的话只循环到10，J以上先不着急打出。
	*/
	for (int i = 3; i < 16; i++)
	{
		//2.0版本策略主动出牌不拆分炸弹，朕自己从来就不打四带二，因为浪
		if (clsHandCardData.value_aHandCardList[i] != 4)
		{
			//出三带一
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				clsHandCardData.value_aHandCardList[i] -= 3;
				for (int j = 3; j < 18; j++)
				{
					if (clsHandCardData.value_aHandCardList[j] > 0 )
					{
						clsHandCardData.value_aHandCardList[j] -= 1;
						clsHandCardData.nHandCardCount -= 4;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[j] += 1;
						clsHandCardData.nHandCardCount += 4;
						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgTHREE_TAKE_ONE, i, 4);
							tmp_1 = j;
						}
					}
				}
				clsHandCardData.value_aHandCardList[i] += 3;
			}
			//出三带二
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				for (int j = 3; j < 16; j++)
				{
					clsHandCardData.value_aHandCardList[i] -= 3;
					if (clsHandCardData.value_aHandCardList[j] > 1)
					{
						clsHandCardData.value_aHandCardList[j] -= 2;
						clsHandCardData.nHandCardCount -= 5;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[j] += 2;
						clsHandCardData.nHandCardCount += 5;
						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgTHREE_TAKE_TWO, i, 5);
							tmp_1 = j;
						}
					}
					clsHandCardData.value_aHandCardList[i] += 3;
				}
			}
			//出四带二单
			if (clsHandCardData.value_aHandCardList[i] > 3)
			{
				//2.0版本策略主动出牌不拆分炸弹，朕自己从来就不打四带二，因为浪
			}
			//出四带二对
			if (clsHandCardData.value_aHandCardList[i] > 3)
			{
				//2.0版本策略主动出牌不拆分炸弹，朕自己从来就不打四带二，因为浪
			}
			//出三带一单连
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if (clsHandCardData.value_aHandCardList[j] > 2)
					{
						prov++;
					}
					else
					{
						break;
					}
					/*本来想做全排列选取带出的牌然后枚举出最高价值的，但考虑到当飞机长度也就是在2-4之间
					所以干脆做三个分支处理算了*/
					//为两连飞机
					if (prov == 2)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 4;
						for (int tmp1 = 3; tmp1 < 18; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 0 )
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 1;
								for (int tmp2 = tmp1; tmp2 < 18; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 0 )
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 1;
										HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
									
										if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
										{
											BestHandCardValue = tmpHandCardValue;
											BestCardGroup = get_GroupData(cgTHREE_TAKE_ONE_LINE, j, prov * 4);
											tmp_1 = tmp1;
											tmp_2 = tmp2;

										}
										clsHandCardData.value_aHandCardList[tmp2] += 1;
									}
								}
								clsHandCardData.value_aHandCardList[tmp1] += 1;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 4;
					}
					//为三连飞机
					if (prov == 3)
					{
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 4;
						for (int tmp1 = 3; tmp1 < 18; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 0 )
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 1;
								for (int tmp2 = tmp1; tmp2 < 18; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 0 )
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 1;
										for (int tmp3 = tmp2; tmp3 < 18; tmp3++)
										{
											if (clsHandCardData.value_aHandCardList[tmp3] > 0 )
											{
												clsHandCardData.value_aHandCardList[tmp3] -= 1;

												HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
												if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
												{

													BestHandCardValue = tmpHandCardValue;
													BestCardGroup = get_GroupData(cgTHREE_TAKE_ONE_LINE, j, prov * 4);
													tmp_1 = tmp1;
													tmp_2 = tmp2;
													tmp_3 = tmp3;

												}
												clsHandCardData.value_aHandCardList[tmp3] += 1;
											}

										}
										clsHandCardData.value_aHandCardList[tmp2] += 1;
									}

								}
								clsHandCardData.value_aHandCardList[tmp1] += 1;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 4;
					}
					//为四连飞机
					if (prov == 4)
					{
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 4;
						for (int tmp1 = 3; tmp1 < 18; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 0 )
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 1;
								for (int tmp2 = tmp1; tmp2 < 18; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 0 )
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 1;
										for (int tmp3 = tmp2; tmp3 < 18; tmp3++)
										{
											if (clsHandCardData.value_aHandCardList[tmp3] > 0 )
											{
												clsHandCardData.value_aHandCardList[tmp3] -= 1;
												for (int tmp4 = tmp3; tmp4 < 18; tmp4++)
												{
													if (clsHandCardData.value_aHandCardList[tmp4] > 0 )
													{
														clsHandCardData.value_aHandCardList[tmp4] -= 1;
														HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
														if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
														{
															BestHandCardValue = tmpHandCardValue;
															BestCardGroup = get_GroupData(cgTHREE_TAKE_ONE_LINE, j, prov * 4);
															tmp_1 = tmp1;
															tmp_2 = tmp2;
															tmp_3 = tmp3;
															tmp_4 = tmp4;
														}
														clsHandCardData.value_aHandCardList[tmp4] += 1;
													}

												}
												clsHandCardData.value_aHandCardList[tmp3] += 1;
											}

										}
										clsHandCardData.value_aHandCardList[tmp2] += 1;
									}

								}
								clsHandCardData.value_aHandCardList[tmp1] += 1;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 4;
					}
					//若prov==5，则是地主可以直接出去，在剪枝部分已经处理
				}

			}
			//出三带一双连
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if (clsHandCardData.value_aHandCardList[j] > 2 )
					{
						prov++;
					}
					else
					{
						break;
					}
					/*本来想做全排列选取带出的牌然后枚举出最高价值的，但考虑到当飞机长度也就是在2-4之间
					所以干脆做三个分支处理算了*/
					//为两连飞机
					if (prov == 2)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 5;
						for (int tmp1 = 3; tmp1 < 16; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 1 )
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 2;
								for (int tmp2 = tmp1; tmp2 < 16; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 1 )
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 2;
										HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
										if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
										{
											BestHandCardValue = tmpHandCardValue;
											BestCardGroup = get_GroupData(cgTHREE_TAKE_TWO_LINE, j, prov * 5);
											tmp_1 = tmp1;
											tmp_2 = tmp2;
										}
										clsHandCardData.value_aHandCardList[tmp2] += 2;
									}
								}
								clsHandCardData.value_aHandCardList[tmp1] += 2;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 5;
					}
					//为三连飞机
					if (prov == 3)
					{
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 5;
						for (int tmp1 = 3; tmp1 < 16; tmp1++)
						{
							if (clsHandCardData.value_aHandCardList[tmp1] > 1 )
							{
								clsHandCardData.value_aHandCardList[tmp1] -= 2;
								for (int tmp2 = tmp1; tmp2 < 16; tmp2++)
								{
									if (clsHandCardData.value_aHandCardList[tmp2] > 1 )
									{
										clsHandCardData.value_aHandCardList[tmp2] -= 2;
										for (int tmp3 = tmp2; tmp3 < 16; tmp3++)
										{
											if (clsHandCardData.value_aHandCardList[tmp3] > 1 )
											{
												clsHandCardData.value_aHandCardList[tmp3] -= 2;
												HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
												if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
												{
													BestHandCardValue = tmpHandCardValue;
													BestCardGroup = get_GroupData(cgTHREE_TAKE_TWO_LINE, j, prov * 5);
													tmp_1 = tmp1;
													tmp_2 = tmp2;
													tmp_3 = tmp3;
												}
												clsHandCardData.value_aHandCardList[tmp3] += 2;
											}

										}
										clsHandCardData.value_aHandCardList[tmp2] += 2;
									}

								}
								clsHandCardData.value_aHandCardList[tmp1] += 2;
							}
						}
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 5;
					}
					//若prov==4，则是地主可以直接出去，在剪枝部分已经处理
				}
			}
		}

	}
	if (BestCardGroup.cgType == cgTHREE_TAKE_ONE)
		{
			clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.uctPutCardType = BestCardGroup;
			return;
		}
	else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO)
		{
			clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.uctPutCardType = BestCardGroup;
			return;
		}
	else if (BestCardGroup.cgType == cgTHREE_TAKE_ONE_LINE)
		{
			for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 4) + 1; j <= BestCardGroup.nMaxCard; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}

			if (BestCardGroup.nCount / 4 == 2)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
			}
			if (BestCardGroup.nCount / 4 == 3)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
			}
			if (BestCardGroup.nCount / 4 == 4)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
				clsHandCardData.value_nPutCardList.push_back(tmp_4);
			}

			clsHandCardData.uctPutCardType = BestCardGroup;
			return;
		}
	else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO_LINE)
		{
			for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 5) + 1; j <= BestCardGroup.nMaxCard; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}
			if (BestCardGroup.nCount / 5 == 2)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
			}
			if (BestCardGroup.nCount / 5 == 3)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
			}
			clsHandCardData.uctPutCardType = BestCardGroup;
			return;
		}
		

	//次之处理当前价值最低的牌，现在不必再考虑这张牌可能被三牌带出等情况
	for (int i = 3; i < 16; i++) 
	{
		if (clsHandCardData.value_aHandCardList[i] != 0&& clsHandCardData.value_aHandCardList[i] != 4)
		{
			//出单牌
			if (clsHandCardData.value_aHandCardList[i] > 0)
			{
				clsHandCardData.value_aHandCardList[i]--;
				clsHandCardData.nHandCardCount--;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i]++;
				clsHandCardData.nHandCardCount++;
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestCardGroup= get_GroupData(cgSINGLE, i, 1);
				}
			}
			//出对牌
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				//尝试打出一对牌，估算剩余手牌价值
				clsHandCardData.value_aHandCardList[i] -= 2;
				clsHandCardData.nHandCardCount -= 2;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 2;
				clsHandCardData.nHandCardCount += 2;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestCardGroup = get_GroupData(cgDOUBLE, i, 2);
				}
			}
			//出三牌
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				clsHandCardData.value_aHandCardList[i] -= 3;
				clsHandCardData.nHandCardCount -= 3;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 3;
				clsHandCardData.nHandCardCount += 3;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestCardGroup = get_GroupData(cgTHREE, i, 3);
				}
			}
			//出单顺
			if (clsHandCardData.value_aHandCardList[i] > 0)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if(clsHandCardData.value_aHandCardList[j]>0)
					{
						prov++;
					}
					else
					{
						break;
					}
					if (prov >= 5)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] --;
						}
						clsHandCardData.nHandCardCount -= prov;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] ++;
						}
						clsHandCardData.nHandCardCount += prov;

						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgSINGLE_LINE, j, prov);
						}
					}
				}
				
			}
			//出双顺
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if (clsHandCardData.value_aHandCardList[j]>1)
					{
						prov++;
					}
					else
					{
						break;
					}
					if (prov >= 3)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -=2;
						}
						clsHandCardData.nHandCardCount -= prov*2;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] +=2;
						}
						clsHandCardData.nHandCardCount += prov*2;

						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgDOUBLE_LINE, j, prov*2);
						}
					}
				}
			}
			//出三顺
			if(clsHandCardData.value_aHandCardList[i] > 2)
			{
				int prov = 0;
				for (int j = i; j < 15; j++)
				{
					if (clsHandCardData.value_aHandCardList[j]>2)
					{
						prov++;
					}
					else
					{
						break;
					}
					if (prov >= 2)
					{

						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] -= 3;
						}
						clsHandCardData.nHandCardCount -= prov * 3;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						for (int k = i; k <= j; k++)
						{
							clsHandCardData.value_aHandCardList[k] += 3;
						}
						clsHandCardData.nHandCardCount += prov * 3;

						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestCardGroup = get_GroupData(cgTHREE_LINE, j, prov * 3);
						}
					}
				}
			}

			//放在if内是因为若此时i有值那么必须要返回一个结果

			if (BestCardGroup.cgType == cgERROR)
			{

			}
			else if (BestCardGroup.cgType == cgSINGLE)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgDOUBLE)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgSINGLE_LINE)
			{
				for (int j = BestCardGroup.nMaxCard- BestCardGroup.nCount+1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
				}
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgDOUBLE_LINE)
			{
				for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount/2) + 1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
				}
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_LINE)
			{
				for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 3) + 1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
				}
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_TAKE_ONE)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO)
			{
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(BestCardGroup.nMaxCard);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_TAKE_ONE_LINE)
			{
				for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 4) + 1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
				}

				if (BestCardGroup.nCount / 4 == 2)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
				}
				if (BestCardGroup.nCount / 4 == 3)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_3);
				}
				if (BestCardGroup.nCount / 4 == 4)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_3);
					clsHandCardData.value_nPutCardList.push_back(tmp_4);
				}

				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			else if (BestCardGroup.cgType == cgTHREE_TAKE_TWO_LINE)
			{
				for (int j = BestCardGroup.nMaxCard - (BestCardGroup.nCount / 5) + 1; j <= BestCardGroup.nMaxCard; j++)
				{
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
					clsHandCardData.value_nPutCardList.push_back(j);
				}
				if (BestCardGroup.nCount / 5 == 2)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
				}
				if (BestCardGroup.nCount / 5 == 3)
				{
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_1);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_2);
					clsHandCardData.value_nPutCardList.push_back(tmp_3);
					clsHandCardData.value_nPutCardList.push_back(tmp_3);
				}
				clsHandCardData.uctPutCardType = BestCardGroup;
			}
			return;
		}
	}
	//如果没有3-2的非炸牌，则看看有没有单王
	if (clsHandCardData.value_aHandCardList[16] == 1 && clsHandCardData.value_aHandCardList[17] == 0)
	{
		clsHandCardData.value_nPutCardList.push_back(16);
		clsHandCardData.uctPutCardType = get_GroupData(cgSINGLE, 16, 1);
		return;
    }
	if (clsHandCardData.value_aHandCardList[16] == 0 && clsHandCardData.value_aHandCardList[17] == 1)
	{
		clsHandCardData.value_nPutCardList.push_back(17);
		clsHandCardData.uctPutCardType = get_GroupData(cgSINGLE, 17, 1);
		return;
	}
	//单王也没有，出炸弹
	for (int i = 3; i < 16; i++)
	{
		if (clsHandCardData.value_aHandCardList[i] == 4)
		{
			clsHandCardData.value_nPutCardList.push_back(i);
			clsHandCardData.value_nPutCardList.push_back(i);
			clsHandCardData.value_nPutCardList.push_back(i);
			clsHandCardData.value_nPutCardList.push_back(i);

			clsHandCardData.uctPutCardType = get_GroupData(cgBOMB_CARD, i, 4);

			return;
		}
	}
	
	//异常错误
	clsHandCardData.uctPutCardType = get_GroupData(cgERROR, 0, 0);
	return;

}

/*
2.0版本策略  根据场上形势决定当前预打出的手牌——分支处理
*/
void get_PutCardList_2(GameSituation &clsGameSituation, HandCardData &clsHandCardData)
{
	if (clsGameSituation.nCardDroit == clsHandCardData.nOwnIndex)
	{
		get_PutCardList_2_unlimit(clsGameSituation, clsHandCardData);
	}
	else
	{
		get_PutCardList_2_limit(clsGameSituation, clsHandCardData);
	}
	return;
}


/*
2.0版本策略  根据场上形势决定当前预打出的手牌——被动出牌
*/
void get_PutCardList_2_limit(GameSituation &clsGameSituation, HandCardData &clsHandCardData)
{
	clsHandCardData.ClearPutCardList();


	/*王炸——当前策略只处理王炸作为倒数第二手的优先出牌逻辑，后续版本会在此基础上优化*/
	if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
	{

		clsHandCardData.value_aHandCardList[17] --;
		clsHandCardData.value_aHandCardList[16] --;
		clsHandCardData.nHandCardCount -= 2;
		HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
		clsHandCardData.value_aHandCardList[16] ++;
		clsHandCardData.value_aHandCardList[17] ++;
		clsHandCardData.nHandCardCount += 2;
		if (tmpHandCardValue.NeedRound == 1)
		{
			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
			return;
		}
	}


	//错误牌型  不出
	if (clsGameSituation.uctNowCardGroup.cgType == cgERROR)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgERROR, 0, 0);
		return;
	}
	//不出牌型，在被动出牌策略里也是错误数据 不出
	else if (clsGameSituation.uctNowCardGroup.cgType == cgZERO)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//单牌类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgSINGLE)
	{

		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgSINGLE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD|| SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}     


		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard=0;
		//是否出牌的标志
		bool PutCards = false;


		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 18; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 0)
			{
				//尝试打出一张牌，估算剩余手牌价值
				clsHandCardData.value_aHandCardList[i]--;
				clsHandCardData.nHandCardCount--;
				HandCardValue tmpHandCardValue=get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i]++;
				clsHandCardData.nHandCardCount++;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue-(BestHandCardValue.NeedRound*7)) <= (tmpHandCardValue.SumValue-(tmpHandCardValue.NeedRound*7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgSINGLE, BestMaxCard, 1);
			return;
		}

		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}


		
	    //管不上
	    clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
		

	}
	//对牌类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgDOUBLE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgDOUBLE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}
		//-------------------------------------------对牌-------------------------------------------

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);

		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 18; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				//尝试打出一对牌，估算剩余手牌价值
				clsHandCardData.value_aHandCardList[i]-=2;
				clsHandCardData.nHandCardCount-=2;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i]+=2;
				clsHandCardData.nHandCardCount+=2;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgDOUBLE, BestMaxCard, 2);
			return;
		}


		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] ==4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
				//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
		    clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三牌类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgTHREE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}
		//-------------------------------------------三牌-------------------------------------------

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;

		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 18; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				//尝试打出一对牌，估算剩余手牌价值
				clsHandCardData.value_aHandCardList[i] -= 3;
				clsHandCardData.nHandCardCount -= 3;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 3;
				clsHandCardData.nHandCardCount += 3;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE, BestMaxCard, 3);
			return;
		}


		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//单连类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgSINGLE_LINE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgSINGLE_LINE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard
				&&SurCardGroupData.nCount== clsGameSituation.uctNowCardGroup.nCount)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}


		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;
		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount;
		//2与王不参与顺子，从当前已打出的顺子最小牌值+1开始遍历
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			/*
			这是一个连续段的问题，如果出现某张牌个数为0，那么必然不存在经过他的顺子
			那么就把计数器置零，如果计数器长度大于等于顺子长度，即可以组成顺子，我们以当前下标i为最高标志
			举个例子：对方牌型为34567，我从4遍历至8，若满足，此时end_i=8，即45678，继续走到9，若还满足，end_i=9
			即56789，若没有10，则prov归零，下一次循环若11存在，则prov=1。
			*/
			if (clsHandCardData.value_aHandCardList[i] > 0)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] --;
				}
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] ++;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = end_i;
					PutCards = true;
				}

			}
		}
			
		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
			}		
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgSINGLE_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}



		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//对连类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgDOUBLE_LINE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgDOUBLE_LINE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard
				&&SurCardGroupData.nCount == clsGameSituation.uctNowCardGroup.nCount)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}


		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;
		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount/2;
		//2与王不参与顺子，从当前已打出的顺子最小牌值+1开始遍历
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 1)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] -=2;
				}
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] +=2;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = end_i;
					PutCards = true;
				}

			}
		}

		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgDOUBLE_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}



		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;

	}
	//三连类型
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_LINE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgTHREE_LINE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard
				&&SurCardGroupData.nCount == clsGameSituation.uctNowCardGroup.nCount)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}


		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;
		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount / 3;
		//2与王不参与顺子，从当前已打出的顺子最小牌值+1开始遍历
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] -= 3;
				}
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] += 3;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = end_i;
					PutCards = true;
				}

			}
		}

		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}



		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一单
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_ONE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgTHREE_TAKE_ONE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}
	    		   
		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;
		//暂存最佳的牌号
		int BestMaxCard = 0;
		//顺带出去的牌
		int tmp_1 = 0;
		//是否出牌的标志
		bool PutCards = false;
		//三带一
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] >2)
			{
				for (int j = 3; j < 18; j++)
				{
					//选出一张以上的牌且不是选择三张的那个牌
					if (clsHandCardData.value_aHandCardList[j] > 0 && j != i)
					{
						clsHandCardData.value_aHandCardList[i] -= 3;
						clsHandCardData.value_aHandCardList[j] -= 1;
						clsHandCardData.nHandCardCount -= 4;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[i] += 3;
						clsHandCardData.value_aHandCardList[j] += 1;
						clsHandCardData.nHandCardCount += 4;
						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestMaxCard = i;
							tmp_1 = j;
							PutCards = true;
						}
					}
				}
			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_TAKE_ONE, BestMaxCard, 4);
			return;
		}
		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一对
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_TWO)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgTHREE_TAKE_TWO&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;
		//暂存最佳的牌号
		int BestMaxCard = 0;
		//顺带出去的牌
		int tmp_1 = 0;
		//是否出牌的标志
		bool PutCards = false;
		//三带一
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] >2)
			{
				for (int j = 3; j < 16; j++)
				{
					//选出一张以上的牌且不是选择三张的那个牌
					if (clsHandCardData.value_aHandCardList[j] > 1 && j != i)
					{
						clsHandCardData.value_aHandCardList[i] -= 3;
						clsHandCardData.value_aHandCardList[j] -= 2;
						clsHandCardData.nHandCardCount -= 5;
						HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
						clsHandCardData.value_aHandCardList[i] += 3;
						clsHandCardData.value_aHandCardList[j] += 2;
						clsHandCardData.nHandCardCount += 5;
						//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
						if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
						{
							BestHandCardValue = tmpHandCardValue;
							BestMaxCard = i;
							tmp_1 = j;
							PutCards = true;
						}
					}
				}
			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_TAKE_TWO, BestMaxCard, 5);
			return;
		}
		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}

		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一单连
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_ONE_LINE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgTHREE_TAKE_ONE_LINE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard
				&&SurCardGroupData.nCount == clsGameSituation.uctNowCardGroup.nCount)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}


		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;
		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount / 4;

		int tmp_1 = 0;
		int tmp_2 = 0;
		int tmp_3 = 0;
		int tmp_4 = 0;
		//2与王不参与顺子，从当前已打出的顺子最小牌值+1开始遍历
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] -= 3;
				}			
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;

				/*本来想做全排列选取带出的牌然后枚举出最高价值的，但考虑到当飞机长度也就是在2-4之间
				所以干脆做三个分支处理算了*/
				//为两连飞机
				if (length == 2)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 0)
						{
							clsHandCardData.value_aHandCardList[j] -= 1;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 0)
								{
									clsHandCardData.value_aHandCardList[k] -= 1;
									HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
									clsHandCardData.value_aHandCardList[k] += 1;

									//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
									if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
									{
										BestHandCardValue = tmpHandCardValue;
										BestMaxCard = end_i;
										tmp_1 = j;
										tmp_2 = k;
										PutCards = true;
									}
								}
							}
							clsHandCardData.value_aHandCardList[j] += 1;
						}

					}
				}
				//为三连飞机
				if (length == 3)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 0)
						{
							clsHandCardData.value_aHandCardList[j] -= 1;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 0)
								{
									clsHandCardData.value_aHandCardList[k] -= 1;
									for (int l = 3; l < 18; l++)
									{
										if (clsHandCardData.value_aHandCardList[l] > 0)
										{
											clsHandCardData.value_aHandCardList[l] -= 1;
											HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
											//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
											if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
											{
												BestHandCardValue = tmpHandCardValue;
												BestMaxCard = end_i;
												tmp_1 = j;
												tmp_2 = k;
												tmp_3 = l;
												PutCards = true;
											}
											clsHandCardData.value_aHandCardList[l] += 1;
										}
									}
									clsHandCardData.value_aHandCardList[k] += 1;
								}
							}
							clsHandCardData.value_aHandCardList[j] += 1;
						}
						

					}
				}
				//为四连飞机
				if (length == 4)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 0)
						{
							clsHandCardData.value_aHandCardList[j] -= 1;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 0)
								{
									clsHandCardData.value_aHandCardList[k] -= 1;
									for (int l = 3; l < 18; l++)
									{
										if (clsHandCardData.value_aHandCardList[l] > 0)
										{
											clsHandCardData.value_aHandCardList[l] -= 1;
											for (int m = 3; m < 18; m++)
											{
												if (clsHandCardData.value_aHandCardList[m] > 0)
												{
													clsHandCardData.value_aHandCardList[m] -= 1;
													HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
													//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
													if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
													{
														BestHandCardValue = tmpHandCardValue;
														BestMaxCard = end_i;
														tmp_1 = j;
														tmp_2 = k;
														tmp_3 = l;
														tmp_4 = m;
														PutCards = true;
													}
													clsHandCardData.value_aHandCardList[m] += 1;
												}
											}
											clsHandCardData.value_aHandCardList[l] += 1;
										}
									}
									clsHandCardData.value_aHandCardList[k] += 1;
								}
							}
							clsHandCardData.value_aHandCardList[j] += 1;
						}


					}
				}
			
				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] += 3;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;
			}
		}

		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}

			if (length == 2)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
		    }
			if (length == 3)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);

			}
			if (length == 4)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
				clsHandCardData.value_nPutCardList.push_back(tmp_4);
			}

			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_TAKE_ONE_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}



		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//三带一对连
	else if (clsGameSituation.uctNowCardGroup.cgType == cgTHREE_TAKE_TWO_LINE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgTHREE_TAKE_TWO_LINE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard
				&&SurCardGroupData.nCount == clsGameSituation.uctNowCardGroup.nCount)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}


		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);


		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;

		//暂存最佳的牌号
		int BestMaxCard = 0;
		//是否出牌的标志
		bool PutCards = false;
		//验证顺子的标志
		int prov = 0;
		//顺子起点
		int start_i = 0;
		//顺子终点
		int end_i = 0;
		//顺子长度
		int length = clsGameSituation.uctNowCardGroup.nCount / 5;

		int tmp_1 = 0;
		int tmp_2 = 0;
		int tmp_3 = 0;
		//2与王不参与顺子，从当前已打出的顺子最小牌值+1开始遍历
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard - length + 2; i < 15; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] > 2)
			{
				prov++;
			}
			else
			{
				prov = 0;
			}
			if (prov >= length)
			{
				end_i = i;
				start_i = i - length + 1;

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] -= 3;
				}
				clsHandCardData.nHandCardCount -= clsGameSituation.uctNowCardGroup.nCount;

				/*本来想做全排列选取带出的牌然后枚举出最高价值的，但考虑到当飞机长度也就是在2-3之间*/
				//为两连飞机
				if (length == 2)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 1)
						{
							clsHandCardData.value_aHandCardList[j] -= 2;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 1)
								{
									clsHandCardData.value_aHandCardList[k] -= 2;
									HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
									clsHandCardData.value_aHandCardList[k] += 2;

									//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
									if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
									{
										BestHandCardValue = tmpHandCardValue;
										BestMaxCard = end_i;
										tmp_1 = j;
										tmp_2 = k;
										PutCards = true;
									}
								}
							}
							clsHandCardData.value_aHandCardList[j] += 2;
						}

					}
				}
				//为三连飞机
				if (length == 3)
				{
					for (int j = 3; j < 18; j++)
					{
						if (clsHandCardData.value_aHandCardList[j] > 1)
						{
							clsHandCardData.value_aHandCardList[j] -= 2;
							for (int k = 3; k < 18; k++)
							{
								if (clsHandCardData.value_aHandCardList[k] > 1)
								{
									clsHandCardData.value_aHandCardList[k] -= 2;
									for (int l = 3; l < 18; l++)
									{
										if (clsHandCardData.value_aHandCardList[l] > 1)
										{
											clsHandCardData.value_aHandCardList[l] -= 2;
											HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
											//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
											if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
											{
												BestHandCardValue = tmpHandCardValue;
												BestMaxCard = end_i;
												tmp_1 = j;
												tmp_2 = k;
												tmp_3 = l;
												PutCards = true;
											}
											clsHandCardData.value_aHandCardList[l] += 2;
										}
									}
									clsHandCardData.value_aHandCardList[k] += 2;
								}
							}
							clsHandCardData.value_aHandCardList[j] += 2;
						}


					}
				}

				for (int j = start_i; j <= end_i; j++)
				{
					clsHandCardData.value_aHandCardList[j] += 3;
				}
				clsHandCardData.nHandCardCount += clsGameSituation.uctNowCardGroup.nCount;
			}
		}

		if (PutCards)
		{
			for (int j = start_i; j <= end_i; j++)
			{
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
				clsHandCardData.value_nPutCardList.push_back(j);
			}

			if (length == 2)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
			}
			if (length == 3)
			{
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_1);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_2);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);
				clsHandCardData.value_nPutCardList.push_back(tmp_3);

			}

			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgTHREE_TAKE_ONE_LINE, BestMaxCard, clsGameSituation.uctNowCardGroup.nCount);
			return;
		}

		//-------------------------------------------炸弹-------------------------------------------

		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{

				//尝试打出炸弹，估算剩余手牌价值,因为炸弹可以参与顺子，不能因为影响顺子而任意出炸
				clsHandCardData.value_aHandCardList[i] -= 4;
				clsHandCardData.nHandCardCount -= 4;
				HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
				clsHandCardData.value_aHandCardList[i] += 4;
				clsHandCardData.nHandCardCount += 4;

				//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
				if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7))
					//如果剩余手牌价值为正，证明出去的几率很大， 那么可以用炸获得先手
					|| tmpHandCardValue.SumValue > 0)
				{
					BestHandCardValue = tmpHandCardValue;
					BestMaxCard = i;
					PutCards = true;
				}

			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, BestMaxCard, 4);
			return;
		}

		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{
			//如果剩余手牌价值为正，证明出去的几率很大，那么可以用炸获得先手，王炸20分
			if (BestHandCardValue.SumValue > 20)
			{
				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);
				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);
				return;
			}
		}



		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//四带两单
	else if (clsGameSituation.uctNowCardGroup.cgType == cgFOUR_TAKE_ONE)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgFOUR_TAKE_ONE&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		/*下文中说到，本策略对于这种牌型有炸必炸，四带二相比炸弹来说会多带出两手牌，即最多提高14权值的价值
		若当前手牌价值大于14，即认为我们能炸即炸，不必考虑四带二的收益，就是这么任性。*/

		if (BestHandCardValue.SumValue > 14)
		{
			//炸弹——这里直接炸，不考虑拆分后果。因为信仰。
			for (int i = 3; i < 16; i++)
			{
				if (clsHandCardData.value_aHandCardList[i] == 4)
				{
					clsHandCardData.value_nPutCardList.push_back(i);
					clsHandCardData.value_nPutCardList.push_back(i);
					clsHandCardData.value_nPutCardList.push_back(i);
					clsHandCardData.value_nPutCardList.push_back(i);

					clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, i, 4);

					return;
				}
			}
			//王炸
			if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
			{

				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);

				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);

				return;
			}
		}
		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;
		//暂存最佳的牌号
		int BestMaxCard = 0;
		//顺带出去的牌
		int tmp_1 = 0;
		int tmp_2 = 0;
		//是否出牌的标志
		bool PutCards = false;
		//同为四带二
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{
				for (int j = 3; j < 18; j++)
				{
					//先选出一张以上的牌且不是选择四张的那个牌
					if (clsHandCardData.value_aHandCardList[j] > 0 && j != i)
					{
						//再选出一张以上的牌且不是选择四张的那个牌且不是第一次选的两张内个牌（策略里四带二不允许带一对,还不如炸）
						for (int k = j + 1; k < 18; k++)
						{
							if (clsHandCardData.value_aHandCardList[k] > 0 && k != i)
							{
								clsHandCardData.value_aHandCardList[i] -= 4;
								clsHandCardData.value_aHandCardList[j] -= 1;
								clsHandCardData.value_aHandCardList[k] -= 1;
								clsHandCardData.nHandCardCount -= 6;
								HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
								clsHandCardData.value_aHandCardList[i] += 4;
								clsHandCardData.value_aHandCardList[j] += 1;
								clsHandCardData.value_aHandCardList[k] += 1;
								clsHandCardData.nHandCardCount += 6;

								//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
								if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
								{
									BestHandCardValue = tmpHandCardValue;
									BestMaxCard = i;
									tmp_1 = j;
									tmp_2 = k;
									PutCards = true;
								}
							}
						}
					}
				}
			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgFOUR_TAKE_ONE, BestMaxCard, 6);
			return;
		}
		//这里是在拍权值较小的情况，且没有选择出良好的四带二牌型，那么也要炸，因为信仰。
		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);

				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, i, 4);

				return;
			}
		}
		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{

			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);

			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);

			return;
		}
		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//四带两对
	else if (clsGameSituation.uctNowCardGroup.cgType == cgFOUR_TAKE_TWO)
	{
		//剪枝：如果能出去最后一手牌直接出
		CardGroupData SurCardGroupData = ins_SurCardsType(clsHandCardData.value_aHandCardList);
		if (SurCardGroupData.cgType != cgERROR)
		{
			if (SurCardGroupData.cgType == cgFOUR_TAKE_TWO&&SurCardGroupData.nMaxCard>clsGameSituation.uctNowCardGroup.nMaxCard)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
			else if (SurCardGroupData.cgType == cgBOMB_CARD || SurCardGroupData.cgType == cgKING_CARD)
			{
				Put_All_SurCards(clsGameSituation, clsHandCardData, SurCardGroupData);
				return;
			}
		}

		//暂存最佳的价值
		HandCardValue BestHandCardValue = get_HandCardValue(clsHandCardData);
		/*下文中说到，本策略对于这种牌型有炸必炸，四带二相比炸弹来说会多带出两手牌，即最多提高14权值的价值
		若当前手牌价值大于14，即认为我们能炸即炸，不必考虑四带二的收益，就是这么任性。*/

		if (BestHandCardValue.SumValue > 14)
		{
			//炸弹——这里直接炸，不考虑拆分后果。因为信仰。
			for (int i = 3; i < 16; i++)
			{
				if (clsHandCardData.value_aHandCardList[i] == 4)
				{
					clsHandCardData.value_nPutCardList.push_back(i);
					clsHandCardData.value_nPutCardList.push_back(i);
					clsHandCardData.value_nPutCardList.push_back(i);
					clsHandCardData.value_nPutCardList.push_back(i);

					clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, i, 4);

					return;
				}
			}
			//王炸
			if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
			{

				clsHandCardData.value_nPutCardList.push_back(17);
				clsHandCardData.value_nPutCardList.push_back(16);

				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);

				return;
			}
		}
		//我们认为不出牌的话会让对手一个轮次，即加一轮（权值减少7）便于后续的对比参考。
		BestHandCardValue.NeedRound += 1;
		//暂存最佳的牌号
		int BestMaxCard = 0;
		//顺带出去的牌
		int tmp_1 = 0;
		int tmp_2 = 0;
		//是否出牌的标志
		bool PutCards = false;
		//同为四带二
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{
				for (int j = 3; j < 16; j++)
				{
					//先选出两张以上的牌且不是选择四张的那个牌
					if (clsHandCardData.value_aHandCardList[j] > 1 && j != i)
					{
						//再选出两张以上的牌且不是选择四张的那个牌且不是第一次选的两张内个牌
						for (int k = j + 1; k < 16; k++)
						{
							if (clsHandCardData.value_aHandCardList[k] > 1 && k != i) 
							{
								clsHandCardData.value_aHandCardList[i] -= 4;
								clsHandCardData.value_aHandCardList[j] -= 2;
								clsHandCardData.value_aHandCardList[k] -= 2;
								clsHandCardData.nHandCardCount -= 8;
								HandCardValue tmpHandCardValue = get_HandCardValue(clsHandCardData);
								clsHandCardData.value_aHandCardList[i] += 4;
								clsHandCardData.value_aHandCardList[j] += 2;
								clsHandCardData.value_aHandCardList[k] += 2;
								clsHandCardData.nHandCardCount += 8;

								//选取总权值-轮次*7值最高的策略  因为我们认为剩余的手牌需要n次控手的机会才能出完，若轮次牌型很大（如炸弹） 则其-7的价值也会为正
								if ((BestHandCardValue.SumValue - (BestHandCardValue.NeedRound * 7)) <= (tmpHandCardValue.SumValue - (tmpHandCardValue.NeedRound * 7)))
								{
									BestHandCardValue = tmpHandCardValue;
									BestMaxCard = i;
									tmp_1 = j;
									tmp_2 = k;
									PutCards = true;
								}
							}
						}
					}
				}
			}
		}
		if (PutCards)
		{
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(BestMaxCard);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_1);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
			clsHandCardData.value_nPutCardList.push_back(tmp_2);
			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgFOUR_TAKE_TWO, BestMaxCard, 8);
			return;
		}
        //这里是在拍权值较小的情况，且没有选择出良好的四带二牌型，那么也要炸，因为信仰。
		for (int i = 3; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);

				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, i, 4);

				return;
			}
		}
		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{

			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);

			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);

			return;
		}
		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;

	}
	//炸弹类型 
	else if (clsGameSituation.uctNowCardGroup.cgType == cgBOMB_CARD)
	{
		//更大的炸弹——这里直接炸，不考虑拆分后果。因为信仰。
		for (int i = clsGameSituation.uctNowCardGroup.nMaxCard + 1; i < 16; i++)
		{
			if (clsHandCardData.value_aHandCardList[i] == 4)
			{
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);
				clsHandCardData.value_nPutCardList.push_back(i);

				clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgBOMB_CARD, i, 4);

				return;
			}
		}
		//王炸
		if (clsHandCardData.value_aHandCardList[17] > 0 && clsHandCardData.value_aHandCardList[16] > 0)
		{

			clsHandCardData.value_nPutCardList.push_back(17);
			clsHandCardData.value_nPutCardList.push_back(16);

			clsHandCardData.uctPutCardType = clsGameSituation.uctNowCardGroup = get_GroupData(cgKING_CARD, 17, 2);

			return;
		}
		//管不上
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
		
	}
	//王炸类型 人都王炸了你还出个毛
	else if (clsGameSituation.uctNowCardGroup.cgType == cgKING_CARD)
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
		return;
	}
	//异常处理 不出
	else
	{
		clsHandCardData.uctPutCardType = get_GroupData(cgZERO, 0, 0);
	}
	return;
}

/*
2.0版本策略  根据场上形势决定当前预打出的手牌——主动出牌
*/
void get_PutCardList_2_unlimit(GameSituation &clsGameSituation, HandCardData &clsHandCardData)
{
	//2.0版本暂时只考虑自身手牌利益的情况
	get_PutCardList_2(clsHandCardData);

	clsGameSituation.uctNowCardGroup = clsHandCardData.uctPutCardType;
}
