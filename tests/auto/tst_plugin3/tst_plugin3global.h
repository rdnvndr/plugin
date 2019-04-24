#include <QtCore/QtGlobal>

#if defined(TST_PLUGIN3_LIBRARY)
#  define TST_PLUGIN3LIB Q_DECL_EXPORT
#else
#  define TST_PLUGIN3LIB Q_DECL_IMPORT
#endif

