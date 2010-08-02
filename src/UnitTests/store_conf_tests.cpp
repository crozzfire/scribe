#include <boost/algorithm/string.hpp>
#include "store_conf_tests.h"
#include "common.h"
#include "conf.h"
#include "scribe_server.h"

using namespace boost;
using namespace std;

CPPUNIT_TEST_SUITE_REGISTRATION(StoreConfTests);
void StoreConfTests::setUp(){

}

void StoreConfTests::tearDown(){

}

//Checking for runtime exceptions
void StoreConfTests::parseConfigCheck(){
StoreConf conf;

//Testing Invalid Location
string file="/invalid/conf";
bool passed=false;

try{
	conf.parseConfig(file);
}catch(runtime_error re){
	try{
		//Valid Location
		string file="/tmp/conf";
		conf.parseConfig(file);
		passed=true;

	}catch(runtime_error re){
		//Should not generally throw a runtime exception here
		passed=false;
	}
}

CPPUNIT_ASSERT(passed==true);
}

void StoreConfTests::getIntCheck(){
	string test_int="test_int";
	string number;
	long int _testInt;
	bool result = (g_Handler->getConfig()).getInt(test_int,_testInt);
	//Testing Overflow
	if(result)
	{
		stringstream _temp;
		_temp<<_testInt;

		(g_Handler->getConfig()).getString(test_int,number);
		CPPUNIT_ASSERT(number==_temp.str());
	}

}

void StoreConfTests::getUnsignedCheck(){
	string test_unsigned="test_unsigned";
	string number;
	unsigned long int _testInt;
	bool result = (g_Handler->getConfig()).getUnsigned(test_unsigned,_testInt);
	//Testing Overflow
	if(result)
	{
		stringstream _temp;
		_temp<<_testInt;

		(g_Handler->getConfig()).getString(test_unsigned,number);
		CPPUNIT_ASSERT(number==_temp.str());
	}

}

void StoreConfTests::getUnsignedLongLongCheck(){
	string test_ull="test_ull";
	string number;
	unsigned long long int _testULL;
	bool result = (g_Handler->getConfig()).getUnsignedLongLong(test_ull,_testULL);
	//Testing Overflow
	if(result)
	{
		stringstream _temp;
		_temp<<_testULL;

		(g_Handler->getConfig()).getString(test_ull,number);
		CPPUNIT_ASSERT(number==_temp.str());
	}

}

void StoreConfTests::getFloatCheck(){
	string test_float="test_float";
	string number;
	float _testFloat;
	bool result = (g_Handler->getConfig()).getFloat(test_float,_testFloat);
	//Testing Precision Overflow
	if(result)
	{
		stringstream _temp;
		_temp<<_testFloat;
		(g_Handler->getConfig()).getString(test_float,number);
		CPPUNIT_ASSERT(number==_temp.str());
	}
}

void StoreConfTests::getStringCheck(){
	string test_string="test_string"; //set test_string="test1234" in the test conf file
	string _testString;
	bool result = (g_Handler->getConfig()).getString(test_string,_testString);
	if(result){
		CPPUNIT_ASSERT(_testString=="test1234");
	}
}

void StoreConfTests::getAllStoresCheck(){
	StoreConf testConf=g_Handler->getConfig();
	vector<pStoreConf> storeList;
	testConf.getAllStores(storeList);
	CPPUNIT_ASSERT(storeList.size()>0);
}
