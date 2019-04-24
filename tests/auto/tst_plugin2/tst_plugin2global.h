#include <QtCore/QtGlobal>

#if defined(TST_PLUGIN2_LIBRARY)
#  define TST_PLUGIN2LIB Q_DECL_EXPORT
#else
#  define TST_PLUGIN2LIB Q_DECL_IMPORT
#endif

