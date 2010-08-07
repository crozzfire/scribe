#include "servertests.h"
#include "Common.h"
#include "EnvDefault.h"

using namespace facebook::fb303;
using namespace facebook;

using namespace scribe::thrift;
using namespace std;
using namespace scribe;

CPPUNIT_TEST_SUITE_REGISTRATION(ServerTests);

void ServerTests::setUp(){
	g_handler->reinitialize();
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

	result = g_handler->Log(messages);

	CPPUNIT_ASSERT(result == OK);
	CPPUNIT_ASSERT(result != TRY_LATER);

	boost::shared_ptr<LogEntry> invalidCat(new LogEntry());
	invalidCat->category="!@#!#@GSDGFDS@#$#@";
	invalidCat->message="SDGDSFGW#$^#$^#$";

	vector<LogEntry> invalidCats;
	invalidCats.push_back(*invalidCat);
	g_handler->status_ = STOPPING;
	result = g_handler->Log(invalidCats);

	CPPUNIT_ASSERT(result != OK);
	CPPUNIT_ASSERT(result == TRY_LATER);
	g_handler->status_=ALIVE;

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

	ResultCode result = g_handler->Log(messages);
	//Throttling check should Allow this because its a single huge packet
	CPPUNIT_ASSERT(result == OK);

	vector<LogEntry> _temp;
	_temp.push_back(*msg);

	huge_number=500;
	while(huge_number--) g_handler->Log(_temp);

	result=g_handler->Log(_temp);
	//Throttling check should Deny now
	CPPUNIT_ASSERT(result == TRY_LATER);
}

//Write the configure scripts according to the test conditions
//or alternatively change the assertions according to the config file
void ServerTests::storeConfigCheck(){

	//StoreConf testConfig=g_handler->getConfig();

	StoreConf testConfig;
	string testFileName="/tmp/scribe.conf";
	testConfig.parseConfig(testFileName);
	int numstores=0;
	bool result;
    std::vector<StoreConfPtr> store_confs;
    testConfig.getAllStores(&store_confs);

    std::vector<StoreConfPtr>::iterator iter = store_confs.begin();
    result= g_handler->configureStore(*iter, &numstores);
    CPPUNIT_ASSERT(result == false);
    CPPUNIT_ASSERT(numstores==0);
    ++iter;

    result= g_handler->configureStore(*iter, &numstores);
    CPPUNIT_ASSERT(result == true);
    CPPUNIT_ASSERT(numstores==1);
    ++iter;

    result= g_handler->configureStore(*iter, &numstores);
    CPPUNIT_ASSERT(result == false);
    CPPUNIT_ASSERT(numstores==1);
}

void ServerTests::newCategoryCheck(){
	boost::shared_ptr<StoreList> testStoreList;
	CategoryMap::iterator cat_iter;

	string nonRegexCat="helloWorld";
	testStoreList=g_handler->createNewCategory(nonRegexCat);
	//Will not be added to categories_ map, as non_Model categories_ are added during initialization time.
	cat_iter = g_handler->categories_.find(nonRegexCat);
	CPPUNIT_ASSERT(cat_iter == g_handler->categories_.end());
	CPPUNIT_ASSERT(testStoreList == NULL);

	string regexCat="HelloWorld";
	testStoreList=g_handler->createNewCategory(regexCat);
	cat_iter = g_handler->categories_.find(regexCat);
	CPPUNIT_ASSERT(cat_iter != g_handler->categories_.end());

	//This filename is too long in most of the current OSes and should fail to create
	string invalidCat="";
	int largeNum=100;
	while(largeNum--) invalidCat+="Abcdef12";

	try{
	testStoreList=g_handler->createNewCategory(invalidCat);
	}catch(exception e){}

	cat_iter = g_handler->categories_.find(invalidCat);
	CPPUNIT_ASSERT(cat_iter == g_handler->categories_.end());

}

void ServerTests::deleteCategoryMapCheck(){

	CategoryMap::iterator cat_iter;

	CategoryMap catCopy=g_handler->categories_;
	g_handler->deleteCategoryMap(catCopy);
	cat_iter = catCopy.begin();
	CPPUNIT_ASSERT(cat_iter==catCopy.end());

	//Unit Tests for Regex Stores
	/*
	CategoryMap regexCatCopy=g_handler->category_regex;
	g_handler->deleteCategoryMap(regexCatCopy);
	cat_iter = regexCatCopy.begin();
	CPPUNIT_ASSERT(cat_iter==regexCatCopy.end());

	CategoryMap blacklistCatCopy=g_handler->category_regex_blacklist;
	g_handler->deleteCategoryMap(blacklistCatCopy);
	cat_iter = blacklistCatCopy.begin();
	CPPUNIT_ASSERT(cat_iter==blacklistCatCopy.end());
	*/
}
