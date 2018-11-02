#include "GameAI_Config.h"

#include "GameAI_Algorithm.h"
#include "GameAI_Test.h"
#include "GameAI_Strategy.h"
#include "GameAI_GroupType.h"
#include "json/json.h"
//通过传递的JSON串构造程序需要的两个大类

/*
字符串包含四个部分：
操作类型         Operation    
枚举类型：       0叫地主1出牌
游戏全局类       GameSituation
手牌信息类       HandCardData
*/
/*本来计划不想
void stringtrim(string& str);

void  get_ClassForString(string& str, int &nRank,int  &nOperation, GameSituation &clsGameSituation, HandCardData&clsHandCardData);

void get_StringForClass(string& str, int &nRank, int  &nOperation,GameSituation &clsGameSituation, HandCardData&clsHandCardData);



//辅助string转class
void get_string_from_struct(string &str, map<string, int> &map_value, map<string, vector<int> > &map_value_vec);


//辅助class转string
void get_struct_from_string(string &str, map<string, int> & map_value, map<string, vector<int> > &map_value_vec);
*/






void  get_ClassForJson(string&strJson, int &nRank, int  &nOperation, GameSituation &clsGameSituation, HandCardData&clsHandCardData);


void get_JsonForClass(string&strJson, int &nRank, int  &nOperation,GameSituation &clsGameSituation, HandCardData&clsHandCardData);

void  JsonValueToVector(Json::Value value, vector<int>&list);

const char * InterfaceSo(const char* sJson);
