#ifndef SMTPMIME_GLOBAL_H
#define SMTPMIME_GLOBAL_H

#if defined(BUILD_SHARED_LIBS)
#  ifdef SMTP_MIME_LIBRARY
#    define SMTP_MIME_EXPORT Q_DECL_EXPORT
#  else
#    define SMTP_MIME_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define SMTP_MIME_EXPORT
#endif

#endif // SMTPMIME_GLOBAL_H
