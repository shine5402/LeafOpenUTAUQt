﻿#ifndef VOICEBANKHANDLER_H
#define VOICEBANKHANDLER_H

#include <QObject>
#include "voicebank.h"
#include <QThread>
#include <QThreadPool>
class VoiceBankHandler : public QObject
{
    Q_OBJECT
public:
    explicit VoiceBankHandler(QObject *parent = nullptr);
    ~VoiceBankHandler();
    QList<VoiceBank *> getVoiceBanks() const;

    VoiceBank* addVoiceBank(QString& path);
    QList<VoiceBank*> addVoiceBanks(QStringList& paths){
        QList<VoiceBank*> voiceBanks;
        for (auto path : paths)
        {
            auto voiceBank = addVoiceBank(path);
            voiceBanks.append(voiceBank);
        }
        return voiceBanks;
    }
    int count(){
        return voiceBanks.count();
    }
    VoiceBank* getVoiceBank(int id){
        return voiceBanks.value(id);
    }
    int getVoiceBankID(VoiceBank* voiceBank)
    {
        return voiceBanks.indexOf(voiceBank);
    }
    void clear();
    class VoiceBankReadFuctionRunner : public QRunnable{
    public:
        VoiceBankReadFuctionRunner(VoiceBank* voicebank);
        void run() override;
    private:
        VoiceBank* voicebank;
    };
    void setThreadPoolMaxThreadCount(int maxCount);
    int getThreadPoolMaxThreadCount() const{
        return threadPool->maxThreadCount();
    }
    enum class SortableInformationID{Name,Path};
    void sort(SortableInformationID sortWhat, Qt::SortOrder order = Qt::AscendingOrder);
private:
    QList<VoiceBank *> voiceBanks{};
    void addVoiceBank(VoiceBank * newVoiceBank){
        voiceBanks.append(newVoiceBank);
    }
    QThreadPool* threadPool = new QThreadPool(this);
    void readThreadPoolMaxThreadCountSettings();
    void saveThreadPoolMaxThreadCountSettings();
private slots:
    void aVoiceBankReadDoneSlot(VoiceBank* voiceBank);
signals:
    void aVoiceBankReadDone(VoiceBank* voicebank);
public slots:
};

#endif // VOICEBANKHANDLER_H
