#include "modelsListWidget.h"
#include <QDebug>

ModelsListWidget::ModelsListWidget(QWidget *parent) : QListWidget(parent)
{
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

   // RIGHT CLIC ACTIONS
    QAction *addModelButton = new QAction("&Add Model", this);
    QAction *saveModelButton = new QAction("&Save Model", this);
    QAction *removeModelButton = new QAction("&Remove Model", this);
    QAction *changeColorButton = new QAction("&Change Color", this);
    QAction *addTextureButton = new QAction("Add &Texture", this);
    referenceModelButton = new QAction("Reference &Model", this);

    this->addAction(addModelButton);
    this->addAction(saveModelButton);
    this->addAction(removeModelButton);
    this->addAction(changeColorButton);
    this->addAction(addTextureButton);
    this->addAction(referenceModelButton);


    emit updateList();


    /* ============================ CONNECTIONS ============================ */

    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(updateCheckedModels()));

    connect(addModelButton, SIGNAL(triggered()), this, SLOT(emitAddModel()));
    connect(saveModelButton, SIGNAL(triggered()), this, SLOT(emitSaveModels()));
    connect(removeModelButton, SIGNAL(triggered()), this, SLOT(emitRemoveModels()));
    connect(changeColorButton, SIGNAL(triggered()), this, SLOT(emitChangeColor()));
    connect(addTextureButton, SIGNAL(triggered()), this, SLOT(emitAddTexture()));
    connect(referenceModelButton, SIGNAL(triggered()), this, SLOT(emitReferenceModel()));
}

void ModelsListWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        QListWidgetItem *clickedItem = this->itemAt(event->pos());

        if(clickedItem)
        {
            for(GLuint i = 0; i < (GLuint)pathsList.size(); i++)
                if(pathsList.at(i).contains(clickedItem->text()))
                    selectedItem = pathsList.at(i);

            referenceModelButton->setEnabled(true);
        }

        else
            referenceModelButton->setEnabled(false);
    }
    else
    {
        QListWidget::mousePressEvent(event);
        selectedItem = QString("");
    }
}


/* ============================ UPDATE ============================ */
void ModelsListWidget::updateModelsList(QStringList items)
{
    this->clear();

    pathsList = items;


    for(GLuint i = 0; i < (GLuint)items.size(); i++)
    {
        QStringList temp = items.at(i).split("/");
        QListWidgetItem *currentItem = new QListWidgetItem(temp.at(temp.size()-1));

        currentItem->setFlags(currentItem->flags() | Qt::ItemIsUserCheckable);
        if(checked.contains(i))
            currentItem->setCheckState(Qt::Checked);
        else
            currentItem->setCheckState(Qt::Unchecked);

        this->addItem(currentItem);
    }

    updateCheckedModels();
}
void ModelsListWidget::updateCheckedModels()
{
    checked.clear();
    for(GLuint i = 0; i < (GLuint)this->count(); i++)
        if(this->item(i)->checkState())
            checked.push_back(i);

    emit updateCheckedModels(checked);
}


/* ============================ SIGNALS ============================ */
void ModelsListWidget::emitAddModel()
{
    emit addModel();
}
void ModelsListWidget::emitSaveModels()
{
    emit selectedModelChanged(selectedItem);
    emit saveModels();
}
void ModelsListWidget::emitChangeColor()
{
    if(!checked.isEmpty() || !selectedItem.isEmpty())
    {
        QColorDialog *colorWindow = new QColorDialog(this);
        colorWindow->setWindowTitle("Models Color");

        QColor newColor = colorWindow->getColor(Qt::white,this, QString("Models Color"));

        emit selectedModelChanged(selectedItem);
        emit changeColor(newColor);
    }
}
void ModelsListWidget::emitRemoveModels()
{
    emit updateCheckedModels(checked);

    emit selectedModelChanged(selectedItem);
    emit removeModels();

    checked.clear();
    emit updateList();
}
void ModelsListWidget::emitAddTexture()
{
    if(!checked.isEmpty() || !selectedItem.isEmpty())
    {
        emit selectedModelChanged(selectedItem);
        emit addTexture();
    }
}
void ModelsListWidget::emitReferenceModel()
{
    if(referenceModel == selectedItem)
        referenceModel = QString("");
    else
        referenceModel = selectedItem;

    changeItemsFont();
}
void ModelsListWidget::changeItemsFont()
{
    for(GLuint i = 0; i < (GLuint)this->count(); i++)
    {
        QFont font;

        if(!referenceModel.isEmpty() && referenceModel.contains(this->item(i)->text()))
            font.setWeight(QFont::Bold);
        else
            font.cleanup();

        this->item(i)->setFont(font);
    }

    emit referenceModelChanged(referenceModel);
}
