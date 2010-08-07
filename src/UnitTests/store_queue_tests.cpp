#include "Common.h"
#include "store_queue_tests.h"

using namespace std;
using namespace boost;
using namespace scribe::thrift;
using namespace scribe;

CPPUNIT_TEST_SUITE_REGISTRATION(StoreQueueTests);

void StoreQueueTests::setUp(){
	g_handler->reinitialize();
}

void StoreQueueTests::tearDown(){
}

void StoreQueueTests::addMessageCheck(){
	string category="bar";
	boost::shared_ptr<StoreList> stores=g_handler->categories_[category];
	StoreList::iterator iter;

	for(iter=stores->begin();iter!=stores->end();++iter){
		unsigned long long oldSize;
		oldSize=(*iter)->getSize();

		boost::shared_ptr<LogEntry> entry(new LogEntry);
		entry->category=category;
		entry->message="hello";
		(*iter)->addMessage(entry);

		unsigned long long msgSize=entry->message.size();
		CPPUNIT_ASSERT((*iter)->getSize() == msgSize+oldSize);
		CPPUNIT_ASSERT(!((*iter)->getSize() <=0));

		boost::shared_ptr<LogEntry> _temp(new LogEntry);
		_temp=((*iter)->msgQueue_)->back();

		CPPUNIT_ASSERT(_temp->category == entry->category);
		CPPUNIT_ASSERT(_temp->message == entry->message);

		CPPUNIT_ASSERT(_temp->category != "");
		CPPUNIT_ASSERT(_temp->message != "");

	}
}

void StoreQueueTests::getCategoryHandledCheck(){
	string category="bar";
	StoreListPtr stores;
	StoreList::iterator iter;

	stores=g_handler->categories_[category];
	for(iter=stores->begin();iter!=stores->end();++iter){
		CPPUNIT_ASSERT((*iter)->getCategoryHandled() == category);
		CPPUNIT_ASSERT((*iter)->getCategoryHandled() != "");
	}

	category="MODEL_CAT";
	stores=g_handler->createNewCategory(category);
	for(iter=stores->begin();iter!=stores->end();++iter){
		CPPUNIT_ASSERT((*iter)->getCategoryHandled() == category);
		CPPUNIT_ASSERT((*iter)->getCategoryHandled() != "");
	}
}

void StoreQueueTests::getStatusCheck(){
	string category="bar";
	boost::shared_ptr<StoreList> stores;
	StoreList::iterator iter;

	stores=g_handler->categories_[category];
	iter=stores->begin();

	for(iter=stores->begin();iter!=stores->end();++iter){
	CPPUNIT_ASSERT((*iter)->getStatus()=="");
	}
}

//Best Results when used with a config file with all the supported stores
void StoreQueueTests::getBaseTypeCheck(){
	string category="bar";
	boost::shared_ptr<StoreList> stores;
	StoreList::iterator iter;

	stores=g_handler->categories_[category];
	iter=stores->begin();

	if(iter != stores->end())
	CPPUNIT_ASSERT((*iter)->getBaseType() == "file");

	if(++iter != stores->end())
	CPPUNIT_ASSERT((*iter)->getBaseType() == "file");

	//--More tests--
}
