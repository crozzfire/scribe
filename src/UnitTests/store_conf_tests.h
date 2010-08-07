#ifndef STORE_CONF_TESTS_H_
#define STORE_CONF_TESTS_H_

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>
#include "Common.h"
#include "Conf.h"

class StoreConfTests: public CPPUNIT_NS::TestFixture {

		CPPUNIT_TEST_SUITE(StoreConfTests);
		CPPUNIT_TEST(getIntCheck);
		CPPUNIT_TEST(getAllStoresCheck);

		CPPUNIT_TEST(getUnsignedCheck);
		CPPUNIT_TEST(getUint64Check);
		CPPUNIT_TEST(getFloatCheck);
		CPPUNIT_TEST(getStringCheck);
		CPPUNIT_TEST(parseConfigCheck);

		CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

protected:
	void getAllStoresCheck();
	void getStoreCheck();
	void getIntCheck();
	void getUnsignedCheck();
	void getUint64Check();
	void getFloatCheck();
	void getStringCheck();
	void parseConfigCheck();
};

#endif /* STORE_CONF_TESTS_H_ */
