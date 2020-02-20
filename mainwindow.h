#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <boost/filesystem.hpp>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    std::vector<std::string> ImageFileNamesVector;
    std::vector<std::string> ImageSubfoldersNamesVector;
    std::vector<std::string> ROITrainFileNamesVector;
    std::vector<std::string> ROITestFileNamesVector;
    std::vector<std::string> OptionsFileNamesVector;

    boost::filesystem::path QMaZdaFolder;
    boost::filesystem::path MZGeneratorPath;
    boost::filesystem::path MzTrainerPath;
    boost::filesystem::path MzTrainerPluginPath;
    boost::filesystem::path MzPredictPath;
    boost::filesystem::path MzClassyfierReductorPath;

    boost::filesystem::path ImageFolderRoot;
    boost::filesystem::path ImageFolder;
    boost::filesystem::path ROIFolder;
    boost::filesystem::path OptionsFolder;
    boost::filesystem::path FeaturesFolderRoot;
    boost::filesystem::path ClassyfiersFolderRoot;
    boost::filesystem::path ClassyfiersOptionFile;
    boost::filesystem::path PredictorOutputFolderRoot;
    boost::filesystem::path BatFolder;
    /*
    boost::filesystem::path
    boost::filesystem::path
    boost::filesystem::path
    */

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void ReloadPaths();

    bool OpenQMaZdaFolder();
    bool ReadImageSubfolders();
    bool OpenImageFolder(int position);
    bool OpenROIFolder();
    //bool OpenROITestFolder();
    bool OpenOptionsFolder();
    bool OpenFeauresFolder();
    bool OpenClassyfiersFolder();
    bool OpenClassyfiersOptionsFile();
    bool OpenPredictorOutputFotder();
    bool OpenBatFolder();

    void CreateBat();

private slots:
    void on_pushButtonOpenQMaZdaFolder_clicked();

    void on_pushButtonUpenImageFolder_clicked();

    void on_pushButtonOpenRoiFolder_clicked();

    void on_pushButtonOpenOptionsFolder_clicked();

    void on_pushButtonOpenMzFeaturesOutFolder_clicked();

    void on_pushButtonOpenClassyfiersFolder_clicked();

    void on_pushButtonOpenClassyfiersOptionsFile_clicked();

    void on_pushButtonOpenPredictorOutputFolder_clicked();

    void on_pushButtonOpenBatFolder_clicked();

    void on_lineEditImageFilePattern_returnPressed();

    void on_lineEditOptionsFilePattern_returnPressed();

    void on_lineEditMaZdaFolder_returnPressed();

    void on_lineEditImageFolder_returnPressed();

    void on_lineEditOptionsFolder_returnPressed();

    void on_lineEditMzFeaturesOutFolder_returnPressed();

    void on_lineEditClassyfiersFolder_returnPressed();

    void on_lineEditClassyfiersOptionsFile_returnPressed();

    void on_lineEditPredictorOutputFolder_returnPressed();

    void on_lineEditBatFolder_returnPressed();

    void on_pushButtonClearOut_clicked();

    void on_pushButtonReload_clicked();

    void on_pushButtonCreateBatFile_clicked();

    void on_listWidgetImageSubfolders_currentRowChanged(int currentRow);

    void on_lineEditROITrainFilePattern_returnPressed();

    void on_lineEditROITestFilePattern_returnPressed();

    void on_lineEditROIFolder_returnPressed();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
