#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <string>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

using namespace boost;
using namespace std;
using namespace boost::filesystem;
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          My functions outside the Mainwindow class
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
void ReadFolder(path FileFolder, std::vector<std::string> *FileNamesVector, string FilePaternStr = ".+")
{
    FileNamesVector->clear();
    if (!exists(FileFolder))
    {
        return;
    }
    if (!is_directory(FileFolder))
    {
        return;
    }
    regex FilePattern(FilePaternStr);
    for (directory_entry& FileToProcess : directory_iterator(FileFolder))
    {

        if (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern ))
            continue;
        path PathLocal = FileToProcess.path();

        FileNamesVector->push_back(PathLocal.filename().string());
    }
    std::sort(FileNamesVector->begin(),FileNamesVector->end());
}
//------------------------------------------------------------------------------------------------------------------------------
void ReadSubfolders(path FileFolder, std::vector<std::string> *SubfoldersNamesVector)//, string FilePaternStr = ".+")
{
    SubfoldersNamesVector->clear();
    if (!exists(FileFolder))
    {
        return;
    }
    if (!is_directory(FileFolder))
    {
        return;
    }
    //regex FilePattern(FilePaternStr);
    for (directory_entry& ElementToProcess : directory_iterator(FileFolder))
    {

        //if (!regex_match(FileToProcess.path().filename().string().c_str(), FilePattern ))
        //    continue;
        path PathLocal = ElementToProcess.path();
        if(is_directory(PathLocal))
        {
            SubfoldersNamesVector->push_back(PathLocal.filename().string());
        }
    }
    if(SubfoldersNamesVector->empty())
        return;
    std::sort(SubfoldersNamesVector->begin(),SubfoldersNamesVector->end());
}
//------------------------------------------------------------------------------------------------------------------------------
string StringVectorToString(std::vector<std::string> FileNamesVector)
{
    string OutStr = "";
    if(FileNamesVector.empty())
        return OutStr;
    for(vector<string>::iterator iFileNamesVector = FileNamesVector.begin(); iFileNamesVector != FileNamesVector.end(); iFileNamesVector++)
    {
        OutStr += *iFileNamesVector + "\n";
    }

    return OutStr;
}
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          Constructor Destructor
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ReloadPaths();
}

MainWindow::~MainWindow()
{
    delete ui;
}
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          My functions in the Mainwindow class
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::ReloadPaths()
{
    OpenQMaZdaFolder();
    ReadImageSubfolders();
    OpenImageFolder(ui->listWidgetImageSubfolders->currentRow());
    OpenROIFolder();
    OpenOptionsFolder();
    OpenFeauresFolder();
    OpenClassyfiersFolder();
    OpenClassyfiersOptionsFile();
    OpenPredictorOutputFotder();
    OpenBatFolder();
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenQMaZdaFolder()
{
    QMaZdaFolder = path(ui->lineEditMaZdaFolder->text().toStdWString());

    if (!exists(QMaZdaFolder))
    {
        ui->textEditOut->append(QString::fromStdString("QMaZda folder : " + QMaZdaFolder.string()+ " not exists "));
        return 0;
    }
    if (!is_directory(QMaZdaFolder))
    {
        ui->textEditOut->append(QString::fromStdString( "QMaZda folder : " + QMaZdaFolder.string()+ " This is not a directory path "));
        return 0;
    }
    ui->lineEditMaZdaFolder->setText(QString::fromStdString(QMaZdaFolder.string()));
    MZGeneratorPath = QMaZdaFolder;
    MZGeneratorPath.append("MzGenerator.exe");
    if (!exists(MZGeneratorPath))
    {
        ui->textEditOut->append(QString::fromStdString("File : " + MZGeneratorPath.string()+ " not exists "));
        return 0;
    }
    MzTrainerPath = QMaZdaFolder;
    MzTrainerPath.append("MzTrainer.exe");
    if (!exists(MzTrainerPath))
    {
        ui->textEditOut->append(QString::fromStdString("File : " + MzTrainerPath.string()+ " not exists "));
        return 0;
    }
    MzPredictPath = QMaZdaFolder;
    MzPredictPath.append("MzPredict.exe");
    if (!exists(MzPredictPath))
    {
        ui->textEditOut->append(QString::fromStdString("File : " + MzPredictPath.string()+ " not exists "));
        return 0;
    }
    MzTrainerPluginPath = QMaZdaFolder;
    MzTrainerPluginPath.append("LdaPlugin.dll");
    if (!exists(MzTrainerPluginPath))
    {
        ui->textEditOut->append(QString::fromStdString("File : " + MzTrainerPluginPath.string()+ " not exists "));
        return 0;
    }
    MzClassyfierReductorPath = QMaZdaFolder;
    MzClassyfierReductorPath.append("MzClasyfierReductor.exe");
    if (!exists(MzClassyfierReductorPath))
    {
        ui->textEditOut->append(QString::fromStdString("File : " + MzClassyfierReductorPath.string()+ " not exists "));
        return 0;
    }
    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::ReadImageSubfolders()
{
    ImageFolderRoot = path(ui->lineEditImageFolder->text().toStdWString());
    ImageSubfoldersNamesVector.clear();
    ui->listWidgetImageSubfolders->clear();
    if(!ui->checkBoxUseSubfolders->checkState())
    {
        return 0;
    }

    if (!exists(ImageFolderRoot))
    {
        ui->textEditOut->append(QString::fromStdString("Image folder root : " + ImageFolderRoot.string()+ " not exists "));
        return 0;
    }
    if (!is_directory(ImageFolderRoot))
    {
        ui->textEditOut->append(QString::fromStdString( " Image folder root: " + ImageFolderRoot.string()+ " This is not a directory path "));
        return 0;
    }
    ReadSubfolders(ImageFolderRoot, &ImageSubfoldersNamesVector);
    if(ImageSubfoldersNamesVector.empty())
        return 0;
    size_t count = ImageSubfoldersNamesVector.size();
    for(size_t i = 0; i < count; i++)
    {
        ui->listWidgetImageSubfolders->addItem(QString::fromStdString(ImageSubfoldersNamesVector[i]));
    }
    ui->listWidgetImageSubfolders->setCurrentRow(0);
    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenImageFolder(int position)
{
    if(position < 0)
    {
        ui->textEditOut->append("subfolder index below 0");
    }
    ImageFolder = ImageFolderRoot;
    ImageFileNamesVector.clear();
    ui->textEditImageFiles->clear();

    if(ui->checkBoxUseSubfolders->checkState())
    {
        if(ImageSubfoldersNamesVector.empty())
        {
            ui->textEditOut->append(QString::fromStdString("No Subfolders"));
            return 0;
        }
        int vectorSize = ImageSubfoldersNamesVector.size();
        //int position = ui->listWidgetImageSubfolders->currentRow();
       if(vectorSize <= position)
       {
           ui->textEditOut->append(QString::fromStdString("too large subfolder index"));
           return 0;
       }
        ImageFolder.append(ImageSubfoldersNamesVector[position]);
    }

    if (!exists(ImageFolder))
    {
        ui->textEditOut->append(QString::fromStdString("Image folder : " + ImageFolder.string()+ " not exists "));
        return 0;
    }
    if (!is_directory(ImageFolder))
    {
        ui->textEditOut->append(QString::fromStdString( " Image folder : " + ImageFolder.string()+ " This is not a directory path "));
        return 0;
    }
    ReadFolder(ImageFolder, &ImageFileNamesVector, ui->lineEditImageFilePattern->text().toStdString() );

    if(ImageFileNamesVector.empty())
    {
        return 0;
    }
    ui->textEditImageFiles->append(QString::fromStdString(StringVectorToString(ImageFileNamesVector)));
    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenROIFolder()
{
    ROIFolder = path(ui->lineEditROIFolder->text().toStdString());

    ROITrainFileNamesVector.clear();
    ROITestFileNamesVector.clear();

    ui->textEditROITrainFiles->clear();
    if (!exists(ROIFolder))
    {
        ui->textEditOut->append(QString::fromStdString("ROI folder : " + ROIFolder.string()+ " not exists "));
        return 0;
    }
    if (!is_directory(ROIFolder))
    {
        ui->textEditOut->append(QString::fromStdString( " Image folder : " + ROIFolder.string()+ " This is not a directory path "));
        return 0;
    }

    ReadFolder(ROIFolder, &ROITrainFileNamesVector, ui->lineEditROITrainFilePattern->text().toStdString() );
    if(ROITrainFileNamesVector.empty())
    {
        return 0;
    }
    ui->textEditROITrainFiles->append(QString::fromStdString(StringVectorToString(ROITrainFileNamesVector)));

    ui->textEditROITestFiles->clear();
    ReadFolder(ROIFolder, &ROITestFileNamesVector, ui->lineEditROITestFilePattern->text().toStdString() );
    if(ROITestFileNamesVector.empty())
    {
        return 0;
    }
    ui->textEditROITestFiles->append(QString::fromStdString(StringVectorToString(ROITestFileNamesVector)));
    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenOptionsFolder()
{
    OptionsFolder = path(ui->lineEditOptionsFolder->text().toStdString());
    OptionsFileNamesVector.clear();
    ui->textEditOptionsFiles->clear();

    if (!exists(OptionsFolder))
    {
        ui->textEditOut->append(QString::fromStdString("Options folder : " + OptionsFolder.string()+ " not exists "));
        return 0;
    }
    if (!is_directory(OptionsFolder))
    {
        ui->textEditOut->append(QString::fromStdString( " Options folder : " + OptionsFolder.string()+ " This is not a directory path "));
        return 0;
    }

    string optionsPattern;

    if(ui->checkBoxUseSubfolders->checkState())
    {
        optionsPattern = ".+" +
                         ui->lineEditFeatureFamily->text().toStdString() +
                         ui->lineEditOptionsFilePattern->text().toStdString();
    }
    else
    {
        optionsPattern = ui->lineEditOptionsFilePattern->text().toStdString();
    }

    ReadFolder(OptionsFolder, &OptionsFileNamesVector, optionsPattern );
    if(OptionsFileNamesVector.empty())
    {
        return 0;
    }
    ui->textEditOptionsFiles->append(QString::fromStdString(StringVectorToString(OptionsFileNamesVector)));
    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenFeauresFolder()
{
    FeaturesFolderRoot = path(ui->lineEditMzFeaturesOutFolder->text().toStdString());
    if(!exists(FeaturesFolderRoot))
    {
        ui->textEditOut->append(QString::fromStdString("MzFeatutes folder : " + FeaturesFolderRoot.string()+ " not exists "));
        return 0;
    }
    if (!is_directory(FeaturesFolderRoot))
    {
        ui->textEditOut->append(QString::fromStdString( " MzFeatutes folder : " + FeaturesFolderRoot.string()+ " This is not a directory path "));
        return 0;
    }

    if(ui->checkBoxUseSubfolders->checkState())
    {
        if(ImageSubfoldersNamesVector.empty())
        {
            ui->textEditOut->append(QString::fromStdString("No subfolders"));
            return 0;
        }

        FeaturesFolderRoot.append(ui->lineEditFeatureFamily->text().toStdString());

        size_t count = ImageSubfoldersNamesVector.size();
        for(size_t i = 0; i < count; i++)
        {
            path FeaturesFolder = FeaturesFolderRoot;
            FeaturesFolder.append(ImageSubfoldersNamesVector[i]);
            if (!exists(FeaturesFolder))
            {
                ui->textEditOut->append(QString::fromStdString("MzFeatutes folder : " + FeaturesFolder.string()+ " not exists "));
                return 0;
            }
            if (!is_directory(FeaturesFolder))
            {
                ui->textEditOut->append(QString::fromStdString( " MzFeatutes folder : " + FeaturesFolder.string()+ " This is not a directory path "));
                return 0;
            }
        }
    }
    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenClassyfiersFolder()
{
    ClassyfiersFolderRoot = path(ui->lineEditClassyfiersFolder->text().toStdString());
    if (!exists(ClassyfiersFolderRoot))
    {
        ui->textEditOut->append(QString::fromStdString("Classyfiers folder Root:" + ClassyfiersFolderRoot.string()+ " not exists "));
        return 0;
    }
    if (!is_directory(ClassyfiersFolderRoot))
    {
        ui->textEditOut->append(QString::fromStdString( "Classyfiersfolder Root : " + ClassyfiersFolderRoot.string()+ " This is not a directory path "));
        return 0;
    }

    if(ui->checkBoxUseSubfolders->checkState())
    {
        ClassyfiersFolderRoot.append(ui->lineEditFeatureFamily->text().toStdString());

        if(ImageSubfoldersNamesVector.empty())
        {
            ui->textEditOut->append(QString::fromStdString("No subfolders"));
            return 0;
        }

        size_t count = ImageSubfoldersNamesVector.size();
        for(size_t i = 0; i < count; i++)
        {
            path ClassyfiersFolder = ClassyfiersFolderRoot;
            ClassyfiersFolder.append(ImageSubfoldersNamesVector[i]);
            if (!exists(ClassyfiersFolder))
            {
                ui->textEditOut->append(QString::fromStdString("Classyfiers folder : " + ClassyfiersFolder.string()+ " not exists "));
                return 0;
            }
            if (!is_directory(ClassyfiersFolder))
            {
                ui->textEditOut->append(QString::fromStdString( "Classyfiers folder : " + ClassyfiersFolder.string()+ " This is not a directory path "));
                return 0;
            }
        }
    }
    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenClassyfiersOptionsFile()
{
    ClassyfiersOptionFile = path(ui->lineEditClassyfiersOptionsFile->text().toStdString());
    if (!exists(ClassyfiersOptionFile))
    {
        ui->textEditOut->append(QString::fromStdString("Classyfiers Options File : " + ClassyfiersOptionFile.string()+ " not exists "));
        return 0;
    }
    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenPredictorOutputFotder()
{
    PredictorOutputFolderRoot = path(ui->lineEditPredictorOutputFolder->text().toStdString());

    if (!exists(PredictorOutputFolderRoot))
    {
        ui->textEditOut->append(QString::fromStdString("Predictor folder Root:" + PredictorOutputFolderRoot.string()+ " not exists "));
        return 0;
    }
    if (!is_directory(PredictorOutputFolderRoot))
    {
        ui->textEditOut->append(QString::fromStdString( "Predictor folder Root: " + PredictorOutputFolderRoot.string()+ " This is not a directory path "));
        return 0;
    }

    if(ui->checkBoxUseSubfolders->checkState())
    {
        PredictorOutputFolderRoot.append(ui->lineEditFeatureFamily->text().toStdString());
        if(ImageSubfoldersNamesVector.empty())
        {
            ui->textEditOut->append(QString::fromStdString("No subfolders"));
            return 0;
        }

        size_t count = ImageSubfoldersNamesVector.size();
        for(size_t i = 0; i < count; i++)
        {
            path PredictorOutputFolder = PredictorOutputFolderRoot;
            PredictorOutputFolder.append(ImageSubfoldersNamesVector[i]);
            if (!exists(PredictorOutputFolder))
            {
                ui->textEditOut->append(QString::fromStdString("Predictor folder : " + PredictorOutputFolder.string()+ " not exists "));
                return 0;
            }
            if (!is_directory(PredictorOutputFolder))
            {
                ui->textEditOut->append(QString::fromStdString( "Predictor folder : " + PredictorOutputFolder.string()+ " This is not a directory path "));
                return 0;
            }
        }
    }

    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
bool MainWindow::OpenBatFolder()
{
    BatFolder = path(ui->lineEditBatFolder->text().toStdString());
    if (!exists(BatFolder))
    {
        ui->textEditOut->append(QString::fromStdString("Image folder : " + BatFolder.string()+ " not exists "));
        return 0;
    }
    if (!is_directory(BatFolder))
    {
        ui->textEditOut->append(QString::fromStdString( " Image folder : " + BatFolder.string()+ " This is not a directory path "));
        return 0;
    }
    return 1;
}
//------------------------------------------------------------------------------------------------------------------------------
void MainWindow::CreateBat()
{
    ui->textEditOut->clear();

    if(!OpenQMaZdaFolder())
    {
        return;
    }

    if(!OpenOptionsFolder())
    {
        return;
    }
    int optionsCount = OptionsFileNamesVector.size();

    if(!OpenBatFolder())
    {
        return;
    }

    if(!OpenROIFolder())
    {
        return;
    }

    if(!OpenFeauresFolder())
    {
        return;
    }

    if(!OpenClassyfiersFolder())
    {
        return;
    }

    if(!OpenPredictorOutputFotder())
    {
        return;
    }

    if(ROITestFileNamesVector.size() != ROITrainFileNamesVector.size())
    {
        ui->textEditOut->append("Different number rois in test and training set");
        return;
    }

    if(ui->checkBoxUseSubfolders->checkState())
    {
        if(!ReadImageSubfolders())
        {
            return;
        }

        size_t subfolderCount = ImageSubfoldersNamesVector.size();

        //--------------------------------Subfolders------------------------------------------------------
        for(size_t iSubFolder = 0; iSubFolder < subfolderCount; iSubFolder++)
        {
            if(!OpenImageFolder(iSubFolder))
            {
                return;
            }
            size_t imagesCount = ImageFileNamesVector.size();
            if(ImageFileNamesVector.size() != ROITrainFileNamesVector.size())
            {
                ui->textEditOut->append("Different number of images and rois");
                return;
            }

            path OutFeaturesFolder = FeaturesFolderRoot;
            OutFeaturesFolder.append(ImageSubfoldersNamesVector[iSubFolder]);
            if (!exists(OutFeaturesFolder))
            {
                ui->textEditOut->append(QString::fromStdString("Features folder : " + OutFeaturesFolder.string()+ " not exists "));
                return;
            }
            if (!is_directory(OutFeaturesFolder))
            {
                ui->textEditOut->append(QString::fromStdString( " Features folder : " + OutFeaturesFolder.string()+ " This is not a directory path "));
                return;
            }
            string prefix = ui->lineEditOutFilePrefix->text().toStdString() +
                            ImageSubfoldersNamesVector[iSubFolder];

            string BatFeaturesFileContent = "cls\n";
            string BatTrainingFileContent = "cls\n";
            string BatPredictFileContent = "cls\n";
            string BatClassyfierReductorFileContent = "cls\n";

            for(int o = 0; o < optionsCount; o++)
            {
                string BatCommonFileContent = "";
                BatCommonFileContent += "rem \"------------------------------------------------------------------------------------------\"\n";
                BatCommonFileContent += "rem \"  " + OptionsFileNamesVector[o] + "  \"\n";
                BatCommonFileContent += "rem \"------------------------------------------------------------------------------------------\"\n";

                path OptionsFile = OptionsFolder;
                OptionsFile.append(OptionsFileNamesVector[o]);

                path FeaturesTrainFile = FeaturesFolderRoot;
                FeaturesTrainFile.append(ImageSubfoldersNamesVector[iSubFolder]);
                FeaturesTrainFile.append("train_" + prefix + OptionsFile.stem().string() + ".csv");

                path FeaturesTestFile = FeaturesFolderRoot;
                FeaturesTestFile.append(ImageSubfoldersNamesVector[iSubFolder]);
                FeaturesTestFile.append("test_" + prefix + OptionsFile.stem().string() + ".csv");

                path ClassyfiersFile = ClassyfiersFolderRoot;
                ClassyfiersFile.append(ImageSubfoldersNamesVector[iSubFolder]);
                ClassyfiersFile.append(prefix + OptionsFile.stem().string() + "Classyfier.txt");

                path PredictorOutFile = PredictorOutputFolderRoot;
                PredictorOutFile.append(ImageSubfoldersNamesVector[iSubFolder]);
                PredictorOutFile.append(prefix + OptionsFile.stem().string() + "Prediction.txt");
                // ---------------------------- Features Bat  -------------------------------------------------------------------

                if(ui->checkBoxCreateFeaturesBat->checkState())
                {
                    string BatTrainingFeaturesFileContent = "";
                    BatTrainingFeaturesFileContent += "rem \"------------------------------------------------------------------------------------------\"\n";
                    BatTrainingFeaturesFileContent += "rem \"    Training features   \"\n";
                    BatTrainingFeaturesFileContent += "rem \"------------------------------------------------------------------------------------------\"\n";

                    string BatTestingFeaturesFileContent = "";
                    BatTestingFeaturesFileContent += "rem \"------------------------------------------------------------------------------------------\"\n";
                    BatTestingFeaturesFileContent += "rem \" Testing features\"\n";
                    BatTestingFeaturesFileContent += "rem \"------------------------------------------------------------------------------------------\"\n";

                    if(!ui->checkBoxCommonTrainAdTest->checkState())
                    {
                        path ImFile = ImageFolder;
                        ImFile.append(ImageFileNamesVector[0]);

                        path RoiTrainFile = ROIFolder;
                        RoiTrainFile.append(ROITrainFileNamesVector[0]);

                        path RoiTestFile = ROIFolder;
                        RoiTestFile.append(ROITestFileNamesVector[0]);


                        BatTrainingFeaturesFileContent += MZGeneratorPath.string()
                                          + " -m roi -i " + ImFile.string()
                                          + " -r " + RoiTrainFile.string()
                                          + "    -o " + FeaturesTrainFile.string()
                                          + " -f " + OptionsFile.string()
                                          + "\n";

                        BatTestingFeaturesFileContent += MZGeneratorPath.string()
                                          + " -m roi -i " + ImFile.string()
                                          + " -r " + RoiTestFile.string()
                                          + "    -o " + FeaturesTestFile.string()
                                          + " -f " + OptionsFile.string()
                                          + "\n";

                        for(size_t i = 1; i < imagesCount; i++)
                        {
                            ImFile = ImageFolder;
                            ImFile.append(ImageFileNamesVector[i]);

                            RoiTrainFile = ROIFolder;
                            RoiTrainFile.append(ROITrainFileNamesVector[i]);

                            RoiTestFile = ROIFolder;
                            RoiTestFile.append(ROITestFileNamesVector[i]);

                            BatTrainingFeaturesFileContent += MZGeneratorPath.string()
                                              + " -m roi -i " + ImFile.string()
                                              + " -r " + RoiTrainFile.string()
                                              + " -a -o " + FeaturesTrainFile.string()
                                              + "\n";

                            BatTestingFeaturesFileContent += MZGeneratorPath.string()
                                              + " -m roi -i " + ImFile.string()
                                              + " -r " + RoiTestFile.string()
                                              + " -a -o " + FeaturesTestFile.string()
                                              + "\n";
                        }
                    }
                    else
                    {
                        path ImFile = ImageFolder;
                        ImFile.append(ImageFileNamesVector[0]);

                        path RoiTrainFile = ROIFolder;
                        RoiTrainFile.append(ROITrainFileNamesVector[0]);

                        BatTrainingFeaturesFileContent += MZGeneratorPath.string()
                                          + " -m roi -i " + ImFile.string()
                                          + " -r " + RoiTrainFile.string()
                                          + "    -o " + FeaturesTrainFile.string()
                                          + " -f " + OptionsFile.string()
                                          + "\n";

                        for(size_t i = 2; i < imagesCount; i+=2)
                        {
                            ImFile = ImageFolder;
                            ImFile.append(ImageFileNamesVector[i]);

                            RoiTrainFile = ROIFolder;
                            RoiTrainFile.append(ROITrainFileNamesVector[i]);

                            BatTrainingFeaturesFileContent += MZGeneratorPath.string()
                                              + " -m roi -i " + ImFile.string()
                                              + " -r " + RoiTrainFile.string()
                                              + " -a -o " + FeaturesTrainFile.string()
                                              + "\n";
                        }

                        ImFile = ImageFolder;
                        ImFile.append(ImageFileNamesVector[1]);

                        path RoiTestFile = ROIFolder;
                        RoiTestFile.append(ROITestFileNamesVector[1]);




                        BatTestingFeaturesFileContent += MZGeneratorPath.string()
                                          + " -m roi -i " + ImFile.string()
                                          + " -r " + RoiTestFile.string()
                                          + "    -o " + FeaturesTestFile.string()
                                          + " -f " + OptionsFile.string()
                                          + "\n";

                        for(size_t i = 3; i < imagesCount; i+=2)
                        {
                            ImFile = ImageFolder;
                            ImFile.append(ImageFileNamesVector[i]);

                            RoiTestFile = ROIFolder;
                            RoiTestFile.append(ROITestFileNamesVector[i]);

                            BatTestingFeaturesFileContent += MZGeneratorPath.string()
                                              + " -m roi -i " + ImFile.string()
                                              + " -r " + RoiTestFile.string()
                                              + " -a -o " + FeaturesTestFile.string()
                                              + "\n";
                        }
                    }

                    BatFeaturesFileContent += BatCommonFileContent;
                    BatFeaturesFileContent += BatTrainingFeaturesFileContent;
                    BatFeaturesFileContent += BatTestingFeaturesFileContent;
                }

                // ---------------------------- Training Bat  -------------------------------------------------------------------

                if(ui->checkBoxCreateTrainingBat->checkState())
                {
                    BatTrainingFileContent += MzTrainerPath.string()
                            + " -p " + MzTrainerPluginPath.string()
                            + " -c " + ClassyfiersOptionFile.string()
                            + " -i " + FeaturesTrainFile.string()
                            + " -o " + ClassyfiersFile.string()
                            + "\n";
                }

                // ---------------------------- Prediction Bat  -------------------------------------------------------------------

                if(ui->checkBoxCreatePredictorBat->checkState())
                {
                    BatPredictFileContent += MzPredictPath.string()
                                      + " -c " + ClassyfiersFile.string()
                                      + " -i " + FeaturesTestFile.string()
                                      + " -o " + PredictorOutFile.string()
                                      + " -v \n";
                }

                // ---------------------------- Class reductor Bat  -------------------------------------------------------------------

                if(ui->checkBoxCreateClassyfierReductorBat->checkState())
                {
                    BatClassyfierReductorFileContent += MzClassyfierReductorPath.string()
                            + " 3 " + ClassyfiersFile.string()
                            + "\n";
                }

            }
            if(ui->checkBoxCreateFeaturesBat->checkState())
            {
                path BatFeaturesFile = BatFolder;
                BatFeaturesFile.append(ui->lineEditBatFileName->text().toStdString() +
                                       "Features_"+
                               ui->lineEditFeatureFamily->text().toStdString() +
                               ImageSubfoldersNamesVector[iSubFolder] +
                               ".bat");
                std::ofstream FileToSave(BatFeaturesFile.string());
                if (!FileToSave.is_open())
                {
                    ui->textEditOut->append(QString::fromStdString("Bat File Not Saved"));
                }
                FileToSave << BatFeaturesFileContent;
                FileToSave.close();
                BatFeaturesFileContent.clear();
            }
            if(ui->checkBoxCreateTrainingBat->checkState())
            {
                path BatTrainingFile = BatFolder;
                BatTrainingFile.append(ui->lineEditBatFileName->text().toStdString() +
                                       "Train_"+
                               ui->lineEditFeatureFamily->text().toStdString() +
                               ImageSubfoldersNamesVector[iSubFolder] +
                               + ".bat");
                std::ofstream FileToSave(BatTrainingFile.string());
                if (!FileToSave.is_open())
                {
                    ui->textEditOut->append(QString::fromStdString("Training Bat File Not Saved"));
                }
                FileToSave << BatTrainingFileContent;
                FileToSave.close();
                BatTrainingFileContent.clear();
            }

            if(ui->checkBoxCreatePredictorBat->checkState())
            {
                path BatPredictFile = BatFolder;
                BatPredictFile.append(ui->lineEditBatFileName->text().toStdString() +
                                       "Predict_"+
                               ui->lineEditFeatureFamily->text().toStdString() +
                               ImageSubfoldersNamesVector[iSubFolder] +
                               + ".bat");
                std::ofstream FileToSave(BatPredictFile.string());
                if (!FileToSave.is_open())
                {
                    ui->textEditOut->append(QString::fromStdString("Predict Bat File Not Saved"));
                }
                FileToSave << BatPredictFileContent;
                FileToSave.close();
                BatPredictFileContent.clear();
            }

            if(ui->checkBoxCreateClassyfierReductorBat->checkState())
            {
                //BatClassyfierReductorFileContent += "pause\n";
                path BatTrainingFile = BatFolder;
                BatTrainingFile.append(ui->lineEditBatFileName->text().toStdString() +
                                       "Reduce_"+
                               ui->lineEditFeatureFamily->text().toStdString() +
                               ImageSubfoldersNamesVector[iSubFolder] +
                               + ".bat");
                std::ofstream FileToSave(BatTrainingFile.string());
                if (!FileToSave.is_open())
                {
                    ui->textEditOut->append(QString::fromStdString("Reductor Bat File Not Saved"));
                }
                FileToSave << BatClassyfierReductorFileContent;
                FileToSave.close();
                BatTrainingFileContent.clear();
            }
        }
        //--------------------------------Subfolders------------------------------------------------------
    }
}
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------
//          Slots
//------------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------------

void MainWindow::on_pushButtonOpenQMaZdaFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);

    path tempDir = path(ui->lineEditMaZdaFolder->text().toStdString());
    if(exists(tempDir) && is_directory(tempDir))
    {
        dialog.setDirectory( QString::fromStdString(tempDir.string()));
    }

    if(dialog.exec())
    {
        ui->lineEditMaZdaFolder->setText(dialog.directory().path());
        //QMaZdaFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenQMaZdaFolder();
}

void MainWindow::on_pushButtonUpenImageFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);

    path tempDir = path(ui->lineEditImageFolder->text().toStdString());
    if(exists(tempDir) && is_directory(tempDir))
    {
        dialog.setDirectory( QString::fromStdString(tempDir.string()));
    }

    if(dialog.exec())
    {
        ui->lineEditImageFolder->setText(dialog.directory().path());
        //ImageFolder = dialog.directory().path().toStdWString();
    }
    else
        return;

    ReadImageSubfolders();
    OpenImageFolder(ui->listWidgetImageSubfolders->currentRow());
}

void MainWindow::on_pushButtonOpenRoiFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);

    path tempDir = path(ui->lineEditROIFolder->text().toStdString());
    if(exists(tempDir) && is_directory(tempDir))
    {
        dialog.setDirectory( QString::fromStdString(tempDir.string()));
    }

    if(dialog.exec())
    {
        ui->lineEditROIFolder->setText(dialog.directory().path());
        //ROIFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenROIFolder();
}

void MainWindow::on_pushButtonOpenOptionsFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);

    path tempDir = path(ui->lineEditOptionsFolder->text().toStdString());
    if(exists(tempDir) && is_directory(tempDir))
    {
        dialog.setDirectory( QString::fromStdString(tempDir.string()));
    }

    if(dialog.exec())
    {
        ui->lineEditOptionsFolder->setText(dialog.directory().path());
        //OptionsFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenOptionsFolder();
}

void MainWindow::on_pushButtonOpenMzFeaturesOutFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);

    path tempDir = path(ui->lineEditMzFeaturesOutFolder->text().toStdString());
    if(exists(tempDir) && is_directory(tempDir))
    {
        dialog.setDirectory( QString::fromStdString(tempDir.string()));
    }

    if(dialog.exec())
    {
        ui->lineEditMzFeaturesOutFolder->setText(dialog.directory().path());
        //MzFeaturesOutFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenFeauresFolder();
}

void MainWindow::on_pushButtonOpenClassyfiersFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);

    path tempDir = path(ui->lineEditClassyfiersFolder->text().toStdString());
    if(exists(tempDir) && is_directory(tempDir))
    {
        dialog.setDirectory( QString::fromStdString(tempDir.string()));
    }

    if(dialog.exec())
    {
        ui->lineEditClassyfiersFolder->setText(dialog.directory().path());
        //ClassyfiersFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenClassyfiersFolder();
}

void MainWindow::on_pushButtonOpenClassyfiersOptionsFile_clicked()
{
    QFileDialog dialog(this, "Open File");
    dialog.setFileMode(QFileDialog::AnyFile);

    path tempDir = path(ui->lineEditClassyfiersOptionsFile->text().toStdString());
    if(exists(tempDir) && is_directory(tempDir))
    {
        dialog.setDirectory( QString::fromStdString(tempDir.string()));
    }

    if(dialog.exec())
    {
        ui->lineEditClassyfiersOptionsFile->setText(dialog.selectedFiles()[0]);
        //ClassyfiersOptionFile = path(dialog.selectedFiles()[0].toStdWString());
    }
    else
        return;
    OpenClassyfiersOptionsFile();
}

void MainWindow::on_pushButtonOpenPredictorOutputFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);

    path tempDir = path(ui->lineEditPredictorOutputFolder->text().toStdString());
    if(exists(tempDir) && is_directory(tempDir))
    {
        dialog.setDirectory( QString::fromStdString(tempDir.string()));
    }

    if(dialog.exec())
    {
        ui->lineEditPredictorOutputFolder->setText(dialog.directory().path());
        //PredictorOutputFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenPredictorOutputFotder();
}

void MainWindow::on_pushButtonOpenBatFolder_clicked()
{
    QFileDialog dialog(this, "Open Folder");
    dialog.setFileMode(QFileDialog::Directory);

    path tempDir = path(ui->lineEditBatFolder->text().toStdString());
    if(exists(tempDir) && is_directory(tempDir))
    {
        dialog.setDirectory( QString::fromStdString(tempDir.string()));
    }

    if(dialog.exec())
    {
        ui->lineEditBatFolder->setText(dialog.directory().path());
        //BatFolder = dialog.directory().path().toStdWString();
    }
    else
        return;
    OpenBatFolder();
}

void MainWindow::on_lineEditImageFilePattern_returnPressed()
{
    ReadImageSubfolders();
    OpenImageFolder(ui->listWidgetImageSubfolders->currentRow());
}

void MainWindow::on_lineEditOptionsFilePattern_returnPressed()
{
    OpenOptionsFolder();
}

void MainWindow::on_lineEditMaZdaFolder_returnPressed()
{
    OpenQMaZdaFolder();
}

void MainWindow::on_lineEditImageFolder_returnPressed()
{
    ReadImageSubfolders();
    OpenImageFolder(ui->listWidgetImageSubfolders->currentRow());
}

void MainWindow::on_lineEditOptionsFolder_returnPressed()
{
    OpenOptionsFolder();
}

void MainWindow::on_lineEditMzFeaturesOutFolder_returnPressed()
{
    OpenFeauresFolder();
}

void MainWindow::on_lineEditClassyfiersFolder_returnPressed()
{
    OpenClassyfiersFolder();
}

void MainWindow::on_lineEditClassyfiersOptionsFile_returnPressed()
{
    OpenClassyfiersOptionsFile();
}

void MainWindow::on_lineEditPredictorOutputFolder_returnPressed()
{
    OpenPredictorOutputFotder();
}

void MainWindow::on_lineEditBatFolder_returnPressed()
{
    OpenBatFolder();
}

void MainWindow::on_pushButtonClearOut_clicked()
{
    ui->textEditOut->clear();
}

void MainWindow::on_pushButtonReload_clicked()
{
    ReloadPaths();
}

void MainWindow::on_pushButtonCreateBatFile_clicked()
{
    CreateBat();
}

void MainWindow::on_listWidgetImageSubfolders_currentRowChanged(int currentRow)
{
    if(currentRow < 0)
        return;
    OpenImageFolder(currentRow);
}

void MainWindow::on_lineEditROITrainFilePattern_returnPressed()
{
    if(ui->checkBoxCommonTrainAdTest->checkState())
        ui->lineEditROITestFilePattern->setText(ui->lineEditROITrainFilePattern->text());
    OpenROIFolder();
}

void MainWindow::on_lineEditROITestFilePattern_returnPressed()
{
    OpenROIFolder();
}

void MainWindow::on_lineEditROIFolder_returnPressed()
{
    OpenROIFolder();
}
