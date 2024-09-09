#pragma once

#include "./basedialog/basedialog.h"

namespace Ui {
class MetaDataEditor;
}

class MetaDataEditor : public BaseDialog {
    Q_OBJECT

public:
    explicit MetaDataEditor(QWidget *parent = nullptr);
    MetaDataEditor(const QVariantMap &map, QWidget *parent = nullptr);
    ~MetaDataEditor();

    void setData(const QVariantMap &map);
    QVariantMap data() const { return m_data; }

private slots:
    void okBtnClicked();

private:
    Ui::MetaDataEditor *ui;
    QVariantMap m_data;
};
