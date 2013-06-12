#include "cppnamespace.h"
#include "cppclass.h"
#include "cppfield.h"
#include "cppmethod.h"
#include "cppenum.h"
#include "cppoperator.h"

#include "../util.h"

#include "cpgf/gassert.h"


CppNamespace::CppNamespace()
	: super()
{
}

CppNamespace::~CppNamespace()
{
}

void CppNamespace::addItem(CppItem * item)
{
	this->doAddItem(item);
	item->setParent(this);
}

void CppNamespace::doAddItem(CppItem * item)
{
	switch(item->getCategory()) {
		case icNamespace:
			this->namespaceList.push_back(static_cast<CppNamespace *>(item));
			break;

		case icClass:
			this->classList.push_back(static_cast<CppClass *>(item));
			break;

		case icField:
			this->fieldList.push_back(static_cast<CppField *>(item));
			break;
			
		case icMethod:
			this->methodList.push_back(dynamic_cast<CppMethod *>(item));
			break;

		case icEnum:
			this->enumList.push_back(static_cast<CppEnum *>(item));
			break;

		case icOperator:
			this->operatorList.push_back(dynamic_cast<CppOperator *>(item));
			break;

		default:
			GASSERT(false);
			break;
	}
}

template <typename T>
void dumpList(T & itemList, std::ostream & os, int level)
{
	for(typename T::iterator it = itemList.begin(); it != itemList.end(); ++it) {
		(*it)->dump(os, level);
	}
}

void CppNamespace::dump(std::ostream & os, int level)
{
	int newLevel = level + 1;

	dumpList(this->namespaceList, os, newLevel);
	dumpList(this->classList, os, newLevel);
	dumpList(this->fieldList, os, newLevel);
	dumpList(this->methodList, os, newLevel);
	dumpList(this->enumList, os, newLevel);
	dumpList(this->operatorList, os, newLevel);
}