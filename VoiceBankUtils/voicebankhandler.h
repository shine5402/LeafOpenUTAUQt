﻿#ifndef VOICEBANKHANDLER_H
#define VOICEBANKHANDLER_H

#include <QObject>
#include "voicebank.h"
#include <QThread>
#include <QThreadPool>
#include <public_defines.h>

///用于管理一系列VoiceBank。
/*!
  VoiceBankHandler 是用于管理 VoiceBank 的类。\n
  VoiceBankHandler 采用单件模式，您不能直接创建，而是通过 getVoiceBankHandler() 来获取其实例。\n
  一般来说，您不需要手动添加 VoiceBank ，而是使用 readVoiceBanksFromMonitorFolders() 来从用户指定的监视文件夹中读取音源库，并自动转化为 VoiceBank 的实例。\n
  它允许在它管理的 VoiceBank 中通过名称、分类、标签等条件查找音源库，详见 findIDByName(const QString &text)、 findIDByCategory(const QString &category)、 findIDByLabel(const QString &label) 。\n
  它也允许对内部的 VoiceBank 列表进行排序，详见 sort(SortableInformationID sortWhat, Qt::SortOrder order = Qt::AscendingOrder) 。
*/
class VoiceBankHandler : public QObject
{
    Q_OBJECT

public:
    static VoiceBankHandler * getVoiceBankHandler();
    ~VoiceBankHandler();

    void readVoiceBanksFromMonitorFolders();

    /*
      监视文件夹相关函数。具体使用参见函数内的文档。
    */

    QStringList getMonitorFolders();
    void setMonitorFolders(const QStringList &value);

    QStringList getOutsideVoiceBankFolders();
    void setOutsideVoiceBankFolders(const QStringList &value);

    QStringList getIgnoreVoiceBankFolders();
    void setIgnoreVoiceBankFolders(const QStringList &value);
    void addIgnoreVoiceBankFolder(const QString &path);

    bool isUseOldFolderScan();
    void setUseOldFolderScan(bool value);

    /*
      以下三个函数在调用过readVoiceBanksFromMonitorFolders()之后才有意义。
    */
    QStringList getIgnoredVoiceBankFolders() const;
    QStringList getNotVoiceBankPaths() const;
    QStringList getScannedSubFolders() const;

    /*
      作为VoiceBank保存者的相关函数
    */
    QList<VoiceBank *> getVoiceBanks() const;

    VoiceBank* addVoiceBank(QString& path);

    QList<VoiceBank*> addVoiceBanks(QStringList& paths){
        QList<VoiceBank*> voiceBanks;
        for (auto path : paths)
        {
            addVoiceBank(path);
        }
        return voiceBanks;
    }

    int count(){
        return voiceBanks.count();
    }

    VoiceBank* getVoiceBank(int id) const{
        return voiceBanks.value(id);
    }

    int getVoiceBankID(VoiceBank* voiceBank) const
    {
        return voiceBanks.indexOf(voiceBank);
    }
    QList<int> findIDByName(const QString &text) const;
    QList<int> findIDByCategory(const QString &category) const;
    QList<int> findIDByLabel(const QString &label) const;

    void clear();

    void setThreadPoolMaxThreadCount(int maxCount);
    int getThreadPoolMaxThreadCount() const{
        return threadPool->maxThreadCount();
    }

    enum class SortableInformationID{Name,Path};
    void sort(SortableInformationID sortWhat, Qt::SortOrder order = Qt::AscendingOrder);

    class VoiceBankReadFuctionRunner : public QRunnable
    {
    public:
        VoiceBankReadFuctionRunner(VoiceBank* voicebank);
        void run() override;
    private:
        VoiceBank* voicebank;
    };

private:
    explicit VoiceBankHandler(QObject *parent = nullptr);
    VoiceBankHandler(const VoiceBankHandler&) = delete;
    VoiceBankHandler& operator= (const VoiceBankHandler&) = delete;


    static VoiceBankHandler *s_voiceBankHanlder;

    int voiceBankReadDoneCount = 0;

    QStringList getVoiceBankFoldersInFolder(const QString &dir);
    bool useOldFolderScan = false;
    QStringList outsideVoiceBankFolders;
    QStringList ignoreVoiceBankFolders;
    QStringList ignoredVoiceBankFolders;
    QStringList notVoiceBankPaths;
    void findScannedSubFolders();
    QStringList scannedSubFolders;

    QStringList monitorFolders = {"./voice"};

    QStringList getFoldersInMonitorFolders();
    void loadMonitorFoldersSettings();
    void saveMonitorFoldersSettings();

    QList<VoiceBank *> voiceBanks{};
    void addVoiceBank(VoiceBank * newVoiceBank){
        voiceBanks.append(newVoiceBank);
    }
    QThreadPool* threadPool = new QThreadPool(this);
    void readThreadPoolMaxThreadCountSettings();
    void saveThreadPoolMaxThreadCountSettings();
    class DestroyHelper {
    public:
        ~DestroyHelper(){
            if (VoiceBankHandler::s_voiceBankHanlder)
                delete VoiceBankHandler::s_voiceBankHanlder;
        }
    };

    static DestroyHelper destoryHelper;
    friend DestroyHelper;
private slots:
    void voiceBankReadDoneSlot(VoiceBank *voiceBank);
signals:
    void aVoiceBankReadDone(VoiceBank* voicebank);
    void backupImageFileBecauseExists(VoiceBank *);
    void cannotBackupImageFile(VoiceBank *);
    void categoriesChanged();
    void labelsChanged();
    void categroiesAndLabelsChanged();
    void useOldFolderScanChanged();
    void voiceBanksReadDone();
public slots:
};


#endif // VOICEBANKHANDLER_H
