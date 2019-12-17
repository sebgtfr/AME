/**
* \file				import.hpp
* \author			Sébastien Le Maire
* \date				13/10/2019
*/

#ifndef AME_IMPORT

# if defined (_WIN32) || defined (_WIN64) || defined(__CYGWIN__)
#  ifdef AME_EXPORT
#   define AME_IMPORT __declspec(dllexport)
#  else
#   define AME_IMPORT __declspec(dllimport)
#  endif
#  pragma warning(disable: 4251)
# else
#  define AME_IMPORT
# endif

#endif /* !AME_IMPORT */