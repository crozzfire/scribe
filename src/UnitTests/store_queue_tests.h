#ifndef STOREQUEUETESTS_H_
#define STOREQUEUETESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Common.h"
#include "Conf.h"

//Exposing the class hack
#define protected public
#define private   public
#include "StoreQueue.h"
#include "ScribeServer.h"
#undef protected
#undef private

class StoreQueueTests : public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE(StoreQueueTests);
	CPPUNIT_TEST(addMessageCheck);
	CPPUNIT_TEST(getCategoryHandledCheck);
	CPPUNIT_TEST(getStatusCheck);
	CPPUNIT_TEST(getBaseTypeCheck);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

protected:
	void addMessageCheck();
	void getCategoryHandledCheck();
	void getStatusCheck();
	void getBaseTypeCheck();
};
#endif /* STOREQUEUETESTS_H_ */
