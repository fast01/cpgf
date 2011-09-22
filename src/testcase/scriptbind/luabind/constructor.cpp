#include "test_common.h"


namespace {


void doTest()
{
	using namespace cpgf;
	using namespace testscript;

	TestLuaContext context;
	context.doString("print(\"Constructor\")");

	GMetaScopedPointer<IMetaClass> metaClass(context.getService()->findClassByName("testscript::TestObject"));
	testCheckAssert(metaClass);
	
	context.getBinding()->bindClass("TestObject", metaClass.get());

	context.doString(""
		LINE(a = TestObject())
		LINE(assert(a.value == Magic1))
		
		LINE(b = TestObject(a))
		LINE(assert(b.value == Magic2))
	);
}


gTestCase(doTest);



}