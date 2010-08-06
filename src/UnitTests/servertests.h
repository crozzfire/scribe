#ifndef SERVERTESTS_H_
#define SERVERTESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Common.h"

//Exposing the class hack
#define protected public
#define private   public
#include "ScribeServer.h"
#undef protected
#undef private

class ServerTests : public CPPUNIT_NS::TestFixture {

	CPPUNIT_TEST_SUITE(ServerTests);
	CPPUNIT_TEST(logResultCheck);
	CPPUNIT_TEST(throttleCheck);
	CPPUNIT_TEST(storeConfigCheck);
	CPPUNIT_TEST(newCategoryCheck);
	CPPUNIT_TEST(deleteCategoryMapCheck);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

protected:
	void logResultCheck();
	void throttleCheck();
	void storeConfigCheck();
	void newCategoryCheck();
	void deleteCategoryMapCheck();

};
#endif /* SERVERTESTS_H_ */
