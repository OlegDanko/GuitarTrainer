#include "FretBoard.hpp"

#include <QApplication>
#include <QFile>
#include <QtWidgets>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFile styleFile(":/style.css");
    styleFile.open(QFile::ReadOnly);
    QString style(styleFile.readAll());
    app.setStyleSheet(style);

    QWidget widget;
    widget.setLayout(new QVBoxLayout());
    FretBoard board(13);
    widget.layout()->addWidget(board.getWidget());
    widget.layout()->addItem(new QSpacerItem(1000000,
                                             1000000,
                                             QSizePolicy::Expanding));

    widget.show();

    return app.exec();
}
