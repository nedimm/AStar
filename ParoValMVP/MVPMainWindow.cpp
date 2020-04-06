#include "MVPMainWindow.h"
#include "AppQt.hpp"
#include <QFileDialog>
#include "GdClient.hpp"
#include <experimental/filesystem>
#include <QMessageBox>

MVPMainWindow::MVPMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    _ui.setupUi(this);
}

void MVPMainWindow::showEvent(QShowEvent *ev)
{
    QMainWindow::showEvent(ev);
    _showEventHelper();
}

void MVPMainWindow::_showEventHelper()
{
    _applyDefaultParamsToUI();
    _appQt.displayEnvironment();
}

MVPParams MVPMainWindow::_readUIParams()
{
    _uiParams.mapFileName = _ui.inputLineEdit->text().toStdString();
    _uiParams.showGrid = _ui.showGridCheckBox->isChecked();
    _uiParams.gridCellSize = _ui.gridCellSizeLineEdit->text().toInt();
    _uiParams.showGraph = _ui.showGraphCheckBox->isChecked();
    _uiParams.showNodeText = _ui.showNodeTextCheckBox->isChecked();
    _uiParams.showNodeCosts = _ui.showNodeCostsCheckBox->isChecked();
    _uiParams.obstacleCostFactor = _ui.obstacleCostLineEdit->text().toFloat();
    _uiParams.smoothAlpha = _ui.smoothAlphaLineEdit->text().toFloat();
    _uiParams.smoothBeta = _ui.smoothBetaLineEdit->text().toFloat();
    _uiParams.randomStartEnd = _ui.randomPointsCheckBox->isChecked();
    _uiParams.startPoint = _ui.startPointLineEdit->text().toInt();
    _uiParams.endPoint = _ui.endPointLineEdit->text().toInt();
    return _uiParams;
}

void MVPMainWindow::_applyDefaultParamsToUI()
{
    _uiParams = _appQt.getParameters();
    _ui.inputLineEdit->setText(_uiParams.mapFileName.c_str());
    _ui.showGridCheckBox->setChecked(_uiParams.showGrid);
    _ui.gridCellSizeLineEdit->setText(std::to_string(_uiParams.gridCellSize).c_str());
    _ui.showGraphCheckBox->setChecked(_uiParams.showGraph);
    _ui.showNodeTextCheckBox->setChecked(_uiParams.showNodeText);
    _ui.showNodeCostsCheckBox->setChecked(_uiParams.showNodeCosts);
    _ui.obstacleCostHorizontalSlider->setValue(_uiParams.obstacleCostFactor * HSLIDER_RANGE);
    _ui.obstacleCostLineEdit->setText(QString().setNum(_uiParams.obstacleCostFactor, 'f', 4));
    _ui.smoothAlphaHorizontalSlider->setValue(_uiParams.smoothAlpha * HSLIDER_RANGE);
    _ui.smoothAlphaLineEdit->setText(QString().setNum(_uiParams.smoothAlpha, 'f', 4));
    _ui.smoothBetaHorizontalSlider->setValue(_uiParams.smoothBeta * HSLIDER_RANGE);
    _ui.smoothBetaLineEdit->setText(QString().setNum(_uiParams.smoothBeta, 'f', 4));
    _ui.randomPointsCheckBox->setChecked(_uiParams.randomStartEnd);
    _ui.startPointLineEdit->setDisabled(_uiParams.randomStartEnd);
    _ui.endPointLineEdit->setDisabled(_uiParams.randomStartEnd);
}

void MVPMainWindow::_disableControlElements(bool disable)
{
    _ui.inputLineEdit->setDisabled(disable);
    _ui.inputPushButton->setDisabled(disable);
    _ui.getPushButton->setDisabled(disable);
    _ui.showGridCheckBox->setDisabled(disable);
    _ui.gridCellSizeLineEdit->setDisabled(disable);
    _ui.showGraphCheckBox->setDisabled(disable);
    _ui.showNodeTextCheckBox->setDisabled(disable);
    _ui.showNodeCostsCheckBox->setDisabled(disable);
    _ui.startPushButton->setDisabled(disable);
    _ui.closePushButton->setDisabled(disable);
    _ui.visualizePushButton->setDisabled(disable);
}

void MVPMainWindow::on_inputPushButton_pressed()
{
    QString filter = "File Description (*.*)";
    _ui.inputLineEdit->setText(QFileDialog::getOpenFileName(this, "Select a file...", QDir::currentPath(), filter));
    _readUIParams();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::on_getPushButton_pressed()
{
    GdClient gdc;
    auto file_names = gdc.getFileNames();
    _ui.openScenarioComboBox->clear();
    
    this->setCursor(QCursor(Qt::WaitCursor));
    for (auto file_name : file_names)
    {
        try
        {
            gdc.getImage(file_name);
        }catch (std::exception& ex)
        {
            QMessageBox msgBox;
            QString message = "Problem contacting Cloud:";
            message.append(ex.what());
            msgBox.setText(message);
            msgBox.exec();
        }
        _ui.openScenarioComboBox->addItem(file_name.c_str());
    }
    this->setCursor(QCursor(Qt::ArrowCursor));
}

void MVPMainWindow::_correctWrongInput()
{
    int min = 0;
    int max = _appQt.getLastNodeIndex();
    if (_ui.startPointLineEdit->text().size() == 0)
        _ui.startPointLineEdit->setText("0");
    if (_ui.endPointLineEdit->text().size() == 0)
        _ui.endPointLineEdit->setText(std::to_string(max).c_str());
        
    int start = _ui.startPointLineEdit->text().toUInt();
    start = std::min(start, max);

    int end = _ui.endPointLineEdit->text().toUInt();
    end = std::min(end, max);

    _ui.startPointLineEdit->setText(std::to_string(start).c_str());
    _ui.endPointLineEdit->setText(std::to_string(end).c_str());

    _uiParams.startPoint = start;
    _uiParams.endPoint = end;
}

void MVPMainWindow::on_showGridCheckBox_stateChanged()
{
    _uiParams.showGrid = _ui.showGridCheckBox->checkState();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::on_showGraphCheckBox_stateChanged()
{
    _uiParams.showGraph = _ui.showGraphCheckBox->checkState();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::_gridCellSizeChanged()
{
    _uiParams.gridCellSize = _ui.gridCellSizeLineEdit->text().toUInt();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::on_gridCellSizeLineEdit_returnPressed()
{
    _gridCellSizeChanged();
}

void MVPMainWindow::on_gridCellSizeLineEdit_editingFinished()
{
    _gridCellSizeChanged();
}

void MVPMainWindow::on_showNodeTextCheckBox_stateChanged()
{
    _uiParams.showNodeText = _ui.showNodeTextCheckBox->checkState();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::on_showNodeCostsCheckBox_stateChanged()
{
    _uiParams.showNodeCosts = _ui.showNodeCostsCheckBox->checkState();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::on_obstacleCostHorizontalSlider_sliderReleased()
{
    float value = _ui.obstacleCostHorizontalSlider->value() / (HSLIDER_RANGE * 1.0);
    _ui.obstacleCostLineEdit->setText(QString().setNum(value, 'f', 4));
    _uiParams.obstacleCostFactor = value;
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::_obstacleCostChanged()
{
    _uiParams.obstacleCostFactor = _ui.obstacleCostLineEdit->text().toFloat();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
    _ui.obstacleCostHorizontalSlider->setValue(_uiParams.obstacleCostFactor * HSLIDER_RANGE);
}

void MVPMainWindow::on_obstacleCostLineEdit_returnPressed()
{
    _obstacleCostChanged();
}

void MVPMainWindow::on_obstacleCostLineEdit_editingFinished()
{
    _obstacleCostChanged();
}

void MVPMainWindow::on_smoothAlphaHorizontalSlider_sliderReleased()
{
    float value = _ui.smoothAlphaHorizontalSlider->value() / (HSLIDER_RANGE * 1.0);
    _ui.smoothAlphaLineEdit->setText(QString().setNum(value, 'f', 4));
    _uiParams.smoothAlpha = value;
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::_smoothAlphaChanged()
{
    _uiParams.smoothAlpha = _ui.smoothAlphaLineEdit->text().toFloat();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
    _ui.smoothAlphaHorizontalSlider->setValue(_uiParams.smoothAlpha * HSLIDER_RANGE);
}

void MVPMainWindow::on_smoothAlphaLineEdit_returnPressed()
{
    _smoothAlphaChanged();
}

void MVPMainWindow::on_smoothAlphaLineEdit_editingFinished()
{
    _smoothAlphaChanged();
}

void MVPMainWindow::on_smoothBetaHorizontalSlider_sliderReleased()
{
    float value = _ui.smoothBetaHorizontalSlider->value() / (HSLIDER_RANGE * 1.0);
    _ui.smoothBetaLineEdit->setText(QString().setNum(value, 'f', 4));
    _uiParams.smoothBeta = value;
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::_smoothBetaChanged()
{
    _uiParams.smoothBeta = _ui.smoothBetaLineEdit->text().toFloat();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
    _ui.smoothBetaHorizontalSlider->setValue(_uiParams.smoothBeta * HSLIDER_RANGE);
}

void MVPMainWindow::on_smoothBetaLineEdit_returnPressed()
{
    _smoothBetaChanged();
}

void MVPMainWindow::on_smoothBetaLineEdit_editingFinished()
{
    _smoothBetaChanged();
}

void MVPMainWindow::on_openScenarioComboBox_activated(int index)
{
    if (index < 0) return;
    const QString& text = _ui.openScenarioComboBox->currentText();
    _ui.inputLineEdit->setText(text);
    _readUIParams();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::on_randomPointsCheckBox_stateChanged()
{
    _uiParams.randomStartEnd = _ui.randomPointsCheckBox->checkState();
    _appQt.setParameters(_uiParams);
    _ui.startPointLineEdit->setDisabled(_uiParams.randomStartEnd);
    _ui.endPointLineEdit->setDisabled(_uiParams.randomStartEnd);
    if (!_uiParams.randomStartEnd)
    {
        _uiParams.startPoint = 0;
        _ui.startPointLineEdit->setText(std::to_string(_uiParams.startPoint).c_str());
        _uiParams.endPoint = _appQt.getLastNodeIndex();
        _ui.endPointLineEdit->setText(std::to_string(_uiParams.endPoint).c_str());
    }
    _appQt.displayEnvironment();
}

void MVPMainWindow::_startPointChanged()
{
    _uiParams.startPoint = _ui.startPointLineEdit->text().toUInt();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::on_startPointLineEdit_returnPressed()
{
    _startPointChanged();
}

void MVPMainWindow::on_startPointLineEdit_editingFinished()
{
    _startPointChanged();
}

void MVPMainWindow::_endPointChanged()
{
    _uiParams.endPoint = _ui.endPointLineEdit->text().toUInt();
    _appQt.setParameters(_uiParams);
    _appQt.displayEnvironment();
}

void MVPMainWindow::on_endPointLineEdit_returnPressed()
{
    _endPointChanged();
}

void MVPMainWindow::on_endPointLineEdit_editingFinished()
{
    _endPointChanged();
}

void MVPMainWindow::on_startPushButton_pressed()
{
    _disableControlElements(true);
    _correctWrongInput();

    MVPParams params = _readUIParams();
    _appQt.setParameters(params);
    _appQt.start();
    _disableControlElements(false);
}

void MVPMainWindow::on_closePushButton_pressed()
{
    _appQt.close();
    close();
}

void MVPMainWindow::on_visualizePushButton_pressed()
{
    _disableControlElements(true);
    _appQt.visualizeOptimizationRun();
    _disableControlElements(false);
}


