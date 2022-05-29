#ifndef SMTPEXPORTS_H
#define SMTPEXPORTS_H

#ifdef SMTP_BUILD
#define SMTP_EXPORT Q_DECL_EXPORT
#elif SMTP_USE
#define SMTP_EXPORT Q_DECL_IMPORT
#else
#define SMTP_EXPORT
#endif

#endif // SMTPEXPORTS_H
