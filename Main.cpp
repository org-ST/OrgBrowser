//===------- Main.cpp - Main File - C ------------------------------*-===//
//
// Copyright (C) 2025 Wdboyes13
// Part of orgBrowser, under the GNU General Public License v3.
// See LICENSE for license information.
// SPDX-License-Identifier: GPL-3.0-or-later
//
//===-----------------------------------------------------------------===//

#include <QtWidgets/QApplication>
#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWebEngineWidgets/QWebEngineProfile>
#include <QtWidgets/QWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <stdlib.h>
#include <QtCore/QString>
#include <filesystem>
#include <regex>

#ifndef __APPLE__RELEASE__
#define BACKIMG "resources/back.png"
#define FORWIMG "resources/forw.png"
#define RELOIMG "resources/relo.png"
#else 
#define BACKIMG "../Resources/back.png"
#define FORWIMG "../Resources/forw.png"
#define RELOIMG "../Resources/relo.png"
#endif

bool endsWithTLD(const std::string& text) {
    std::regex tldRegex(R"(\.[a-z]{2,}$)", std::regex::icase);
    return std::regex_search(text, tldRegex);
}

int main(int argc, char* argv[]){
    qDebug() << "Initializing QApplication";

    QApplication app(argc, argv);
    QWidget window;
    QVBoxLayout* layout = new QVBoxLayout(&window);
    layout->setContentsMargins(0, 0, 0, 0); // remove all outer padding
    layout->setSpacing(0); // remove spacing between widgets
    QToolBar* ToolBar = new QToolBar();

    qDebug() << "Making Search Bar";
    QLineEdit* searchBar = new QLineEdit();
    searchBar->setPlaceholderText("Enter URL");


    qDebug() << "Making Back Button";
    QToolButton* backbutton = new QToolButton();
    QPixmap backpixmap(BACKIMG);
    QIcon backbuttonIcon(backpixmap);
    backbutton->setIcon(backbuttonIcon);
    backbutton->setIconSize(backpixmap.rect().size());
    backbutton->resize(backpixmap.rect().size());

    qDebug() << "Making Forward Button";
    QToolButton* forbutton = new QToolButton();
    QPixmap forpixmap(FORWIMG);
    QIcon forbuttonIcon(forpixmap);
    forbutton->setIcon(forbuttonIcon);
    forbutton->setIconSize(forpixmap.rect().size());
    forbutton->resize(forpixmap.rect().size());

    qDebug() << "Making Reload Button";
    QToolButton* relobutton = new QToolButton();
    QPixmap relopixmap(RELOIMG);
    QIcon relobuttonIcon(relopixmap);
    relobutton->setIcon(relobuttonIcon);
    relobutton->setIconSize(relopixmap.rect().size());
    relobutton->resize(relopixmap.rect().size());

    qDebug() << "Adding buttons to toolbar";
    ToolBar->addWidget(backbutton);
    ToolBar->addWidget(forbutton);
    ToolBar->addWidget(searchBar);
    ToolBar->addWidget(relobutton);

    qDebug() << "Setting Caches";
    layout->addWidget(ToolBar);
    QWebEngineProfile* profile = new QWebEngineProfile("OrgBrowser", &app);
    std::string cachehome = getenv("HOME");
    if (cachehome.empty()) {
        qWarning("HOME environment variable not set!");
        return 1;
    }
    cachehome.append("/Library/Caches/OrgBrowser/");
    std::filesystem::create_directories(cachehome);
    std::string CachePath = cachehome + "/cache";
    std::filesystem::create_directories(CachePath);
    std::string PersPath = cachehome + "/store";
    std::filesystem::create_directories(PersPath);
    profile->setCachePath(QString::fromStdString(CachePath));
    profile->setPersistentStoragePath(QString::fromStdString(PersPath));
    profile->setPersistentCookiesPolicy(QWebEngineProfile::ForcePersistentCookies);

    // Create web view
    qDebug() << "Making Web View";
    QWebEngineView* view = new QWebEngineView();
    QWebEnginePage* page = new QWebEnginePage(profile, view);
    view->setPage(page);
    view->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(view, 1);

    // Load initial page
    view->load(QUrl("https://google.com/"));

    // When user presses Enter in the search bar
    qDebug() << "Connecting Buttons";
    QObject::connect(searchBar, &QLineEdit::returnPressed, [&]() {
        QString urlText = searchBar->text();
        if (!endsWithTLD(urlText.toStdString())){
            urlText = "https://google.com/search?q=" + urlText;
        }
        if (!urlText.startsWith("http"))
            urlText = "https://" + urlText;
        
        view->load(QUrl(urlText));
    });
    QObject::connect(backbutton, &QToolButton::pressed, [&](){
        view->back();
    });
    QObject::connect(forbutton, &QToolButton::pressed, [&] (){
        view->forward();
    });
    QObject::connect(relobutton, &QToolButton::pressed, [&](){
        view->reload();
    });

    // Show everything
    qDebug() << "Starting Window";
    window.setWindowTitle("OrgBrowser");
    window.resize(1000, 700);
    window.show();

    qDebug() << "Returning";
    return app.exec();
}