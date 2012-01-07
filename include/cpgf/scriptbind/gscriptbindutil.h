#ifndef __GSCRIPTBINDUTIL_H
#define __GSCRIPTBINDUTIL_H


#include "cpgf/scriptbind/gscriptbind.h"
#include "cpgf/scriptbind/gscriptbindapi.h"

namespace cpgf {


#define DEF_LOAD_PARAM_HELPER(N, unused) params[N] = &GPP_CONCAT(p, N);
#define DEF_LOAD_PARAM(N) \
	const GMetaVariant * params[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER, GPP_EMPTY())

#define DEF_LOAD_PARAM_HELPER_API(N, unused) params[N] = GPP_CONCAT(p, N).getData();
#define DEF_LOAD_PARAM_API(N) \
	GMetaVarData params[N == 0 ? 1 : N]; \
	GPP_REPEAT_3(N, DEF_LOAD_PARAM_HELPER_API, GPP_EMPTY())

#define DEF_CALL_HELPER(N, unused) \
	inline GMetaVariant invokeScriptFunction(GScriptObject * scriptObject, const char * name GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GMetaVariant & p)) { \
		DEF_LOAD_PARAM(N) \
		return scriptObject->invokeIndirectly(name, params, N); \
	} \
	inline GMetaVariant invokeScriptFunction(IScriptObject * scriptObject, const char * name GPP_COMMA_IF(N) GPP_REPEAT_PARAMS(N, const GMetaVariant & p)) { \
		DEF_LOAD_PARAM_API(N) \
		GMetaVarData result; \
		scriptObject->invoke(&result, name, params, N); \
		return GMetaVariant(result); \
	}

GPP_REPEAT_2(REF_MAX_ARITY, DEF_CALL_HELPER, GPP_EMPTY())

#undef DEF_CALL_HELPER
#undef DEF_LOAD_PARAM
#undef DEF_LOAD_PARAM_HELPER
#undef DEF_LOAD_PARAM_API
#undef DEF_LOAD_PARAM_HELPER_API

IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject, bool freeObject);
IScriptObject * scriptObjectToInterface(GScriptObject * scriptObject);

void injectMetaClassToScript(IScriptObject * scriptObject, IMetaClass * metaClass, void * instance);


} // namespace cpgf




#endif

