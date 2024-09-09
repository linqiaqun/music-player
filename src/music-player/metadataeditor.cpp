#include "metadataeditor.h"

#include <QCoreApplication>
#include <QFileInfo>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QtConcurrent>

#include "./taglibhelper/taglibhelper.h"
#include "ui_metadataeditor.h"

MetaDataEditor::MetaDataEditor(QWidget *parent) : BaseDialog(parent), ui(new Ui::MetaDataEditor) {
    ui->setupUi(this);
    setWindowTitle(QCoreApplication::applicationName());
    connect(ui->okBtn, &QPushButton::clicked, this, &MetaDataEditor::okBtnClicked);

    // window shadow setting
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setOffset(0, 0);
    shadow->setColor(Qt::gray);
    shadow->setBlurRadius(32);
    ui->container->setGraphicsEffect(shadow);
    setContentsMargins(8, 8, 8, 8);
}

MetaDataEditor::MetaDataEditor(const QVariantMap &map, QWidget *parent) : MetaDataEditor(parent) {
    setData(map);
}

MetaDataEditor::~MetaDataEditor() { delete ui; }

void MetaDataEditor::setData(const QVariantMap &map) {
    m_data = map;
    ui->mdePath->setText(QFileInfo(map["path"].toString()).fileName());
    ui->mdeTitle->setText(map["title"].toString());
    ui->mdeArtist->setText(map["artist"].toString());
    ui->mdeAlbum->setText(map["album"].toString());
    ui->mdeGenre->setText(map["genre"].toString());
    ui->mdeYear->setText(map["year"].toString());
    ui->mdeTrack->setText(map["track"].toString());
    ui->mdeComment->setText(map["comment"].toString());
}

void MetaDataEditor::okBtnClicked() {
    QStringList nameList = TagLibHelper::propertyNames();
    QList<QLineEdit *> editList = {ui->mdeTitle, ui->mdeArtist, ui->mdeAlbum,  ui->mdeGenre,
                                   ui->mdeYear,  ui->mdeTrack,  ui->mdeComment};
    QVariantList list;
    for (auto i = 0; i < editList.count(); ++i) {
        if (editList[i]->text() != m_data[nameList[i]].toString()) {
            QVariantMap map;
            map.insert("name", nameList[i]);
            map.insert("value", editList[i]->text());
            list.append(map);
        }
    }
    if (0 == list.count()) reject();

    QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>;
    connect(watcher, &QFutureWatcher<bool>::finished, this, [=]() {
        bool result = watcher->result();
        watcher->deleteLater();
        if (!result) {
            QMessageBox::critical(nullptr, tr("error"), tr("update metadata failed"));
            return;
        }

        for (auto &item : list) {
            QVariantMap map = item.toMap();
            m_data.insert(map["name"].toString(), map["value"]);
        }
        accept();
    });
    watcher->setFuture(
        QtConcurrent::run(TagLibHelper::setPropertyList, m_data["path"].toString(), list));
}
