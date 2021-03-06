﻿#ifndef VOICEBANKMANAGERWINDOW_H
#define VOICEBANKMANAGERWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QAction>
#include "voicebankhandler.h"
#include <QDebug>
#include <QTableWidgetItem>
#include <QMenu>
#include <QAction>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include "folderssettingdialog.h"
#include "textcodecsettingdialog.h"
#include <QSettings>
#include <public_defines.h>
#include "TextCodecUtils/textcodecconvertdialog.h"
#include <QPair>
#include <QInputDialog>
#include "voicebanktablemodel.h"
#include "EngineUtils/moresamplerconfigreader.h"
#include "EngineUtils/moresamplerconfigsdialog.h"
#include <QDir>
#include <QFileInfo>
#include <QRandomGenerator>
#include "voicebankiconcropdialog.h"
#include <QImageReader>
#include <QMediaPlayer>
#include <QProgressBar>
#include <QTranslator>
#include "./TextCodecUtils/qchardet.h"
#include "categoriesandlabelslistwidget.h"
#include "CommonUtils/showhtmldialog.h"
#include <setoperations.h>
#include <functional>
#include "TextCodecUtils/textconverthelper.h"

namespace Ui {
class VoiceBankManagerWindow;
}

/// 音源管理器组件的主窗口。
class VoiceBankManagerWindow : public QMainWindow
{


    Q_OBJECT
public:
    explicit VoiceBankManagerWindow(QWidget *parent = nullptr);
    ~VoiceBankManagerWindow() override;

    void loadVoiceBanksAndTable();

public slots:

protected:
    void changeEvent(QEvent *e) override;
private:
    Ui::VoiceBankManagerWindow *ui;

    VoiceBankHandler* voiceBankHandler = VoiceBankHandler::getVoiceBankHandler();

    struct TableColumn
    {
        static constexpr int Name = 0;
        static constexpr int Path = 1;
    };
    VoiceBankTableModel* voiceBankTableModel = nullptr;

    void showVoiceBanksRows(const QList<int> &voiceBankIDs);


    bool showMoreInformationInTotalCountLabel = true;
    void updateVoiceBankCountLabel();

    QMenu* voiceBanksTableWidgetMenu = new QMenu(this);
    QMenu* voiceBankCategoriesSubMenu = nullptr;
    QActionGroup* voiceBankCategoriesActionGroup = new QActionGroup(this);
    QMenu* voiceBankLabelsSubMenu = nullptr;
    QActionGroup* voiceBankLabelsActionGroup = new QActionGroup(this);
    void createVoiceBanksTableMenu();

    VoiceBank *getSelectedVoiceBank();
    VoiceBank *getSelectedVoiceBank(const QModelIndex &current);

    void setVoiceBankInfomation(VoiceBank *voiceBank);

    QProgressBar* samplePlayerProgress = new QProgressBar();
    QMediaPlayer* samplePlayer = new QMediaPlayer(this);

    void autoDetectTranslate();
    QList<QTranslator*> translators;
    void removeAllTranslators();

    CategoriesAndLabelsListWidget* categoriesAndLabelsListWidget = new CategoriesAndLabelsListWidget(voiceBankHandler,this);
    QStringList currentCategoriesFilter = {""};
    QStringList currentLabelFilter = {""};

    void loadWindowStatus();
    void saveWindowStatus();

    void letUserModifyFolder(std::function<QStringList(VoiceBankHandler*)>getFunc, std::function<void(VoiceBankHandler*,const QStringList&)>setFunc, const QString& name, const QStringList &defaultList = {});

    void refreshCategoryAndLabelsActionsChecked();

private slots:
#ifndef NDEBUG
    //Debug菜单项的槽
    void debugFunction();
#endif

    //void voiceBankReadDoneSlot(VoiceBank *voiceBank);

    void createVoiceBanksCategoriesSubMenu();
    void on_voiceBanksTableView_customContextMenuRequested(const QPoint &);
    void on_voicebankImage_customContextMenuRequested(const QPoint &);
    //音源列表右键菜单触发槽
    void openVoiceBankPathInExplorer();
    void openVoiceBankCharacterFileByOS();
    void openVoiceBankReadmeFileByOS();
    void copyVoiceBankPathtoClipboard();
    void copyVoiceBankCharacterFilePathtoClipboard();
    void copyVoiceBankReadmeFilePathtoClipboard();
    void reloadVoiceBankActionSlot();
    void convertCharacterCodecActionSlot();
    void convertReadmeCodecActionSlot();
    void convertWavFileNameCodecActionSlot();
    void moresamplerConfigEditActionSlot();
    void modifyNameActionSlot();
    void modifyIconActionSlot();
    void addNewCategoryActionSlot();
    void setCategoryActionsSlot(QAction *action);
    void createVoiceBanksLabelsSubMenu();
    void addNewLabelActionSlot();
    void setLabelActionSlot(QAction *action);
    void ignoreActionSlot();
    void setCodecForVoiceBankActionSlot();

    //UI菜单点击触发槽
    void on_actionMonitor_Folders_triggered();
    void on_actionOutside_VoiceBanks_triggered();
    void on_actionIgnored_folders_triggered();
    void on_actionRefresh_triggered();
    void on_actionDefault_TextCodec_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_actionAbout_Qt_triggered();
    void on_actionSet_Thread_Pool_Max_Count_triggered();
    void on_actionFor_text_file_triggered();
    void on_actionFor_File_Name_triggered();
    void on_actionuse_old_watched_folder_scan_strategy_toggled(bool checked);
    void on_actionshow_more_infomation_in_total_count_label_toggled(bool checked);
    void on_actionView_scan_details_triggered();
    void on_actionEdit_Global_MoresamplerConfig_triggered();
    void on_actionchoose_a_voicebank_randomly_triggered();

    void onVoiceBankViewCurrentChanged(const QModelIndex &, const QModelIndex &);

    void onBackupImageFileBecauseExists(VoiceBank * voicebank);
    void onCannotBackupImageFile(VoiceBank* voicebank);

    //语言相关槽
    void dealLanguageMenuAutoAndDontStates();
    void dealLanguageMenuLoadFile();

    //过滤器相关槽
    void on_searchLineEdit_textChanged(const QString &);
    void onCurrentCategoriesChanged(const QStringList& current);
    void onCurrentLabelsChanged(const QStringList& current);
    void dealFilters();


    //播放样例相关槽
    void on_playSamplebutton_clicked();
    void onSamplePlayerPositionChange(qint64 position);
    void onSamplePlayerStateChanged(QMediaPlayer::State state);

    void setUIAfterVoiceBanksReadDone();

    void onUseOldFolderScanChanged();
};

#endif // VOICEBANKMANAGERWINDOW_H
