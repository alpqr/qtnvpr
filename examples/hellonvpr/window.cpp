/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the QtNVPR module
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "window.h"
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QDebug>

Window::Window()
    : prog(nullptr)
{
    setFormat(QNvPathRendering::format());
}

Window::~Window()
{
    delete prog;
}

void Window::initializeGL()
{
    if (!nvpr.create())
        qFatal("NVPR init failed");

    QOpenGLContext *ctx = QOpenGLContext::currentContext();
    qDebug() << ctx->format();

    prog = new QOpenGLShaderProgram;

    prog->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                  "uniform vec4 color;\n"
                                  "void main() {\n"
                                  "  gl_FragColor = color;\n"
                                  "}\n"
                                  );
    prog->link();
    progColorLoc = prog->uniformLocation("color");
}

void Window::resizeGL(int, int)
{
}

GLuint pathObj = 42;

void Window::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();

    // from nvpr_basic.c in NVprSDK
    f->glClearStencil(0);
    f->glClearColor(0,0,0,0);
    f->glStencilMask(~0);
    f->glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    prog->bind();

    static const GLubyte pathCommands[10] =
      { GL_MOVE_TO_NV, GL_LINE_TO_NV, GL_LINE_TO_NV, GL_LINE_TO_NV,
        GL_LINE_TO_NV, GL_CLOSE_PATH_NV,
        'M', 'C', 'C', 'Z' };  // character aliases
    static const GLshort pathCoords[12][2] =
      { {100, 180}, {40, 10}, {190, 120}, {10, 120}, {160, 10},
        {300,300}, {100,400}, {100,200}, {300,100},
        {500,200}, {500,400}, {300,300} };
    nvpr.pathCommands(pathObj, 10, pathCommands, 24, GL_SHORT, pathCoords);

    nvpr.pathParameteri(pathObj, GL_PATH_JOIN_STYLE_NV, GL_ROUND_NV);
    nvpr.pathParameterf(pathObj, GL_PATH_STROKE_WIDTH_NV, 6.5);

    nvpr.matrixLoadIdentity(GL_PROJECTION);
    nvpr.matrixLoadIdentity(GL_MODELVIEW);
    nvpr.matrixOrtho(GL_MODELVIEW, 0, 500, 0, 400, -1, 1);

    nvpr.stencilFillPath(pathObj, GL_COUNT_UP_NV, 0x1F);
    f->glEnable(GL_STENCIL_TEST);
    f->glStencilFunc(GL_NOTEQUAL, 0, 0x1F);
    f->glStencilOp(GL_KEEP, GL_KEEP, GL_ZERO);

    prog->setUniformValue(progColorLoc, QColor(Qt::green));
    nvpr.coverFillPath(pathObj, GL_BOUNDING_BOX_NV);

    nvpr.stencilStrokePath(pathObj, 0x1, ~0);

    prog->setUniformValue(progColorLoc, QColor(Qt::yellow));
    nvpr.coverStrokePath(pathObj, GL_CONVEX_HULL_NV);
}
