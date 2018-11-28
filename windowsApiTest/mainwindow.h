#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QString>
#include <windows.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

private:
    QVBoxLayout* m_vlayoutGloble;
    QPushButton* m_pbExecute;
    QPlainTextEdit* m_pteEcho;

private:
    void componentsInit();

public:
    void setEcho(QString context);

private slots:
    void onExecuteButtonClicked();
};

void setHook();
void unHook();
LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam );
LRESULT CALLBACK mouseProc(int nCode,WPARAM wParam,LPARAM lParam );

#endif // MAINWINDOW_H
