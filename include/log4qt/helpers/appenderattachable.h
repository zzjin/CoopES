/******************************************************************************
 *
 * package:     Log4Qt
 * file:        appenderattachable.h
 * created:     December 2010
 * author:      Andreas Bacher
 *
 *
 * Copyright 2010 Andreas Bacher
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#ifndef LOG4QT_APPENDERATTACHABLE_H
#define LOG4QT_APPENDERATTACHABLE_H


/******************************************************************************
 * Dependencies
 ******************************************************************************/
#include "../log4qt.h"
#include "logobjectptr.h"

#include <QtCore/QList>
#include <QtCore/QReadWriteLock>

/******************************************************************************
 * Declarations
 ******************************************************************************/

namespace Log4Qt
{

class Appender;

/*!
 * \brief Implementation for attaching appenders to objects
 */
class LOG4QT_EXPORT AppenderAttachable
{

public:
    AppenderAttachable();
    virtual ~AppenderAttachable() {}

    /*!
     * Add an appender.
     */
    virtual void addAppender(Appender *pAppender);

    /*!
     * Get all previously added appenders as an Enumeration.
     */
    virtual QList<Appender *> appenders() const;

    /*!
     * Get an appender by name.
     */
    virtual Appender *appender(const QString &rName) const;

    /*!
     Returns <code>true</code> if the specified appender is in the
     list of attached appenders, <code>false</code> otherwise.
    */
    virtual bool isAttached(Appender *pAppender) const;

    /*!
     * Removes all appenders that have been previously added from this
     * Logger.
     *
     * To allow configurators to collect events during the configuration
     * process ListAppenders with the configuratorList property set, will
     * not be removed.
     *
     * \sa LisAppender::setConfiguratorList()
     */
    virtual void removeAllAppenders();

    /*!
     * Remove the appender passed as parameter from the list of appenders.
     */
    virtual void removeAppender(Appender *pAppender);

    /*!
     * Remove the appender with the name passed as parameter from the
     * list of appenders.
     */
    virtual void removeAppender(const QString &rName);

protected:
    QList< LogObjectPtr<Appender> > mAppenders;
    mutable QReadWriteLock mAppenderGuard;
};

/**************************************************************************
 * Operators, Helper
 **************************************************************************/


/**************************************************************************
 * Inline
 **************************************************************************/

} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::AppenderAttachable, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_APPENDERATTACHABLE_H
