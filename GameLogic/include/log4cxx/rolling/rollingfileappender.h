/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#if !defined(_LOG4CXX_ROLLING_ROLLING_FILE_APPENDER_H)
#define _LOG4CXX_ROLLING_ROLLING_FILE_APPENDER_H

#include <log4cxx/portability.h>
#include <log4cxx/spi/optionhandler.h>
#include <log4cxx/fileappender.h>
#include <log4cxx/rolling/triggeringpolicy.h>
#include <log4cxx/rolling/rollingpolicy.h>
#include <log4cxx/rolling/action.h>

namespace log4cxx {
    namespace rolling {


        /**
         * <code>RollingFileAppender</code> extends {@link FileAppender} to backup the log files
         * depending on {@link RollingPolicy} and {@link TriggeringPolicy}.
         * <p>
         * To be of any use, a <code>RollingFileAppender</code> instance must have both
         * a <code>RollingPolicy</code> and a <code>TriggeringPolicy</code> set up.
         * However, if its <code>RollingPolicy</code> also implements the
         * <code>TriggeringPolicy</code> interface, then only the former needs to be
         * set up. For example, {@link TimeBasedRollingPolicy} acts both as a
         * <code>RollingPolicy</code> and a <code>TriggeringPolicy</code>.
         *
         * <p><code>RollingFileAppender</code> can be configured programattically or
         * using {@link org.apache.log4j.joran.JoranConfigurator}. Here is a sample
         * configration file:

        <pre>&lt;?xml version="1.0" encoding="UTF-8" ?>
        &lt;!DOCTYPE log4j:configuration>

        &lt;log4j:configuration debug="true">

          &lt;appender name="ROLL" class="org.apache.log4j.rolling.RollingFileAppender">
            <b>&lt;rollingPolicy class="org.apache.log4j.rolling.TimeBasedRollingPolicy">
              &lt;param name="FileNamePattern" value="/wombat/foo.%d{yyyy-MM}.gz"/>
            &lt;/rollingPolicy></b>

            &lt;layout class="org.apache.log4j.PatternLayout">
              &lt;param name="ConversionPattern" value="%c{1} - %m%n"/>
            &lt;/layout>
          &lt;/appender>

          &lt;root">
            &lt;appender-ref ref="ROLL"/>
          &lt;/root>

        &lt;/log4j:configuration>
        </pre>

         *<p>This configuration file specifies a monthly rollover schedule including
         * automatic compression of the archived files. See
         * {@link TimeBasedRollingPolicy} for more details.
         *
         * @author Heinz Richter
         * @author Ceki G&uuml;lc&uuml;
         * @since  1.3
         * */
        class LOG4CXX_EXPORT RollingFileAppender : public FileAppender {
          DECLARE_LOG4CXX_OBJECT(RollingFileAppender)
          BEGIN_LOG4CXX_CAST_MAP()
                  LOG4CXX_CAST_ENTRY(RollingFileAppender)
                  LOG4CXX_CAST_ENTRY_CHAIN(FileAppender)
          END_LOG4CXX_CAST_MAP()

          /**
           * Triggering policy.
           */
          TriggeringPolicyPtr triggeringPolicy;

          /**
           * Rolling policy.
           */
          RollingPolicyPtr rollingPolicy;

          /**
           * Length of current active log file.
           */
          size_t fileLength;

          /**
           * Asynchronous action (like compression) from previous rollover.
           */
          ActionPtr lastRolloverAsyncAction;

        public:
          /**
           * The default constructor simply calls its {@link
           * FileAppender#FileAppender parents constructor}.
           * */
          RollingFileAppender();

          void activateOptions(log4cxx::helpers::Pool&);


          /**
             Implements the usual roll over behaviour.

             <p>If <code>MaxBackupIndex</code> is positive, then files
             {<code>File.1</code>, ..., <code>File.MaxBackupIndex -1</code>}
             are renamed to {<code>File.2</code>, ...,
             <code>File.MaxBackupIndex</code>}. Moreover, <code>File</code> is
             renamed <code>File.1</code> and closed. A new <code>File</code> is
             created to receive further log output.

             <p>If <code>MaxBackupIndex</code> is equal to zero, then the
             <code>File</code> is truncated with no backup files created.

           */
          bool rollover(log4cxx::helpers::Pool& p);

        protected:

        /**
         Actual writing occurs here.
        */
        virtual void subAppend(const spi::LoggingEventPtr& event, log4cxx::helpers::Pool& p);

        public:

          RollingPolicyPtr getRollingPolicy() const;

          TriggeringPolicyPtr getTriggeringPolicy() const;

          /**
           * Sets the rolling policy. In case the 'policy' argument also implements
           * {@link TriggeringPolicy}, then the triggering policy for this appender
           * is automatically set to be the policy argument.
           * @param policy
           */
          void setRollingPolicy(const RollingPolicyPtr& policy);

          void setTriggeringPolicy(const TriggeringPolicyPtr& policy);


          /**
            * Close appender.  Waits for any asynchronous file compression actions to be completed.
          */
          void close();

          protected:
          /**
             Returns an OutputStreamWriter when passed an OutputStream.  The
             encoding used will depend on the value of the
             <code>encoding</code> property.  If the encoding value is
             specified incorrectly the writer will be opened using the default
             system encoding (an error message will be printed to the loglog.
           @param os output stream, may not be null.
           @return new writer.
           */
          log4cxx::helpers::WriterPtr createWriter(log4cxx::helpers::OutputStreamPtr& os);

          public:



          /**
           * Get byte length of current active log file.
           * @return byte length of current active log file.
           */
          size_t getFileLength() const;

          /**
           * Increments estimated byte length of current active log file.
           * @param increment additional bytes written to log file.
           */
          void incrementFileLength(size_t increment);

        };

        typedef log4cxx::helpers::ObjectPtrT<RollingFileAppender> RollingFileAppenderPtr;

    }
}

#endif

