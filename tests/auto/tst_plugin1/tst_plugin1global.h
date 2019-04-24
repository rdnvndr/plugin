#include <QtCore/QtGlobal>

#if defined(TST_PLUGIN1_LIBRARY)
#  define TST_PLUGIN1LIB Q_DECL_EXPORT
#else
#  define TST_PLUGIN1LIB Q_DECL_IMPORT
#endif

