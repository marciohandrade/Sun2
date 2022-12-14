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

#ifndef _LOG4CXX_PATTERN_LITERAL_PATTERN_CONVERTER_H
#define _LOG4CXX_PATTERN_LITERAL_PATTERN_CONVERTER_H

#include <log4cxx/pattern/loggingeventpatternconverter.h>

namespace log4cxx {
  namespace pattern {

/**
 * Formats a string literal.
 *
 * @author Curt Arnold
 * @since 0.9.8
 *
 */
class LOG4CXX_EXPORT LiteralPatternConverter : public LoggingEventPatternConverter {
  /**
   * String literal.
   */
  const LogString literal;

  /**
   * Create a new instance.
   * @param literal string literal.
   */
  LiteralPatternConverter(const LogString& literal);


public:
   DECLARE_LOG4CXX_PATTERN(LiteralPatternConverter)
   BEGIN_LOG4CXX_CAST_MAP()
     LOG4CXX_CAST_ENTRY(LiteralPatternConverter)
     LOG4CXX_CAST_ENTRY_CHAIN(LoggingEventPatternConverter)
  END_LOG4CXX_CAST_MAP()

  static PatternConverterPtr newInstance(const LogString& literal);

  /**
   * {@inheritDoc}
   */
  void format(const log4cxx::spi::LoggingEventPtr& event,
     LogString& toAppendTo,
     log4cxx::helpers::Pool& p) const;

  /**
   * {@inheritDoc}
   */
  void format(const log4cxx::helpers::ObjectPtr& obj,
     LogString& toAppendTo,
     log4cxx::helpers::Pool& p) const;
};

  }
}
#endif

