#include "mgftextviewwidget.h"
#include "ui_mgftextviewwidget.h"

#include "mgf/Archive.h"

MGFTextViewWidget::MGFTextViewWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MGFTextViewWidget)
{
    m_SupportedTypes.insert(MGFFileType::Text);

    ui->setupUi(this);
}

MGFTextViewWidget::~MGFTextViewWidget()
{
    delete ui;
}

void MGFTextViewWidget::LoadMGFItem(const MGF::File *item)
{
    AMGFResourceViewer::LoadMGFItem(item);

    QString data = ResourceManager.GetTextData(*item);

    ui->plainTextEdit->clear();
    ui->plainTextEdit->setPlainText(data);
}
