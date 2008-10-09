/****************************************************************************
**
** Copyright (C) Qxt Foundation. Some rights reserved.
**
** This file is part of the QxtCore module of the Qxt library.
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the Common Public License, version 1.0, as published by
** IBM.
**
** This file is provided "AS IS", without WARRANTIES OR CONDITIONS OF ANY
** KIND, EITHER EXPRESS OR IMPLIED INCLUDING, WITHOUT LIMITATION, ANY
** WARRANTIES OR CONDITIONS OF TITLE, NON-INFRINGEMENT, MERCHANTABILITY OR
** FITNESS FOR A PARTICULAR PURPOSE.
**
** You should have received a copy of the CPL along with this file.
** See the LICENSE file and the cpl1.0.txt file included with the source
** distribution for more information. If you did not receive a copy of the
** license, contact the Qxt Foundation.
**
** <http://libqxt.org>  <foundation@libqxt.org>
**
****************************************************************************/

#ifndef QXTGLOBAL_H
#define QXTGLOBAL_H

#define QXT_VERSION 0x000400
#define QXT_VERSION_STR "0.4.0"

//--------------------------global macros------------------------------

#ifndef QXT_NO_MACROS

#endif // QXT_NO_MACROS

//--------------------------export macros------------------------------

#define QXT_DLLEXPORT DO_NOT_USE_THIS_ANYMORE

#if defined(QXT_WINDOWS)

#    if defined(BUILD_QXT_CORE)
#        define QXT_CORE_EXPORT __declspec(dllexport)
#    else
#        define QXT_CORE_EXPORT __declspec(dllimport)
#    endif // BUILD_QXT_CORE

#    if defined(BUILD_QXT_GUI)
#        define QXT_GUI_EXPORT __declspec(dllexport)
#    else
#        define QXT_GUI_EXPORT __declspec(dllimport)
#    endif // BUILD_QXT_GUI

#    if defined(BUILD_QXT_NETWORK)
#        define QXT_NETWORK_EXPORT __declspec(dllexport)
#    else
#        define QXT_NETWORK_EXPORT __declspec(dllimport)
#    endif // BUILD_QXT_NETWORK

#    if defined(BUILD_QXT_SQL)
#        define QXT_SQL_EXPORT __declspec(dllexport)
#    else
#        define QXT_SQL_EXPORT __declspec(dllimport)
#    endif // BUILD_QXT_SQL

#    if defined(BUILD_QXT_WEB)
#        define QXT_WEB_EXPORT __declspec(dllexport)
#    else
#        define QXT_WEB_EXPORT __declspec(dllimport)
#    endif // BUILD_QXT_WEB

#    if defined(BUILD_QXT_CRYPTO)
#        define QXT_CRYPTO_EXPORT __declspec(dllexport)
#    else
#        define QXT_CRYPTO_EXPORT __declspec(dllimport)
#    endif // BUILD_QXT_CRYPTO

#    if defined(BUILD_QXT_BERKELEY)
#        define QXT_BERKELEY_EXPORT __declspec(dllexport)
#    else
#        define QXT_BERKELEY_EXPORT __declspec(dllimport)
#    endif // BUILD_QXT_BERKELEY

#else // !QXT_WINDOWS

#    define QXT_CORE_EXPORT
#    define QXT_GUI_EXPORT
#    define QXT_MEDIA_EXPORT
#    define QXT_NETWORK_EXPORT
#    define QXT_SQL_EXPORT
#    define QXT_WEB_EXPORT
#    define QXT_CRYPTO_EXPORT
#    define QXT_BERKELEY_EXPORT

#endif // QXT_WINDOWS

#if defined BUILD_QXT_CORE || defined BUILD_QXT_GUI || defined BUILD_QXT_MEDIA || defined  BUILD_QXT_SQL || defined BUILD_QXT_NETWORK || defined BUILD_QXT_KIT || defined BUILD_QXT_WEB || defined BUILD_QXT_CRYPTO || defined BUILD_QXT_BERKELEY
#   define BUILD_QXT
#endif

QXT_CORE_EXPORT const char* qxtVersion();

#endif // QXT_GLOBAL
