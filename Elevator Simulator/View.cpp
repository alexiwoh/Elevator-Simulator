#include "View.h"

View::View(MainWindow * m, ECS *model) {
    mainWindow = m;
    this->model = model;
    curElev = 0;
    curFloor = model->getMinFloor();

    drawGrid();
    updateTimer = new QTimer(this);
    updateTimer->start(refreshTime);
    connect(updateTimer,SIGNAL(timeout()),this,SLOT(update()));

    createGUI();


    sButtons = new QHash<QString,QPushButton*>();
    nButtons = new QList<QPushButton*>();

    createGUIButtons();
    update();
}

View::~View()   {

    for(int i=0; i<labels.size(); ++i) delete labels[i];
    labels.clear();
    for(int i=0; i<labelsGUI.size(); ++i) delete labelsGUI[i];
    labelsGUI.clear();
    for(int i=0; i<widgets.size(); ++i) delete widgets[i];
    widgets.clear();

    delete curElevSlider;
    delete weightBox;
    delete floorsBox; delete elevBox; delete minFloorsBox;
    delete curFloorSlider;
    delete dimensionsButton;
    delete updateTimer;

    for(QHash<QString,QPushButton*>::iterator i=sButtons->begin(); i != sButtons->end(); ++i)
        if(i.value() != NULL)   delete i.value();
    delete sButtons;

    for(int i=0; i<nButtons->size(); ++i)   {
        delete nButtons->at(i);
    }
    delete nButtons;
    delete upButton; delete downButton;
    delete floorLabel; delete elevLabel;
    delete curElevNumLabel; delete curFloorNumLabel;
    delete randomButton;

    if(model!=NULL) delete model;
}

void View::setMainWindow(MainWindow * m) {mainWindow = m;}
MainWindow *View::getMainWindow()   {return mainWindow;}
void View::setModel(ECS * ecs)  {model = ecs;}
ECS *View::getModel()   {return model;}

void View::restartNewDimensions()   {

    // Try to extract numbers from text boxes.
    int m,n,l;
    ECS *newModel = NULL;
    bool okM, okN, okL;
    try {
        m = (elevBox->text()).toInt(&okM);
        n = (floorsBox->text()).toInt(&okN);
        l = (minFloorsBox->text()).toInt(&okL);
        if(!(okM && okN && okL))    throw QException();
        newModel = new ECS(m,n,l,model->hasRandomEvents());
    }
    catch(...) {
        if(newModel != NULL)   delete newModel;
        return;
    }

    // Clean up GUI elements.
    for(int i=0; i<labels.size(); ++i) {
        delete labels[i];
    }
    labels.clear();

    for(int i=0; i<labelsGUI.size(); ++i) {
        delete labelsGUI[i];
    }
    labelsGUI.clear();

    for(QHash<QString,QPushButton*>::iterator i=sButtons->begin(); i != sButtons->end(); ++i)
        if(i.value() != NULL)   delete i.value();

    for(int i=0; i<nButtons->size(); ++i)   {
        delete nButtons->at(i);
    }
    nButtons->clear();
    delete upButton; delete downButton;
    delete floorLabel; delete elevLabel;
    delete curElevSlider; delete curFloorSlider;
    delete curElevNumLabel; delete curFloorNumLabel;
    delete floorsBox; delete elevBox; delete minFloorsBox;
    delete weightBox;
    delete randomButton;

    delete model;

    // Create new GUI.
    model = newModel;
    curElev = 0;
    curFloor = model->getMinFloor();
    createGUI();
    createGUIButtons();
    update();
}

void View::toggleButton(QString button) {
    if(button.toLower()=="up" || button.toLower()=="down")  {
       model->toggleButton(button,curFloor - model->getMinFloor());
       return;
    }
    model->toggleButton(button,curElev);
}

void View::update()  {
    if(model == NULL)   return;

    Elevator *e = model->getElevators()->at(curElev);
    curElev = curElevSlider->value(); curFloor = curFloorSlider->value();
    model->setCurElevator(curElev); model->setCurFloor(curFloor);
    if(curElevNumLabel !=NULL && curFloorNumLabel != NULL)  {
        curElevNumLabel->setText("(E" + QString::number(curElev) + ")");
        curFloorNumLabel->setText("(" + (QString)((curFloor>=0)?"F":"B") + QString::number(qAbs(curFloor)) + ")");
    }
    if(floorLabel != NULL && elevLabel != NULL) {
        elevLabel->setText("Elevator Buttons  (E" + QString::number(curElev) + ")"
            + "\nCurrent Floor:  " + (QString)((e->getCurFloor() + model->getMinFloor() >= 0)?"":"B") + QString::number(qAbs(e->getCurFloor() + model->getMinFloor()))
            + "\nStatus:  " + e->getStatus());
        floorLabel->setText("Floor Buttons  (" + (QString)((curFloor>=0)?"F":"B") + QString::number(qAbs(curFloor)) + ")");

    }
    weightBox->setText(QString::number(e->getCurWeight()));

    // Change button colors.
    QColor col;
    QString qs;
    for(int i=0; i<nButtons->size(); ++i)   {
        if(e->getNButtons()->at(i) == true) {
            col = QColor(Qt::blue);
        } else {
            col = QColor(Qt::gray);
        }
        QString qs = QString("background-color: %1").arg(col.name());
        nButtons->at(i)->setStyleSheet(qs);
    }
    for(QHash<QString,QPushButton*>::iterator i=sButtons->begin(); i != sButtons->end(); ++i) {
        if(e->getSButtons()->value(i.key()) == true) {
            col = QColor(Qt::blue);
        } else {
            col = QColor(Qt::gray);
        }
        QString qs = QString("background-color: %1").arg(col.name());
        i.value()->setStyleSheet(qs);

    }

    // Random events button.
    if(model->hasRandomEvents())    col = QColor(Qt::darkYellow);
    else                            col = QColor(Qt::gray);
    qs = QString("background-color: %1").arg(col.name());
    randomButton->setStyleSheet(qs);
    randomButton->setText("Random Events: " + (QString)((model->hasRandomEvents()) ? "ON" : "OFF"));

    // Up/down buttons.
    Floor *floors = model->getFloors()->at(curFloor - model->getMinFloor());

    if(floors->getUpButtonState() == true) {col = QColor(Qt::blue);}
    else                                   {col = QColor(Qt::gray);}
    qs = QString("background-color: %1").arg(col.name());
    upButton->setStyleSheet(qs);
    upButton->setDisabled(curFloor - model->getMinFloor() == model->getNumFloors()-1);
    upButton->setVisible(curFloor - model->getMinFloor() != model->getNumFloors()-1);

    if(floors->getDownButtonState() == true) {col = QColor(Qt::blue);}
    else                                   {col = QColor(Qt::gray);}
    qs = QString("background-color: %1").arg(col.name());
    downButton->setStyleSheet(qs);
    downButton->setDisabled(curFloor == model->getMinFloor());
    downButton->setVisible(curFloor != model->getMinFloor());

    drawGrid();
}

void View::createGUIButtons()   {

    int n = model->getNumFloors();
    int i, endX, endY, minFloor, posX, posY;
    minFloor = model->getMinFloor();
    endX = gridStartX + boxSize*(model->getMaxElevators()-1);
    endY = gridstartY;
    posX = endX; posY = endY;
    QPushButton *qb;

    // Up/down buttons for selected floor.
    floorLabel = new QLabel("Floor Buttons",mainWindow);
    floorLabel->setGeometry(QRect(posX,posY,boxSize*3,boxSize));
    floorLabel->show();

    posX = endX; posY += boxSize*1.1;

    upButton = new QPushButton("Up",mainWindow);
    upButton->setGeometry(QRect(posX,posY,boxSize,boxSize));
    upButton->show();
    connect(upButton, &QPushButton::released, [this]() { toggleButton("up"); });
    posX += boxSize*1.1;
    downButton = new QPushButton("Down",mainWindow);
    downButton->setGeometry(QRect(posX,posY,boxSize,boxSize));
    downButton->show();
    connect(downButton, &QPushButton::released, [this]() { toggleButton("down"); });


    posX = endX; posY += boxSize*1.25;

    elevLabel = new QLabel("Elevator Buttons",mainWindow);
    elevLabel->setGeometry(QRect(posX,posY,boxSize*6,boxSize*1.25));
    elevLabel->show();

    posX = endX; posY += boxSize*1.25;



    // Special buttons.
    qb = new QPushButton("Help",mainWindow);
    qb->setGeometry(QRect(posX,posY,boxSize,boxSize));
    qb->show();
    connect(qb, &QPushButton::released, [this]() { toggleButton("help"); });
    posX += boxSize*1.1;
    sButtons->insert("help",qb);

    qb = new QPushButton("Fire",mainWindow);
    qb->setGeometry(QRect(posX,posY,boxSize,boxSize));
    qb->show();
    connect(qb, &QPushButton::released, [this]() { toggleButton("fire"); });
    posX += boxSize*1.1;
    sButtons->insert("fire",qb);

    qb = new QPushButton("Overload",mainWindow);
    qb->setGeometry(QRect(posX,posY,boxSize*2,boxSize));
    qb->show();
    connect(qb, &QPushButton::released, [this]() { toggleButton("overloaded"); });
    posX += boxSize*2.1;
    sButtons->insert("overloaded",qb);

    qb = new QPushButton("Power Out",mainWindow);
    qb->setGeometry(QRect(posX,posY,boxSize*2.2,boxSize));
    qb->show();
    connect(qb, &QPushButton::released, [this]() { toggleButton("powerOut"); });
    posX += boxSize*2.2;
    sButtons->insert("powerOut",qb);

    posX = endX; posY += boxSize*1.1;

    qb = new QPushButton("Open",mainWindow);
    qb->setGeometry(QRect(posX,posY,boxSize,boxSize));
    qb->show();
    connect(qb, &QPushButton::released, [this]() { toggleButton("doorOpen"); });
    posX += boxSize*1.1;
    sButtons->insert("doorOpen",qb);

    qb = new QPushButton("Close",mainWindow);
    qb->setGeometry(QRect(posX,posY,boxSize,boxSize));
    qb->show();
    connect(qb, &QPushButton::released, [this]() { toggleButton("doorClose"); });
    posX += boxSize*1.1;
    sButtons->insert("doorClose",qb);

    qb = new QPushButton("Door Obstacle",mainWindow);
    qb->setGeometry(QRect(posX,posY,boxSize*2.2,boxSize));
    qb->show();
    connect(qb, &QPushButton::released, [this]() { toggleButton("doorObstacle"); });
    posX += boxSize*2.2;
    sButtons->insert("doorObstacle",qb);

    posX = endX; posY += boxSize*1.1;

    // Numbered buttons.
    for(i=0; i<n; ++i)  {

        if(i+minFloor>=0)   qb = new QPushButton(QString::number(i+minFloor),mainWindow);
        else                qb = new QPushButton("B"+QString::number(qAbs(i+minFloor)),mainWindow);
        qb->setGeometry(QRect(posX,posY,boxSize,boxSize));
        qb->show();
        nButtons->append(qb);
        posX += boxSize*1.1;
        if(i % 6 == 5) {
            posX = endX; posY += boxSize*1.1;
        }
        QString text = QString::number(i);
        connect(qb, &QPushButton::released, [this, text]() { toggleButton(text); });
    }

    posX = endX; posY += boxSize*1.1;

    // Textbox and button for current elevator weight.
    QLabel *ql = new QLabel("Weight (kg)",mainWindow);
    ql->setGeometry(QRect(posX,posY+boxSize*0.25,maxLabelLength*1.25,boxSize));
    ql->show();
    labelsGUI.append(ql);
    posX += boxSize*1.5;
    weightBox = new QLineEdit(mainWindow);
    weightBox->setGeometry(0,0,boxSize,boxSize);
    weightBox->move(posX,posY+boxSize/4);
    weightBox->show();
    posX += boxSize*1.1*3;
    posX = endX;
    posY += boxSize*1.5;



}

void View::createGUI()  {
    if(model == NULL)   return;

    int n = model->getMaxFloors();
    int posX, posY;
    int minFloor = model->getMinFloor();
    int endY = gridstartY + boxSizeGrid*n;
    posX = gridStartX; posY = endY + boxSize/4;
    QLabel *ql;

    // Slider for current floor number.
    ql = new QLabel("Floor #",mainWindow);
    ql->setGeometry(QRect(gridStartX,posY,maxLabelLength/2,boxSize));
    ql->show();
    labelsGUI.append(ql);
    curFloorNumLabel = new QLabel(QString::number(curFloor),mainWindow);
    curFloorNumLabel->setGeometry(QRect(gridStartX + maxLabelLength/1.5,posY,boxSize,boxSize));
    curFloorNumLabel->show();
    posX += maxLabelLength;
    curFloorSlider = new QSlider(Qt::Horizontal,mainWindow);
    curFloorSlider->setTickPosition(QSlider::TicksBothSides);
    curFloorSlider->setGeometry(0,0,boxSize*5,boxSize);
    curFloorSlider->move(posX,posY+boxSize/4);
    curFloorSlider->setMinimum(minFloor);
    curFloorSlider->setMaximum(model->getNumFloors()-1+minFloor);
    curFloorSlider->setTickInterval(1);
    curFloorSlider->setSingleStep(1);
    curFloorSlider->show();

    posX = gridStartX;
    posY += boxSize;

    // Slider for current elevator number.
    ql = new QLabel("Elevator #",mainWindow);
    ql->setGeometry(QRect(gridStartX,posY,maxLabelLength/2,boxSize));
    ql->show();
    labelsGUI.append(ql);
    curElevNumLabel = new QLabel("(E" + QString::number(curElev) + ")",mainWindow);
    curElevNumLabel->setGeometry(QRect(gridStartX + maxLabelLength/1.5,posY,boxSize,boxSize));
    curElevNumLabel->show();
    posX += maxLabelLength;
    curElevSlider = new QSlider(Qt::Horizontal,mainWindow);
    curElevSlider->setTickPosition(QSlider::TicksBothSides);
    curElevSlider->setGeometry(0,0,boxSize*5,boxSize);
    curElevSlider->move(posX,posY+boxSize/4);
    curElevSlider->setMinimum(0);
    curElevSlider->setMaximum(model->getNumElevators()-1);
    curElevSlider->setTickInterval(1);
    curElevSlider->setSingleStep(1);
    curElevSlider->show();
    posX = gridStartX;
    posY += boxSize*1.5;

    // Textbox and button for number of elevators and floors.
    ql = new QLabel("# Elevators",mainWindow);
    ql->setGeometry(QRect(gridStartX,posY+boxSize*0.25,maxLabelLength,boxSize));
    ql->show();
    labelsGUI.append(ql);
    posX += boxSize*1.5;
    elevBox = new QLineEdit(mainWindow);
    elevBox->setGeometry(0,0,boxSize,boxSize);
    elevBox->move(posX,posY+boxSize/4);
    elevBox->setText(QString::number(model->getNumElevators()));
    elevBox->show();
    posX += boxSize*1.1;

    ql = new QLabel("# Floors",mainWindow);
    ql->setGeometry(QRect(posX,posY+boxSize*0.25,maxLabelLength,boxSize));
    ql->show();
    labelsGUI.append(ql);
    posX += boxSize*1.1;
    floorsBox = new QLineEdit(mainWindow);
    floorsBox->setGeometry(0,0,boxSize,boxSize);
    floorsBox->move(posX,posY+boxSize/4);
    floorsBox->setText(QString::number(model->getNumFloors()));
    floorsBox->show();
    posX += boxSize*1.1;

    ql = new QLabel("Min Floor",mainWindow);
    ql->setGeometry(QRect(posX,posY+boxSize*0.25,maxLabelLength,boxSize));
    ql->show();
    labelsGUI.append(ql);
    posX += boxSize*1.5;
    minFloorsBox = new QLineEdit(mainWindow);
    minFloorsBox->setGeometry(0,0,boxSize,boxSize);
    minFloorsBox->move(posX,posY+boxSize/4);
    minFloorsBox->setText(QString::number(model->getMinFloor()));
    minFloorsBox->show();
    //posX += boxSize*1.1;
    posX = gridStartX; posY += boxSize*1.5;

    dimensionsButton = new QPushButton("Change Dimensions",mainWindow);
    dimensionsButton->setGeometry(0,0,boxSize*3,boxSize);
    dimensionsButton->move(posX,posY+boxSize/4);
    connect(dimensionsButton, SIGNAL(released()), this, SLOT(restartNewDimensions()));

    posX += boxSize*3.3;

    randomButton = new QPushButton("Turn ON Random Events",mainWindow);
    randomButton->setGeometry(QRect(posX,posY + boxSize/4,boxSize*3.3,boxSize));
    randomButton->show();
    connect(randomButton, &QPushButton::released, [this]() { model->toggleRandomEvents(); });

}

void View::drawGrid()   {
    if(model == NULL)   return;
    for(int i=0; i<labels.size(); ++i)  delete labels[i];
    labels.clear();

    int m = model->getNumElevators();
    int n = model->getNumFloors();
    int i, endY, minFloor;
    minFloor = model->getMinFloor();
    endY = gridstartY + boxSizeGrid*n;
    QLabel *q;
    for(i=0; i<m; ++i)  {
        QString str = "E" + QString::number(i);
        q = new QLabel(str,mainWindow);
        q->setGeometry(QRect(gridStartX + boxSize + boxSizeGrid*(i+1),endY,boxSizeGrid,boxSizeGrid));
        q->show();
        labels.append(q);
    }
    for(i=minFloor; i<minFloor + n; ++i)  {
        QString str = ((i>=0)?"F":"B") + QString::number(qAbs(i)) + "  " + model->getFloors()->at(i-minFloor)->getSymbol();
        q = new QLabel(str,mainWindow);
        q->setGeometry(QRect(gridStartX,endY - boxSizeGrid*(i-minFloor+1),boxSize*2,boxSizeGrid));
        q->show();
        labels.append(q);
    }

    QList<Elevator*> *e = model->getElevators();
    for(i=0; i<m; ++i)   {
        Elevator *ele = e->at(i);
        QString str = ele->getSymbol();
        q = new QLabel(str,mainWindow);
        q->setGeometry(QRect(gridStartX + boxSize + boxSizeGrid*(i+1),endY - boxSizeGrid*(ele->getCurFloor() + 1),boxSizeGrid,boxSizeGrid));
        q->show();
        labels.append(q);
    }
}





