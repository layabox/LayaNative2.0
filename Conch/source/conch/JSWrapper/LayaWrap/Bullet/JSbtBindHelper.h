#ifndef __JSbtBindHelper_H__
#define __JSbtBindHelper_H__

#define JSbt_Bind_Global_Func(func, ...)  \
	JSP_ADD_GLOBAL_FUNCTION(func, func, __VA_ARGS__)


#endif // !__JSbtBindHelper_H__
