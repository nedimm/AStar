#pragma once

#include "ui_ParoValMVP.h"
#include "AppQt.hpp"
#include "MVPParams.hpp"


class MVPMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MVPMainWindow(QWidget *parent = Q_NULLPTR);

protected:
    void showEvent(QShowEvent *ev);

private:
    Ui::ParoValMVPClass _ui;
    MVPParams _uiParams;
    AppQt _appQt;
    void _showEventHelper();
    const int HSLIDER_RANGE = 100000;

private slots:
    MVPParams _readUIParams();
    void _applyDefaultParamsToUI();
    void _disableControlElements(bool disable);
    void _correctWrongInput();
    void on_startPushButton_pressed();
    void on_closePushButton_pressed();
    void on_showGridCheckBox_stateChanged();
    void on_showGraphCheckBox_stateChanged();
    void _gridCellSizeChanged();
    void on_gridCellSizeLineEdit_returnPressed();
    void on_gridCellSizeLineEdit_editingFinished();

    void on_showNodeTextCheckBox_stateChanged();
    void on_showNodeCostsCheckBox_stateChanged();
    void on_obstacleCostHorizontalSlider_sliderReleased();
    void _obstacleCostChanged();
    void on_obstacleCostLineEdit_returnPressed();
    void on_obstacleCostLineEdit_editingFinished();

    void on_smoothAlphaHorizontalSlider_sliderReleased();
    void _smoothAlphaChanged();
    void on_smoothAlphaLineEdit_returnPressed();
    void on_smoothAlphaLineEdit_editingFinished();

    void on_smoothBetaHorizontalSlider_sliderReleased();
    void _smoothBetaChanged();
    void on_smoothBetaLineEdit_returnPressed();
    void on_smoothBetaLineEdit_editingFinished();

    void on_randomPointsCheckBox_stateChanged();
    void _startPointChanged();
    void on_startPointLineEdit_returnPressed();
    void on_startPointLineEdit_editingFinished();
    void _endPointChanged();

    void on_endPointLineEdit_returnPressed();
    void on_endPointLineEdit_editingFinished();
};
