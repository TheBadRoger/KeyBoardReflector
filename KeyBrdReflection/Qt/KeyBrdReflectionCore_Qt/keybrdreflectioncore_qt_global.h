#pragma once

#include <QtCore/qglobal.h>
#include <string>
#include <Windows.h>

#ifndef BUILD_STATIC
# if defined(KEYBRDREFLECTIONCORE_QT_LIB)
#  define KEYBRDREFLECTIONCORE_QT_EXPORT Q_DECL_EXPORT
# else
#  define KEYBRDREFLECTIONCORE_QT_EXPORT Q_DECL_IMPORT
# endif
#else
# define KEYBRDREFLECTIONCORE_QT_EXPORT
#endif
