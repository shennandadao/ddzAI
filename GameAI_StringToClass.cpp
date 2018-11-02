#include "GameAI_StringToClass.h"

//string retJson;


/*本来计划不用json的奈何调试不通过
void stringtrim(string &str){
        int begin = 0;
        begin = str.find(" ",begin);
        while(begin != -1)
        {

                str.replace(begin, 1, "");  // 用空串替换str中从begin开始的1个字符
                begin = str.find(" ",begin);  //查找空格在替换后的str中第一次出现的位置
        }

}

void get_string_from_struct(string &str, map<string, int> &map_value, map<string, vector<int> > &map_value_vec)
{
        str = str + "{";

	cout << "=================get_string_from_struct=========map_value begin========" << endl;
        for(map<string, int>::iterator it = map_value.begin(); it != map_value.end(); it++){
                cout << it->first << "    " << it->second << endl;
                str = str + "\"";
		char st[10];
		memset( st, 0, sizeof(st) ); 
                sprintf( st, "%d", it->second);
                string s(st);

                str = str + it->first + "\":" + s + ",";
        }
	cout << "=================get_string_from_struct=========map_value end========" << endl;

	cout << "=================get_string_from_struct=========map_value_vec begin========" << endl;
        for(map<string, vector<int> >::iterator it = map_value_vec.begin(); it != map_value_vec.end(); it++){
                cout << it->first << "    " ;
               	 
                str = str + "\"" + it->first + "\"";

                vector<int>  vec = it->second;
                string t_str(":[");
		cout <<"数组大小:" << vec.size() << "    " ;
		if (vec.size() > 0){
                	for (size_t  i=0; i < vec.size() ; i++){
                        	cout << vec[i] << "     ";
                        	char st[10];
				memset( st, 0, sizeof(st) ); 
                        	sprintf( st, "%d", vec[i]);
                        	string s(st);

                        	t_str = t_str + s + ",";
                        
                	}
		
			t_str = t_str.substr(0, t_str.size() - 1);
		}
		
                t_str = t_str + "]";

                str = str + t_str + ",";

		cout << endl;
        }

	str = str.substr(0, str.size()-1);	

        str = str + "}";
	cout << "=================get_string_from_struct=========map_value_vec end========" << endl;

        return;
}


void get_struct_from_string(string &str, map<string, int> & map_value, map<string, vector<int> > &map_value_vec)
{
	cout << "=====================================================" << endl;	
	cout << "string转成map start" << endl;	
	stringtrim(str);
	cout << str << endl;
	vector < string > vecStr;	
	vector < string >::iterator vecStrIter;	


	string::iterator iter = str.begin();
	string::iterator tmpiter = str.begin();

	string bstr=",\"";
	int pos;
	for(int i=1; i <= 7; i++){
		pos  = str.find(bstr);
		if(i == 1){
			cout << str.substr(1, str.find(bstr)) << endl;
			vecStr.push_back( str.substr(1, str.find(bstr) - 1) );
			str =  str.substr(pos + 1, str.size());
		}else{
			cout << str.substr(0, str.find(bstr)) << endl;
			vecStr.push_back( str.substr(0, str.find(bstr)) );
			str = str.substr(pos + 1, str.size());
			if(i==7){
				cout << str.substr(0, str.size()-1) << endl;
				vecStr.push_back( str.substr(0, str.size()-1) );
			}
		}
	}		

	
	cout << "------------------------------" << endl;
	bstr = ":[";
	for(size_t i=0; i < vecStr.size(); i++){
		string st = vecStr[i];
		pos = st.find(bstr);	
		cout << pos << "    " << st << endl;	
		if(pos == -1){
			string tmpStr = ":";
			int tmpPos = st.find(tmpStr);
			//cout << tmpPos << endl; 
			string k = st.substr(1, tmpPos-2);
			string vStr = st.substr(tmpPos+1, st.size());
			int value = atoi(vStr.c_str());
			cout << k << "    "  << value << endl;
			map_value.insert(pair<string, int>(k, value));
		}else{
			int tmpPos = st.find(bstr);
			//cout << tmpPos << endl;
			string k = st.substr(1, tmpPos -2);
			string v = st.substr(tmpPos+1, st.size()-1);	
			cout << k << "    "  << v << endl;
			vector<int> vec;
			if (v.length() != 2){
				int len = v.size(); 
				for(int i=1; i < len; i++){
					string cstr = ",";
					int pos = v.find(cstr);
					
					if(pos > 0){
						if(i==1){
						   	string value = v.substr(1, pos -1);			
							v = v.substr(pos+1, v.size());	
							cout << value << "    " << v << endl;
							vec.push_back(atoi(value.c_str()));
						}else{
							string value = v.substr(0, pos );
							v = v.substr(pos +1, v.size());
							cout << value << "    " << v << endl;
							vec.push_back(atoi(value.c_str()));
						}	
					}else{
						string dstr = "]";
						//int pos = v.find(cstr);
						string value = v.substr(0, v.size()-1);
						//v = v.substr(pos, v.size());
						v = "]";
						cout  <<  value << "    " << endl;
						vec.push_back(atoi(value.c_str()));
						break;	
					}	
				}	
			}

			map_value_vec.insert(pair<string, vector<int> >(k, vec));
			
		}
	}		
	cout << "string转成map end" << endl;	
	cout << "=====================================================" << endl;	

	return;

}


*/

/*
Json包含四个部分：
操作类型         Operation    
枚举类型：       0叫地主1出牌
游戏全局类       GameSituation
手牌信息类       HandCardData
*/
/*
void  get_ClassForString(string& str, int &nRank,int  &nOperation, GameSituation &clsGameSituation, HandCardData&clsHandCardData)
{
	cout << " ==============string to class==========begin=================" << endl;
	cout << " string 转换 class" << endl;

	map<string, int> map_value;
	map<string, vector<int> > map_value_vec;

	get_struct_from_string(str, map_value, map_value_vec);

	//打印map
	cout << " ************map打印**start*********" << endl;
	for(map<string, int>::iterator it = map_value.begin(); it != map_value.end(); it++){
		cout << it->first << "    " << it->second << endl;
	}	

	for(map<string, vector<int> >::iterator it = map_value_vec.begin(); it != map_value_vec.end(); it++){
		vector<int> vec = it->second;
		cout << it->first << "    ";
		for(size_t i=0; i < vec.size(); i++){
			cout << vec[i] << "  ";
		}
		cout << endl;	
	}	

	cout << " ************map打印  end ************" << endl;
	
	for(map<string, int>::iterator it = map_value.begin(); it != map_value.end(); it++)
	{
		cout << it->first << "     " << it->second << endl;
		if(it->first == string("nOperation")){
			nOperation = it->second;
		}else if (it->first == string("nRank")){
			nRank = it->second;

		}else if (it->first == string("nNowLandScore")){
			clsGameSituation.nNowLandScore = it->second;

		}else if (it->first == string("nCardDroit")){
			clsGameSituation.nCardDroit = it->second;
		}else if (it->first == string("nOwnIndex")){
			clsHandCardData.nOwnIndex = it->second;
		}
	}

	
	for(map<string, vector<int> >::iterator it = map_value_vec.begin(); it != map_value_vec.end(); it++){
		vector<int> vec = it->second;		
		
		cout << it->first << "    " << endl;
		for(size_t i=0; i< vec.size();i++){
			cout << vec[i] << "    ";
		}		
		cout << endl;

		if(it->first == string("NowCardsList")){
			clsGameSituation.uctNowCardGroup = ins_SurCardsType(vec);
						
		}else if (it->first == string("HandCardList")){
			for(size_t i=0; i < vec.size(); i++){
				if(vec[i] == 53){
					clsHandCardData.color_nHandCardList.push_back(56);
				}else{

					clsHandCardData.color_nHandCardList.push_back(vec[i]);
				}
			}
		}

	}	


	clsHandCardData.Init();


	cout << " ==============string to class==========end=================" << endl;
	cout << "字符串转成类后: 打印类数据" << endl;
	cout << "AI等级:" << nRank << endl;	
	cout << "nOperation操作:" << nOperation << endl;	
	cout << "nNowLandScore当前叫地主阶段叫的分值:" << clsGameSituation.nNowLandScore << endl;	
	cout << "当前控手的玩家座位id:" << clsGameSituation.nCardDroit << endl;
	cout << "机器人自己的座位id:" << clsHandCardData.nOwnIndex << endl;
	
	cout << "手牌:";
	for(size_t i =0; i<clsHandCardData.color_nHandCardList.size(); i++)
	{
		cout << clsHandCardData.color_nHandCardList[i] << "   "; 
	}	
	cout << endl;
	cout << "clsGameSituation.uctNowCardGroup：" << clsGameSituation.uctNowCardGroup.cgType << "    " << clsGameSituation.uctNowCardGroup.nValue << "    " << clsGameSituation.uctNowCardGroup.nCount << "   " << clsGameSituation.uctNowCardGroup.nMaxCard << endl;

	cout << "string转成类后动态库中进行工作中" << endl;
}



void get_StringForClass(string& str, int &nRank, int  &nOperation,GameSituation &clsGameSituation, HandCardData&clsHandCardData)
{
	cout << "============================" << endl;
	cout << " class to string" << endl;
	cout << "动态库策略完成后" << endl;
	cout << "打印类的数据" << endl;

	
	cout << "字符串转成类后: 打印类数据" << endl;
	cout << "AI等级:" << nRank << endl;	
	cout << "nOperation操作:" << nOperation << endl;	
	cout << "nNowLandScore当前叫地主阶段叫的分值:" << clsGameSituation.nNowLandScore << endl;	
	cout << "当前控手的玩家座位id:" << clsGameSituation.nCardDroit << endl;
	cout << "机器人自己的座位id:" << clsHandCardData.nOwnIndex << endl;
	
	cout << "手牌:";
	for(size_t i =0; i<clsHandCardData.color_nHandCardList.size(); i++)
	{
		cout << clsHandCardData.color_nHandCardList[i] << "   "; 
	}	
	cout << endl;


	cout << " NowCardList:"; 
		for (size_t i = 0; i < clsGameSituation.PutCardList.size(); i++)
	{
		if (clsGameSituation.PutCardList[i] == 56)
		{
			cout<<  53 << "   ";
		}
		else
		{
			cout << clsGameSituation.PutCardList[i] << "    ";
		}
	} 

	cout << " PutCardList:";	
	for (size_t i = 0; i < clsHandCardData.color_nPutCardList.size(); i++)
	{
		if (clsHandCardData.color_nPutCardList[i] == 56)
		{
			cout << 53 << "   ";
		}
		else
		{
			cout << clsHandCardData.color_nPutCardList[i] << "   ";
		}

	}

	
	str = str + "{\"nRank\":";

	char st[10];
        memset( st, 0, sizeof(st) );
        sprintf( st, "%d", nRank);
	str = str + st + ",";


	str = str + "\"nOperation\":";
        memset( st, 0, sizeof(st) );
        sprintf( st, "%d", nOperation); 
	str = str + st + ",";

	str = str + "\"nNowLandScore\":";
        memset( st, 0, sizeof(st) );
        sprintf( st, "%d", clsGameSituation.nNowLandScore); 
	str = str + st + ",";

	str = str + "\"nCardDroit\":";
        memset( st, 0, sizeof(st) );
        sprintf( st, "%d", clsGameSituation.nCardDroit); 
	str = str + st + ",";

	str = str + "\"nOwnIndex\":";
        memset( st, 0, sizeof(st) );
        sprintf( st, "%d", clsHandCardData.nOwnIndex); 
	str = str + st + ",";


	cout << " clsGameSituation.PutCardList:" << clsGameSituation.PutCardList.size() << endl;
	if( clsGameSituation.PutCardList.size() > 0 ){	
		str = str + "\"NowCardsList\":[";
        	memset( st, 0, sizeof(st) );
		for(size_t i=0; i < clsGameSituation.PutCardList.size();i++){
			if (clsGameSituation.PutCardList[i] == 56)
			{
        		sprintf( st, "%d", 53); 
			str = str + st;
			str = str + ",";		
			}
			else
			{
			sprintf( st, "%d", clsGameSituation.PutCardList[i]); 
			str = str + st;
			str = str + ",";		
			}
		}
		str = str.substr(0, str.size()-1);
		str = str + "],";
	}else{
		str = str + "\"NowCardsList\":[],";
	}	


	
	if(clsHandCardData.color_nHandCardList.size() > 0){
		str = str + "\"HandCardList\":[";
        	memset( st, 0, sizeof(st) );
	for (size_t i = 0; i < clsHandCardData.color_nHandCardList.size(); i++)
	{
		if (clsHandCardData.color_nHandCardList[i] == 56)
		{
        		sprintf( st, "%d", 53); 
			str = str + st;
			str = str + ",";		
		}
		else
		{
			sprintf( st, "%d", clsHandCardData.color_nHandCardList[i]); 
			str = str + st;
			str = str + ",";		
		}
		
	}
		str = str.substr(0, str.size()-1);
		str = str + "],";
	}
	else{
		str = str + "\"HandCardList\":[],";
	}



	if(clsHandCardData.color_nPutCardList.size() > 0){
		str = str + "\"PutCardList\":[";
        	memset( st, 0, sizeof(st) );
	for (size_t i = 0; i < clsHandCardData.color_nPutCardList.size(); i++)
	{
		if (clsHandCardData.color_nPutCardList[i] == 56)
		{
        		sprintf( st, "%d", 53); 
			str = str + st;
			str = str + ",";		
		}
		else
		{
			sprintf( st, "%d", clsHandCardData.color_nPutCardList[i]); 
			str = str + st;
			str = str + ",";		
		}
		
	}
		str = str.substr(0, str.size() - 1);
		str = str + "]";
	}else{
		str = str + "\"PutCardList\":[]";
	}
	
	str = str + "}";

	//stringtrim(str);
}
*/



void  JsonValueToVector(Json::Value value, vector<int>&list)
{
	list.clear();
	for (unsigned i = 0; i <value.size(); i++)
	{
		if (value[i].asInt() != 53)
		{
			list.push_back((value[i].asInt() / 4) + 3);
		}
		else
		{
			list.push_back(17);
		}

	}
}
/*
Json包含四个部分：
操作类型         Operation    
枚举类型：       0叫地主1出牌
游戏全局类       GameSituation
手牌信息类       HandCardData
*/
void  get_ClassForJson(string&strJson, int &nRank,int  &nOperation, GameSituation &clsGameSituation, HandCardData&clsHandCardData)
{

	Json::Reader reader;
	Json::Value root;
	if (reader.parse(strJson, root))  // reader将Json字符串解析到root，root将包含Json里所有子元素  
	{

		vector<int>tmplist;

		nOperation = root["nOperation"].asInt();
		nRank = root["nRank"].asInt();
		clsGameSituation.nNowLandScore = root["nNowLandScore"].asInt();
		clsGameSituation.nCardDroit =root["nCardDroit"].asInt();
		clsHandCardData.nOwnIndex = root["nOwnIndex"].asInt();

		Json::Value PutCardList = root["NowCardsList"];

		JsonValueToVector(PutCardList, tmplist);
		clsGameSituation.uctNowCardGroup = ins_SurCardsType(tmplist);

		Json::Value HandCardList = root["HandCardList"];

		for (unsigned i = 0; i <HandCardList.size(); i++)
		{
			if (HandCardList[i].asInt() == 53)
			{
				clsHandCardData.color_nHandCardList.push_back(56);
			}
			else
			{
				clsHandCardData.color_nHandCardList.push_back(HandCardList[i].asInt());
			}

		}

		clsHandCardData.Init();

	}

}



void get_JsonForClass(string&strJson, int &nRank, int  &nOperation,GameSituation &clsGameSituation, HandCardData&clsHandCardData)
{
	Json::Value root;

	Json::Value Null;
	root["nOperation"] = nOperation;
	root["nRank"] = nRank;
	root["nNowLandScore"] = clsGameSituation.nNowLandScore;
	root["nCardDroit"] = clsGameSituation.nCardDroit;
	root["nOwnIndex"] = clsHandCardData.nOwnIndex;

	root["NowCardsList"]= Null;
	root["HandCardList"]= Null;
    root["PutCardList"]= Null;
	for (size_t i = 0; i < clsGameSituation.PutCardList.size(); i++)
	{
		int num = static_cast<int>(i);
		if (clsGameSituation.PutCardList[num] == 56)
		{
			root["NowCardsList"][num] = 53;
		}
		else
		{
			root["NowCardsList"][num] = clsGameSituation.PutCardList[num];
		}
	}
	for (size_t num = 0; num < clsHandCardData.color_nHandCardList.size(); num++)
	{
		int i = static_cast<int>(num);
		if (clsHandCardData.color_nHandCardList[i] == 56)
		{
			root["HandCardList"][i] = 53;
		}
		else
		{
			root["HandCardList"][i] = clsHandCardData.color_nHandCardList[i];
		}
		
	}
	for (size_t num = 0; num < clsHandCardData.color_nPutCardList.size(); num++)
	{
		int i = static_cast<int>(num);
		if (clsHandCardData.color_nPutCardList[i] == 56)
		{
			root["PutCardList"][i] = 53;
		}
		else
		{
			root["PutCardList"][i] = clsHandCardData.color_nPutCardList[i];
		}

	}




	Json::FastWriter writer;
	strJson = writer.write(root);

	//strJson=root.toStyledString();
}




//lua调用c++库接口
const char * InterfaceSo(const char* sJson)
{
	string strJson = sJson;

	string retJson;

	int nOperation = -1;
	int nRank = 0;
	GameSituation clsGameSituation;
	HandCardData  clsHandCardData;

	//cout << "strJson is " << strJson << endl;
	get_ClassForJson(strJson, nRank,nOperation, clsGameSituation, clsHandCardData);
	
	//叫地主
	if (nOperation==0)
	{
		int  tmpLandScore = LandScore(clsGameSituation, clsHandCardData);
		if (tmpLandScore > clsGameSituation.nNowLandScore)
		{
			clsGameSituation.nNowLandScore = tmpLandScore;
		}
		
	}
	//出牌
    else if (nOperation == 1)
	{
		if (nRank == 1)
		{
			get_PutCardList_1(clsGameSituation, clsHandCardData);
			clsHandCardData.PutCards();
			clsGameSituation.PutCardList = clsHandCardData.color_nPutCardList;
		}
		else if(nRank == 2)
		{
			get_PutCardList_2(clsGameSituation, clsHandCardData);
			clsHandCardData.PutCards();
			clsGameSituation.PutCardList = clsHandCardData.color_nPutCardList;
		}
		else
		{
			cout << "AI等级目前只支持1  2" << endl;
		}
		if (clsHandCardData.uctPutCardType.cgType != cgZERO&&clsHandCardData.uctPutCardType.cgType!=cgERROR)
		{
			clsGameSituation.nCardDroit = clsHandCardData.nOwnIndex;
			clsGameSituation.uctNowCardGroup = clsHandCardData.uctPutCardType;
		}
		

	}
	else
	{
		cout << "nOpeation:目前只支持0叫分  1出牌操作" << endl;
		return strJson.c_str();
	}


	get_JsonForClass(retJson, nRank,nOperation,clsGameSituation, clsHandCardData);

	return  retJson.c_str();
}

