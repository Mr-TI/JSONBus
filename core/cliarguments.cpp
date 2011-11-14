/*
    Copyright (c) 2012, Emeric Verschuur <emericv@gmail.com>
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
        * Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
        * Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
        * Neither the name of the <organization> nor the
        names of its contributors may be used to endorse or promote products
        derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY Emeric Verschuur <emericv@gmail.com> ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL Emeric Verschuur <emericv@gmail.com> BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <cliarguments.h>
#include <common.h>
#include <QStringList>
#include <QRegExp>

namespace jsonbus {

typedef QPair<QString, QString> LineElement;

CliArguments::CliArguments() {
}

CliArguments::~CliArguments() {
}

const QString &CliArguments::getValue(const QString &name) const {
  auto it = m_arguments.find(name);
  if (it == m_arguments.end()) {
    throw CliArgumentsException(tr("The parameter % is not definied").arg(name));
  }
  if (it.value().value == QString::null) {
    throw CliArgumentsException(tr("The parameter % has no value").arg(name));
  }
  return it.value().value;
}

bool CliArguments::isEnabled(const QString &name) const {
  auto it = m_arguments.find(name);
  if (it == m_arguments.end()) {
    throw CliArgumentsException(tr("The parameter % is not definied").arg(name));
  }
  if (it.value().value != QString::null) {
    throw CliArgumentsException(tr("The parameter % has a value").arg(name));
  }
  return it.value().enabled;
}

void CliArguments::define(const QString &name, char shortTag, const QString &description, const QString &value) {
  if (m_arguments.find(name) != m_arguments.end()) {
    throw CliArgumentsException(tr("The argument % is already definied").arg(name));
  }
  m_arguments.insert(name, Element(name, shortTag, description, value));
  if (shortTag) {
    m_shortTagToName[shortTag] = name;
  }
}

void CliArguments::parse(const QStringList &argList) {
  QString argName = QString::null, argValue;
  QRegExp patternArg("^--([\\w_-]+)(=(.*))?$");
  QRegExp patternShortArg("^-([a-zA-Z]+)(.*)$");
  for (auto it = ++argList.begin(); it != argList.end(); it++) {
    if (patternArg.indexIn(*it) != -1) {
      argName = patternArg.cap(1);
      auto eltIt = m_arguments.find(argName);
      if (eltIt == m_arguments.end()) {
        throw CliArgumentsException(tr("Invalid argument --%1").arg(argName));
      }
      if (eltIt.value().value != QString::null) {
        if (!patternArg.cap(2).isEmpty()) {
          eltIt.value().value = patternArg.cap(3);
        } else {
          it++;
          if (it == argList.end()) {
            throw CliArgumentsException(tr("The argument --%1 require a value").arg(argName));
          }
          eltIt.value().value = *it;
        }
      } else {
        eltIt.value().enabled = true;
      }
    } else if (patternShortArg.indexIn(*it) != -1) {
      QString sargs = patternShortArg.cap(1);
      for (int i = 0; i < sargs.length(); i++) {
        auto argIt = m_shortTagToName.find(sargs[i].toAscii());
        if (argIt == m_shortTagToName.end()) {
          throw CliArgumentsException(tr("Invalid argument -%1").arg(sargs[i]));
        }
        Element &elt = m_arguments[*argIt];
        if (elt.value != QString::null) {
          if (i == (sargs.length() - 1)) {
            if (!patternShortArg.cap(2).isEmpty()) {
              elt.value = patternShortArg.cap(2);
              break;
            } else {
              it++;
              if (it != argList.end()) {
                elt.value = *it;
                break;
              }
            }
          }
          throw CliArgumentsException(tr("The argument -%1 require a value").arg(sargs[i]));
        } else {
          elt.enabled = true;
        }
      }
    } else {
      throw CliArgumentsException(tr("Unexpected argument %1").arg(*it));
    }
  }
}

void CliArguments::displayUseInstructions() {
  QList<LineElement> list;
  QString argName;
  QString argValue;
  int maxlen = 0;
  for (auto it = m_arguments.begin(); it != m_arguments.end(); it++) {
    argName = "";
    argValue = it.value().value;
    if (it.value().shortTag) {
      argName = " -" + QString(it.value().shortTag) + (argValue != QString::null ? " <" + tr("value") + "> or" : " or");
    }
    argName += " --" + it.value().name + (argValue != QString::null ? "=<" + tr("value") + ">" : "");
    maxlen = qMax(maxlen, argName.length());
    list.append(LineElement(argName, it.value().description + (argValue != QString::null ? " (" + tr("value: ") + argValue + ")" : "")));
  }
  cout << tr("Use:") << endl;
  foreach(LineElement pair, list) {
    cout << pair.first << QString(maxlen - pair.first.length() + 4, ' ') << pair.second << endl;
  }
}

}
