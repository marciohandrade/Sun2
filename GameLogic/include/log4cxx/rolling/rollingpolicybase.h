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

#if !defined(_LOG4CXX_ROLLING_ROLLING_POLICY_BASE_H)
#define _LOG4CXX_ROLLING_ROLLING_POLICY_BASE_H

#include <log4cxx/helpers/object.h>
#include <log4cxx/logger.h>
#include <log4cxx/logmanager.h>
#include <log4cxx/rolling/rollingpolicy.h>
#include <log4cxx/pattern/patternconverter.h>
#include <log4cxx/pattern/formattinginfo.h>
#include <log4cxx/pattern/patternparser.h>

namespace log4cxx {
    namespace rolling {

        /**
         * Implements methods common to most, it not all, rolling
         * policies.
         *
         * @author Curt Arnold
         * @since 0.9.8
         */
        class LOG4CXX_EXPORT RollingPolicyBase :
           public virtual RollingPolicy,
           public virtual helpers::ObjectImpl {
        protected:
          BEGIN_LOG4CXX_CAST_MAP()
                  LOG4CXX_CAST_ENTRY(RollingPolicy)
                  LOG4CXX_CAST_ENTRY(spi::OptionHandler)
          END_LOG4CXX_CAST_MAP()


          private:
          /**
           * File name pattern converters.
           */
          std::vector<log4cxx::pattern::PatternConverterPtr> patternConverters;

          /**
           * File name field specifiers.
           */
          std::vector<log4cxx::pattern::FormattingInfoPtr> patternFields;

          /**
           * File name pattern.
           */
          LogString fileNamePatternStr;


          public:
          RollingPolicyBase();
          virtual ~RollingPolicyBase();
          /**
           * {@inheritDoc}
           */
          virtual void activateOptions(log4cxx::helpers::Pool& p) = 0;
          virtual const log4cxx::pattern::PatternMap& getFormatSpecifiers() const = 0;

          virtual void setOption(const LogString& option,
               const LogString& value);

          /**
           * Set file name pattern.
           * @param fnp file name pattern.
           */
           void setFileNamePattern(const LogString& fnp);

           /**
            * Get file name pattern.
            * @return file name pattern.
            */
           LogString getFileNamePattern() const;


           protected:
           /**
            *   Parse file name pattern.
            */
           void parseFileNamePattern();

          /**
           * Format file name.
           *
           * @param obj object to be evaluted in formatting, may not be null.
           * @param buf string buffer to which formatted file name is appended, may not be null.
           */
          void formatFileName(log4cxx::helpers::ObjectPtr& obj,
             LogString& buf, log4cxx::helpers::Pool& p) const;

           log4cxx::pattern::PatternConverterPtr getIntegerPatternConverter() const;
           log4cxx::pattern::PatternConverterPtr getDatePatternConverter() const;


       };
    }
}


#endif
