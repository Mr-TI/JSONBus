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

#include <argsparser.h>
#include <common.h>
#include <QStringList>
#include <QRegExp>

namespace jsonbus {
    
typedef QPair<QString, QString> LineElement;

ArgsParser::ArgsParser() {
}

ArgsParser::~ArgsParser() {
}

void ArgsParser::define(const QString &name, char shortTag, const QString &description, const QString &value) {
    if (m_arguments.find(name) != m_arguments.end()) {
        throw new ArgsParserException(tr("The argument % is already definied").arg(name));
    }
    m_arguments.insert(name, Element(name, shortTag, description, value));
    if (shortTag) {
        m_shortTagToName[shortTag] = name;
    }
}

void ArgsParser::parse(const QStringList &argList) {
    QString argName = QString::null, argValue;
    QRegExp patternArg("^--([\\w_-]+)(=(.*))$");
    QRegExp patternShortArg("^-([a-zA-Z]+)(\\d*)$");
    for(auto it = argList.begin(); it != argList.end(); it++) {
        if (patternArg.indexIn(*it)) {
            argName = patternArg.cap(1);
            auto elt
            if (!patternArg.cap(2).isEmpty()) {
                argValue = patternArg.cap(3);
            } else {
                argValue = *(++it)
            }
        }
    }
}

void ArgsParser::displayUseInstructions() {
    QList<LineElement> list;
    QString argName;
    QString argValue;
    int maxlen = 0;
    for (auto it = m_arguments.begin(); it != m_arguments.end(); it++) {
        argName = "";
        argValue = it.value().value;
        if (it.value().shortTag) {
            argName = " -" + QString(it.value().shortTag) + (argValue != QString::null ? " <" + tr("value") + "> or": " or");
        }
        argName += " --" + it.value().name + (argValue != QString::null ? "=<" + tr("value") + ">": "");
        maxlen = qMax(maxlen, argName.length());
        list.append(LineElement(argName,it.value().description + (argValue != QString::null ? "(" + tr("value: ") + argValue + ")": "")));
    }
    cout << tr("Use:") << endl;
    foreach(LineElement pair, list) {
        cout << pair.first << QString(maxlen - pair.first.length() + 4, ' ') << pair.second << endl;
    }
}

}
