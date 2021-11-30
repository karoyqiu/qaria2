﻿/*! ***********************************************************************************************
 *
 * \file        aria2c.h
 * \brief       Aria2c 类头文件。
 *
 * \version     1.0
 * \date        2021-11-30
 *
 * \author      Roy QIU <karoyqiu@gmail.com>
 * \copyright   © 2018~2021 ubesthelp。
 *
 **************************************************************************************************/
#pragma once
#include <QObject>

class QWebSocket;

/// Download status.
enum class DownloadStatus
{
    /// Currently downloading/seeding downloads
    Active,
    /// Downloads in the queue, download is not started
    Waiting,
    /// Paused downloads
    Paused,
    /// Downloads that were stopped because of error
    Error,
    /// Stopped and completed downloads
    Complete,
    /// The downloads removed by user
    Removed,
};

/**
 * \brief   下载的文件
 */
struct DownloadFile
{
    /// Index of the file, starting at 1, in the same order as files appear in the multi-file torrent.
    int index = 0;
    /// File path.
    QString path;
    /// File size in bytes.
    qint64 length = 0;
    /// Completed length of this file in bytes.
    qint64 completedLength = 0;
    /// `true` if this file is selected by --select-file option.
    bool selected = false;
    /// Returns a list of URIs for this file.
    QList<QUrl> uris;
};

using DownloadFiles = QList<DownloadFile>;

/// Struct which contains information retrieved from the .torrent (file).
struct BitTorrentInfo
{
    /// name in info dictionary.
    QString name;
    /// The comment of the torrent.
    QString comment;
    /// The creation time of the torrent. The value is an integer since the epoch, measured in seconds.
    QDateTime creationDate;
    /// File mode of the torrent. The value is either `single` or `multi`.
    QString mode;
};

/// Progress of the download.
struct DownloadItem
{
    /// GID of the download.
    QString gid;
    /// Status
    DownloadStatus status = DownloadStatus::Active;
    /// Total length of the download in bytes.
    qint64 totalLength = 0;
    /// Completed length of the download in bytes.
    qint64 completedLength = 0;
    /// Uploaded length of the download in bytes.
    qint64 uploadLength = 0;
    /// Download speed of this download measured in bytes/sec.
    qint64 downloadSpeed = 0;
    /// Upload speed of this download measured in bytes/sec.
    qint64 uploadSpeed = 0;
    /// The code of the last error for this item, if any.
    int errorCode = 0;
    /// The (hopefully) human readable error message associated to `errorCode`.
    QString errorMessage;
    /// List of GIDs which are generated as the result of this download.
    QStringList followedBy;
    /// The reverse link for `followedBy`.
    QString following;
    /// Directory to save files.
    QString dir;
    /// Returns the list of files.
    DownloadFiles files;
    /// BitTorrent only.
    BitTorrentInfo bittorrent;
};

using DownloadItems = QList<DownloadItem>;


class Aria2c : public QObject
{
    Q_OBJECT

public:
    explicit Aria2c(QObject *parent = nullptr);

    void start();

    void addUri(const QStringList &uris, const QVariantHash &options = {});
    void addUri(const QString &uri, const QVariantHash &options = {})
    {
        addUri(QStringList{ uri }, options);
    }

signals:
    void onDownloadStart(const QString &gid);
    void onDownloadPause(const QString &gid);
    void onDownloadStop(const QString &gid);
    void onDownloadComplete(const QString &gid);
    void onDownloadError(const QString &gid);
    void onBtDownloadComplete(const QString &gid);

private:
    static QString generateToken();

    void onConnected();
    void handleMessage(const QString &msg);

    void send(const QJsonDocument &doc);

    template<typename... T>
    void callAsync(const QString &method, T&&... args);

    void toQVariantList(QVariantList & /*result*/) {}

    template<typename T>
    void toQVariantList(QVariantList &result, T &&x);

    template<typename T, typename... Ts>
    void toQVariantList(QVariantList &result, T &&head, Ts&&... tail);

private:
    QWebSocket *ws_;
    QString secret_;
};


template<typename... T>
void Aria2c::callAsync(const QString &method, T&&... args)
{
    QVariantList params;
    toQVariantList(params, std::forward<T>(args)...);
    params.prepend(QString(QL("token:") % secret_));

    QJsonObject obj{
        { QS("jsonrpc"), QS("2.0") },
        { QS("id"), generateToken() },
        { QS("method"), method },
        { QS("params"), QJsonArray::fromVariantList(params) },
    };

    send(QJsonDocument(obj));
}


template<typename T>
void Aria2c::toQVariantList(QVariantList &result, T &&x)
{
    result.prepend(x);
}


template<typename T, typename... Ts>
void Aria2c::toQVariantList(QVariantList &result, T &&head, Ts&&... tail)
{
    toQVariantList(result, std::forward<Ts>(tail)...);
    result.prepend(head);
}
