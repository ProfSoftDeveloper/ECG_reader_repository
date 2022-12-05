#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QFileDialog>
#include "ecg_data.h"
#include <QButtonGroup>
#include "..\build-ECG_reader-Desktop_Qt_6_4_1_MinGW_64_bit-Debug\ui_mainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void plot();

    void adjust_scale(const ECG_data_show& data_show);
    QCPRange scale_range_x() const;
    QCPRange scale_range_y() const;
private slots:
    void on_choose_data_btn_clicked();
    void on_show_data_btn_clicked();
    void on_clear_btn_clicked();

    void on_vert_spac_box_valueChanged(double arg1);

    void on_watch_6cm_rb_clicked(bool checked);

    void on_default_scale_rb_clicked(bool checked);
    void switch_hor_buttons(bool arg);
    void switch_vert_buttons(bool arg);
    void switch_user_def_buttons(bool arg);

    void on_time_slider_6s_sliderMoved(int position);
    void adjust_zoom_window();

    void on_time_width_val_valueChanged(double arg1);
    void vertical_scaling();
    void on_auto_vert_sc_btn_clicked(bool checked);

    void on_symm_vert_sc_btn_clicked(bool checked);

    void on_user_def_vert_sc_btn_clicked(bool checked);

    void on_const_vert_sc_btn_clicked(bool checked);

private:
    Ui::MainWindow *ui;
    QString dir;
    ECG_data data_;
    ECG_data_show data_show_;
    QButtonGroup * hor_watch_;
    QButtonGroup * vert_watch_;
    double time_window_width;
    double slider_coef_;
};
#endif // MAINWINDOW_H
