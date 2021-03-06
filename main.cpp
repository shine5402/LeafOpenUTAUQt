﻿#include "editorwindow.h"
#include "VoiceBankUtils/voicebankmanagerwindow.h"
#include <QApplication>
#include <QTranslator>
#include <leaflogger.h>
#include <QLibraryInfo>
#include <QTextCodec>
#include "public_defines.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("Leaf UTAU Qt Developer Team");
    QCoreApplication::setOrganizationDomain("shine5402.top");
    QCoreApplication::setApplicationName("Leaf UTAU Qt");
    LeafLogger::LogInit();

#ifdef EDITOR
    EditorWindow w;
    w.show();
#endif

#ifdef VOICEBANK_MANAGER
    VoiceBankManagerWindow v;
    v.show();
    v.loadVoiceBanksAndTable();
#endif
    return a.exec();
}
