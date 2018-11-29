#include "mainwindow.h"
#include "ui_mainwindow.h"

bool isConsumed = false;
HHOOK keyHook;
HHOOK mouseHook;
MainWindow* appAddress;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    componentsInit();
    ui->centralWidget->setLayout(m_vlayoutGloble);
    setWindowTitle(tr("WindowsApiTest"));

    appAddress = this;
}

MainWindow::~MainWindow()
{
    unHook();
    delete ui;
}

void MainWindow::componentsInit()
{
    m_pbExecute = new QPushButton(tr("execute"));
    connect(m_pbExecute, &m_pbExecute->clicked, this, &onExecuteButtonClicked);

    m_pteEcho = new QPlainTextEdit;

    m_vlayoutGloble = new QVBoxLayout;
    m_vlayoutGloble->addWidget(m_pteEcho);
    m_vlayoutGloble->addWidget(m_pbExecute);
}

void MainWindow::setEcho(QString context)
{
    m_pteEcho->appendPlainText(context);
}

void MainWindow::onExecuteButtonClicked()
{
    setHook();

    HANDLE handle = NULL;
//    handle = OpenProcess("FeiQ.exe");
}

void setHook()
{
    keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyProc, GetModuleHandle(NULL), 0);
    mouseHook = SetWindowsHookEx(WH_MOUSE_LL, mouseProc, GetModuleHandle(NULL), 0);
}

void unHook()
{
    UnhookWindowsHookEx(keyHook);
    UnhookWindowsHookEx(mouseHook);
}

LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(wParam == WM_KEYUP)
    {
        return isConsumed;
    }

    //在WH_KEYBOARD_LL模式下lParam 是指向KBDLLHOOKSTRUCT类型地址
    KBDLLHOOKSTRUCT *pkbhs = (KBDLLHOOKSTRUCT *) lParam;

    TCHAR szBuf[MAXBYTE] = {0};
    GetKeyNameText((pkbhs->scanCode<<16) + (pkbhs->flags<<24), szBuf, MAXBYTE);

    if(isConsumed)
        appAddress->setEcho("you pressed, but no effect");
    else
        appAddress->setEcho("0x" + QString::number(pkbhs->vkCode, 16) + ": " + QString::fromWCharArray(szBuf));

    if(pkbhs->vkCode == VK_F12)
    {
        unHook();
//        qApp->quit();
    }

    return isConsumed;//返回1表示截取消息不再传递,返回0表示不作处理,消息继续传递
}

LRESULT CALLBACK mouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if(wParam == WM_MOUSEMOVE || wParam == WM_LBUTTONUP
            || wParam == WM_RBUTTONUP || wParam == WM_MBUTTONUP)
    {
        return isConsumed;
    }

    if(isConsumed)
        appAddress->setEcho("you clicked, but no effect");
    else
        appAddress->setEcho("0x" + QString::number(wParam, 16));

    return isConsumed;
}

