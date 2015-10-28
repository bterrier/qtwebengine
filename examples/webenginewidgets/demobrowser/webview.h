/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file. Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QIcon>
#include <QWebEngineView>

QT_BEGIN_NAMESPACE
class QAuthenticator;
class QMouseEvent;
class QNetworkProxy;
class QNetworkReply;
class QSslError;
QT_END_NAMESPACE

class BrowserMainWindow;
class WebPage : public QWebEnginePage {
    Q_OBJECT

signals:
    void loadingUrl(const QUrl &url);

public:
    WebPage(QWebEngineProfile *profile, QObject *parent = 0);
    BrowserMainWindow *mainWindow();

protected:
    bool acceptNavigationRequest(const QUrl &url, NavigationType type, bool isMainFrame) Q_DECL_OVERRIDE;
    QWebEnginePage *createWindow(QWebEnginePage::WebWindowType type) Q_DECL_OVERRIDE;
#if !defined(QT_NO_UITOOLS)
    QObject *createPlugin(const QString &classId, const QUrl &url, const QStringList &paramNames, const QStringList &paramValues);
#endif
    virtual bool certificateError(const QWebEngineCertificateError &error) Q_DECL_OVERRIDE;
    virtual void javaScriptConsoleMessage(JavaScriptConsoleMessageLevel level, const QString& message, int lineNumber, const QString& sourceID)  Q_DECL_OVERRIDE;

private slots:
#if defined(QWEBENGINEPAGE_UNSUPPORTEDCONTENT)
    void handleUnsupportedContent(QNetworkReply *reply);
#endif
    void authenticationRequired(const QUrl &requestUrl, QAuthenticator *auth);
    void proxyAuthenticationRequired(const QUrl &requestUrl, QAuthenticator *auth, const QString &proxyHost);

private:
    friend class WebView;

    // set the webview mousepressedevent
    Qt::KeyboardModifiers m_keyboardModifiers;
    Qt::MouseButtons m_pressedButtons;
    QUrl m_loadingUrl;
};

class WebView : public QWebEngineView {
    Q_OBJECT

public:
    WebView(QWidget *parent = 0);
    WebPage *webPage() const { return m_page; }
    void setPage(WebPage *page);

    void loadUrl(const QUrl &url);
    QUrl url() const;
    QIcon icon() const;

    QString lastStatusBarText() const;
    inline int progress() const { return m_progress; }

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    void wheelEvent(QWheelEvent *event);

signals:
    void iconChanged();

private slots:
    void setProgress(int progress);
    void loadFinished(bool success);
    void setStatusBarText(const QString &string);
    void openLinkInNewTab();
    void onFeaturePermissionRequested(const QUrl &securityOrigin, QWebEnginePage::Feature);
    void onIconUrlChanged(const QUrl &url);
    void iconLoaded();

private:
    QString m_statusBarText;
    QUrl m_initialUrl;
    int m_progress;
    WebPage *m_page;
    QIcon m_icon;
    QNetworkReply *m_iconReply;
};

#endif
