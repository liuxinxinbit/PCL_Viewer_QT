#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include <iostream>
#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QTextEdit>


//QStringList getFileNames(const QString &path);
pcl::visualization::PCLVisualizer::Ptr viewer;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    vtkOutputWindow::SetGlobalWarningDisplay(0);
    QString filepath = ui->textEdit->toPlainText();
    QStringList files = getFileNames(filepath);
    ui->listWidget->addItems(files);

    viewer.reset (new pcl::visualization::PCLVisualizer ("viewer", false));
    viewer->setBackgroundColor (0, 0, 0);
    viewer->setCameraPosition(-10,0,5,0.3,0,1,0);
    vtkWidget = new QVTKWidget(this,QFlag(0));
    ui->verticalLayout->addWidget(vtkWidget);
    ui->verticalLayout->update();
    vtkWidget->SetRenderWindow(viewer->getRenderWindow());
}

MainWindow::~MainWindow()
{
    ren->Delete();
    delete vtkWidget;
    delete ui;
}
QStringList MainWindow::getFileNames(const QString &path)
{
    QDir dir(path);
    QStringList nameFilters;
    nameFilters << "*.pcd";
    QStringList files = dir.entryList(nameFilters, QDir::Files|QDir::Readable, QDir::Name);
    return files;
}

void MainWindow::on_pushButton_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home/liuxinxin/ToolKit", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    this->ui->textEdit->setText(dir);


}


void MainWindow::on_pushButton_2_clicked()
{
    QString filepath = ui->textEdit->toPlainText();
    QStringList files = getFileNames(filepath);
    ui->listWidget->addItems(files);
}


void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QDir dir(ui->textEdit->toPlainText());
    QString absFilePath = dir.absoluteFilePath(item->text());
    //    std::cout<<absFilePath.toStdString()<<std::endl;


    pcl::PointCloud<pcl::PointXYZI>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZI>);

    if (pcl::io::loadPCDFile<pcl::PointXYZI> (absFilePath.toStdString(), *cloud) == -1) //* load the file
    {
        PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
    }
    {
        viewer->resetCamera();
        viewer->setCameraPosition(-10,0,5,0.3,0,1,0);
        viewer->removeAllPointClouds();
        pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZI> fildColor(cloud, "intensity");//按照intensity字段进行渲染
        viewer->addPointCloud<pcl::PointXYZI>(cloud, fildColor, "sample cloud");
        viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "sample cloud");//设置点云大小
        viewer->getRenderWindow()->Render();

    }
}
