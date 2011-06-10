/******************************************************************************
 *
 * package:     Log4Qt
 * file:        asyncappender.h
 * created:     February 2011
 * author:      Andreas Bacher
 *
 *
 * Copyright 2011 Andreas Bacher
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

#ifndef LOG4QT_ASYNCAPPENDER_H
#define LOG4QT_ASYNCAPPENDER_H

/******************************************************************************
 * Dependencies
 ******************************************************************************/

#include "appenderskeleton.h"
#include "helpers/appenderattachable.h"

#include <QtCore/QQueue>
#include <QtCore/QMutex>

/******************************************************************************
 * Declarations
 ******************************************************************************/


namespace Log4Qt
{

class Dispatcher;

/*!
 * \brief The class AsyncAppender lets users log events asynchronously.
 *
 * The AsyncAppender will collect the events sent to it and then dispatch them to all the
 * appenders that are attached to it. You can attach multiple appenders to an AsyncAppender.
 *
 * The AsyncAppender uses a separate thread to serve the events fromthe event loop.
 *
 * \note All the functions declared in this class are thread-safe.
 * &nbsp;
 * \note The ownership and lifetime of objects of this class are managed.
 *       See \ref Ownership "Object ownership" for more details.
 */
class LOG4QT_EXPORT  AsyncAppender : public AppenderSkeleton, public AppenderAttachable
{
Q_OBJECT

public:
	AsyncAppender(QObject *parent = 0);
	virtual ~AsyncAppender();

	virtual bool requiresLayout() const;

	virtual void activateOptions();
	virtual void close();
	void callAppenders(const LoggingEvent &rEvent) const;

	/*!
	 * Tests if all entry conditions for using append() in this class are
	 * met.
	 *
	 * If a conditions is not met, an error is logged and the function
	 * returns false. Otherwise the result of
	 * AppenderSkeleton::checkEntryConditions() is returned.
	 *
	 * The checked conditions are:
	 * - dispatcher thread running (APPENDER_ASNC_DISPATCHER_NOT_RUNNING)
	 *
	 * The function is called as part of the checkEntryConditions() chain
	 * started by AppenderSkeleton::doAppend().
	 *
	 * \sa AppenderSkeleton::doAppend(),
	 *     AppenderSkeleton::checkEntryConditions()
	 */
	virtual bool checkEntryConditions() const;

protected:
	virtual void append(const LoggingEvent &rEvent);

#ifndef QT_NO_DEBUG_STREAM
	/*!
	 * Writes all object member variables to the given debug stream
	 * \a rDebug and returns the stream.
	 *
	 * <tt>
	 * %AsyncAppender(name:"WA" encoding:"" immediateFlush:true
	 *                 isactive:false isclosed:false layout:"TTCC"
	 *                 referencecount:1 threshold:"NULL"
	 *                 writer:0x0)
	 * </tt>
	 * \sa QDebug, operator<<(QDebug debug, const LogObject &rLogObject	)
	 */
		virtual QDebug debug(QDebug &rDebug) const;
#endif // QT_NO_DEBUG_STREAM

private:
    Q_DISABLE_COPY(AsyncAppender)

    //! Event dispatcher trhead
    QThread       *mpThread;
    Dispatcher    *mpDispatcher;
};


/**************************************************************************
 * Operators, Helper
 **************************************************************************/


/**************************************************************************
 * Inline
 **************************************************************************/


} // namespace Log4Qt


// Q_DECLARE_TYPEINFO(Log4Qt::AsyncAppender, Q_COMPLEX_TYPE); // Use default


#endif // LOG4QT_AsyncAppender_H
