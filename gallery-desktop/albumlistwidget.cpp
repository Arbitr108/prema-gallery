#include "albumlistwidget.h"
#include "ui_albumlistwidget.h"

#include <QInputDialog>
#include "albummodel.h"

AlbumListWidget::AlbumListWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AlbumListWidget),
    mAlbumModel(nullptr)
{
    ui->setupUi(this);
    connect(ui->createAlbumButton, &QPushButton::clicked,
            this, &AlbumListWidget::createAlbum);
}

void AlbumListWidget::setModel(AlbumModel* model){
    mAlbumModel = model;
    ui->albumList->setModel(mAlbumModel);
}

void AlbumListWidget::setSelectionModel(QItemSelectionModel* selectionModel){
    ui->albumList->setSelectionModel(selectionModel);
}

void AlbumListWidget::createAlbum()
{
    if(!mAlbumModel){
        return;
    }
    bool ok;
    QString albumName = QInputDialog::getText(this,
                                              "Создание нового альбома",
                                              "Выберите название",
                                              QLineEdit::Normal,
                                              "Новый альбом", &ok);
    if(ok && !albumName.isEmpty()){
        Album album(albumName);
        QModelIndex createdIndex = mAlbumModel->addAlbum(album);
        ui->albumList->setCurrentIndex(createdIndex);
    }
}

AlbumListWidget::~AlbumListWidget()
{
    delete ui;
}
