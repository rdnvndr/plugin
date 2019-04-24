#include <QtCore/QtGlobal>

#if defined(TST_PLUGIN4_LIBRARY)
#  define TST_PLUGIN4LIB Q_DECL_EXPORT
#else
#  define TST_PLUGIN4LIB Q_DECL_IMPORT
#endif

