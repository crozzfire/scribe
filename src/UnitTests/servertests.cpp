#include "servertests.h"
#include "common.h"
#include "env_default.h"

using namespace facebook::fb303;
using namespace facebook;

using namespace scribe::thrift;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(ServerTests);

void ServerTests::setUp(){
	g_Handler->reinitialize();
}

void ServerTests::tearDown(){
}

void ServerTests::logResultCheck(){
	ResultCode result;

	//Valid LogEntry
	boost::shared_ptr<LogEntry> msg(new LogEntry());
	msg->category="test_cat";
	msg->message="test_msg";

	//Blank Entry - Still valid
	boost::shared_ptr<LogEntry> b_msg(new LogEntry());
	b_msg->category="";
	b_msg->message="";

	vector<LogEntry> messages;
	messages.push_back(*msg);
	messages.push_back(*b_msg);

	result = g_Handler->Log(messages);

	CPPUNIT_ASSERT(result == OK);
	CPPUNIT_ASSERT(result != TRY_LATER);

	boost::shared_ptr<LogEntry> invalidCat(new LogEntry());
	invalidCat->category="!@#!#@GSDGFDS@#$#@";
	invalidCat->message="SDGDSFGW#$^#$^#$";

	vector<LogEntry> invalidCats;
	invalidCats.push_back(*invalidCat);
	g_Handler->status = STOPPING;
	result = g_Handler->Log(invalidCats);

	CPPUNIT_ASSERT(result != OK);
	CPPUNIT_ASSERT(result == TRY_LATER);
	g_Handler->status=ALIVE;

}

void ServerTests::throttleCheck(){

	string huge_string;
	long long huge_number=200000;
	while(huge_number--){
		huge_string+=char(huge_number%128);
	}

	boost::shared_ptr<LogEntry> msg(new LogEntry());
	msg->category="test_cat";
	msg->message=huge_string;

	//creating a sort of overflow to check throttling due to rate
	vector<LogEntry> messages;
	huge_number=500;
	while(huge_number--){
		messages.push_back(*msg);
	}

	ResultCode result = g_Handler->Log(messages);
	//Throttling check should Allow this because its a single huge packet
	CPPUNIT_ASSERT(result == OK);

	vector<LogEntry> _temp;
	_temp.push_back(*msg);

	huge_number=500;
	while(huge_number--) g_Handler->Log(_temp);

	result=g_Handler->Log(_temp);
	//Throttling check should Deny now
	CPPUNIT_ASSERT(result == TRY_LATER);
}

//Write the configure scripts according to the test conditions
//or alternatively change the assertions according to the config file
void ServerTests::storeConfigCheck(){

	//StoreConf testConfig=g_Handler->getConfig();

	StoreConf testConfig;
	string testFileName="/tmp/scribe.conf";
	testConfig.parseConfig(testFileName);
	int numstores=0;
	bool result;
    std::vector<pStoreConf> store_confs;
    testConfig.getAllStores(store_confs);

    std::vector<pStoreConf>::iterator iter = store_confs.begin();
    result= g_Handler->configureStore(*iter, &numstores);
    CPPUNIT_ASSERT(result == false);
    CPPUNIT_ASSERT(numstores==0);
    ++iter;

    result= g_Handler->configureStore(*iter, &numstores);
    CPPUNIT_ASSERT(result == true);
    CPPUNIT_ASSERT(numstores==1);
    ++iter;

    result= g_Handler->configureStore(*iter, &numstores);
    CPPUNIT_ASSERT(result == false);
    CPPUNIT_ASSERT(numstores==1);
}

void ServerTests::newCategoryCheck(){
	boost::shared_ptr<store_list_t> testStoreList;
	category_map_t::iterator cat_iter;

	string nonRegexCat="helloWorld";
	testStoreList=g_Handler->createNewCategory(nonRegexCat);
	//Will not be added to categories map, as non_Model categories are added during initialization time.
	cat_iter = g_Handler->categories.find(nonRegexCat);
	CPPUNIT_ASSERT(cat_iter == g_Handler->categories.end());
	CPPUNIT_ASSERT(testStoreList == NULL);

	string regexCat="HelloWorld";
	testStoreList=g_Handler->createNewCategory(regexCat);
	cat_iter = g_Handler->categories.find(regexCat);
	CPPUNIT_ASSERT(cat_iter != g_Handler->categories.end());

	//This filename is too long in most of the current OSes and should fail to create
	string invalidCat="";
	int largeNum=100;
	while(largeNum--) invalidCat+="Abcdef12";

	try{
	testStoreList=g_Handler->createNewCategory(invalidCat);
	}catch(exception e){}

	cat_iter = g_Handler->categories.find(invalidCat);
	CPPUNIT_ASSERT(cat_iter == g_Handler->categories.end());

}

void ServerTests::deleteCategoryMapCheck(){

	category_map_t::iterator cat_iter;

	category_map_t catCopy=g_Handler->categories;
	g_Handler->deleteCategoryMap(catCopy);
	cat_iter = catCopy.begin();
	CPPUNIT_ASSERT(cat_iter==catCopy.end());

	category_map_t regexCatCopy=g_Handler->category_regex;
	g_Handler->deleteCategoryMap(regexCatCopy);
	cat_iter = regexCatCopy.begin();
	CPPUNIT_ASSERT(cat_iter==regexCatCopy.end());

	category_map_t blacklistCatCopy=g_Handler->category_regex_blacklist;
	g_Handler->deleteCategoryMap(blacklistCatCopy);
	cat_iter = blacklistCatCopy.begin();
	CPPUNIT_ASSERT(cat_iter==blacklistCatCopy.end());

}
