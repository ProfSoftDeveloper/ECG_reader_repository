#include "mainwindow.h"
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plot->addGraph();
    ui->plot->graph(0)->setScatterStyle(QCPScatterStyle::ssDot);
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);
    //  Setup double spin box for adjusting vertical scale show
    ui->vert_spac_box->setSingleStep(0.01);
    ui->vert_spac_box->setMinimum(0.00);
    ui->vert_spac_box->setMaximum(0.30);
    ui->vert_spac_box->setValue(0.10);
    //  Setup radio buttons for general watching mode
    hor_watch_ = new QButtonGroup(this);
    hor_watch_->addButton(ui->default_scale_rb);
    hor_watch_->addButton(ui->watch_6cm_rb);
    hor_watch_->setExclusive(true);
    //  Setup radio buttons for vertical scale watching mode
    vert_watch_ = new QButtonGroup(this);
    vert_watch_->addButton(ui->auto_vert_sc_btn);
    vert_watch_->addButton(ui->const_vert_sc_btn);
    vert_watch_->addButton(ui->symm_vert_sc_btn);
    vert_watch_->addButton(ui->user_def_vert_sc_btn);
    vert_watch_->setExclusive(true);
    //  Initially "show" button and associated options are inactive
    //  Main "show" button
    ui->show_data_btn->setEnabled(false);
    //  Three show options
    ui->default_scale_rb->setEnabled(false);
    ui->watch_6cm_rb->setEnabled(false);
    //  Additional options buttons
    ui->vert_spac_box->setEnabled(false);
    switch_hor_buttons(false);
    switch_vert_buttons(false);
    switch_user_def_buttons(false);
    slider_coef_ = 10.0;
}
MainWindow::~MainWindow()
{
    delete hor_watch_;
    delete vert_watch_;
    delete ui;
}
void MainWindow::plot()
{
    data_show_.set_vert_spacing(ui->vert_spac_box->value(),
                                ui->const_vert_sc_btn->isChecked(),
                                ui->symm_vert_sc_btn->isChecked(),
                                ui->user_def_vert_sc_btn->isChecked());
    adjust_scale(data_show_);
    ui->plot->graph(0)->setData(data_.get_time(), data_.get_signal());
    ui->plot->replot();
    ui->plot->update();
}
void MainWindow::adjust_scale(const ECG_data_show& data_show)
{
    ui->plot->xAxis->setRange(data_show.t_min(), data_show.t_max());
    ui->plot->yAxis->setRange(data_show.s_min_show(), data_show.s_max_show());
}
QCPRange MainWindow::scale_range_x() const
{
    return ui->plot->xAxis->range();
}
QCPRange MainWindow::scale_range_y() const
{
    return ui->plot->yAxis->range();
}
void MainWindow::on_choose_data_btn_clicked()
{
    dir = QFileDialog::getExistingDirectory(this,
    tr("Choose data directory"), "/home",
    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //  qDebug() << "The data directory path is " << dir;
    data_ = ECG_data(dir);
    ui->show_data_btn->setEnabled(true);
}
void MainWindow::on_show_data_btn_clicked()
{
    data_show_ = ECG_data_show(0, data_.get_data_length());
    data_show_.setup_limits(data_);
    ui->default_scale_rb->setEnabled(true);
    ui->watch_6cm_rb->setEnabled(true);
    ui->vert_spac_box->setEnabled(true);
    ui->default_scale_rb->setChecked(true);
    plot();
}
void MainWindow::on_clear_btn_clicked()
{
    ui->vert_spac_box->setEnabled(false);
    ui->default_scale_rb->setEnabled(false);
    ui->watch_6cm_rb->setEnabled(false);
    switch_hor_buttons(false);
    switch_vert_buttons(false);
    switch_user_def_buttons(false);
    ui->show_data_btn->setEnabled(false);
    data_.clear();
    plot();
}

void MainWindow::on_vert_spac_box_valueChanged(double arg1)
{
    plot();
}
void MainWindow::on_default_scale_rb_clicked(bool checked)
{
    if(checked)
    {
        switch_hor_buttons(false);
        switch_vert_buttons(false);
        switch_user_def_buttons(false);
        data_show_.setup_default_positions_i_f();
        data_show_.setup_limits(data_);
        plot();
    }
}
void MainWindow::switch_hor_buttons(bool arg)
{
    ui->time_width_lbl->setEnabled(arg);
    ui->time_width_val->setEnabled(arg);
    ui->time_slider_6s->setEnabled(arg);
}
void MainWindow::switch_vert_buttons(bool arg)
{
    ui->auto_vert_sc_btn->setEnabled(arg);
    ui->const_vert_sc_btn->setEnabled(arg);
    ui->symm_vert_sc_btn->setEnabled(arg);
    ui->user_def_vert_sc_btn->setEnabled(arg);
}
void MainWindow::switch_user_def_buttons(bool arg)
{
    ui->from_lbl->setEnabled(arg);
    ui->from_val->setEnabled(arg);
    ui->to_lbl->setEnabled(arg);
    ui->to_val->setEnabled(arg);
}
void MainWindow::on_watch_6cm_rb_clicked(bool checked)
{
    if(checked)
    {
        //  Enabling necessary buttons
        switch_hor_buttons(true);
        switch_vert_buttons(true);
        switch_user_def_buttons(false);
        //  Setting time window value box
        ui->time_width_val->setSingleStep(0.1);         //  step = 1/slider_coef_
        ui->time_width_val->setMinimum(1.0);            //  min = 1/slider_coef_
        ui->time_width_val->setValue(6.0);              //  val = 6
        //  Setting up time_window_width
        ui->time_width_val->setMaximum(data_show_.t_max_default());
        time_window_width = std::min(ui->time_width_val->value(), data_show_.t_max_default());
        //  Setting up initial zoom window
        data_show_.setup_pos_i(0);
        auto it_f = std::lower_bound(data_.get_time().begin(), data_.get_time().end(), time_window_width);
        size_t position_f = it_f - data_.get_time().begin();
        data_show_.setup_pos_f(position_f);
        //  Switching up auto mode for vertical scale
        ui->auto_vert_sc_btn->setChecked(true);
        data_show_.setup_limits(data_);
        //  Setting up time scale slider
        double full_time = data_show_.t_max_default() - data_show_.t_min_default();
        assert(full_time > time_window_width);
        double t_range = fabs(full_time) - time_window_width;
        ui->time_slider_6s->setRange(0, int(t_range*slider_coef_));
        //  Plot
        plot();
    }
}

void MainWindow::on_time_slider_6s_sliderMoved(int position)
{
    adjust_zoom_window();
}
void MainWindow::on_time_width_val_valueChanged(double arg1)
{
    adjust_zoom_window();
}
void MainWindow::adjust_zoom_window()
{
    //  Slider position equals minimum time value to show
    //  slider_coef is constant coefficient to transfer between int and double
    double t_min = ui->time_slider_6s->value()/slider_coef_;
    //  Maximal time value to show is minimal time value plus time window width,
    //  and also in any case can't exceed t_max_default
    double t_max = std::min(data_show_.t_max_default(), t_min + ui->time_width_val->value());
    auto it_min = std::lower_bound(data_.get_time().begin(), data_.get_time().end(), t_min);
    auto it_max = std::lower_bound(data_.get_time().begin(), data_.get_time().end(), t_max);
    size_t position_min = it_min - data_.get_time().begin();
    size_t position_max = it_max - data_.get_time().begin();
    data_show_.setup_pos_i(position_min);
    data_show_.setup_pos_f(position_max);
    vertical_scaling();
}

void MainWindow::vertical_scaling()
{
    switch_user_def_buttons(false);
    if( ui->auto_vert_sc_btn->isChecked() ||
        ui->const_vert_sc_btn->isChecked() ||
        ui->symm_vert_sc_btn->isChecked())
    {
        //  For all cases except of manual setting of show limits,
        //  the setup_limits method sets up show limits for signal
        data_show_.setup_limits(data_);
    }
    else if(ui->user_def_vert_sc_btn->isChecked())
    {
        //  For manual setting of show limits,
        //  the limits are read from two spin boxes,
        //  min and max limits of spin boxes values are defined according to data
        switch_user_def_buttons(true);
        ui->from_val->setMinimum(data_show_.s_min_default());
        ui->from_val->setMaximum(data_show_.s_max_default());
        ui->to_val->setMinimum(data_show_.s_min_default());
        ui->to_val->setMaximum(data_show_.s_max_default());
        data_show_.setup_s_min_show_s_max_show_manually(ui->from_val->value(), ui->to_val->value());
        data_show_.setup_limits(data_);
    }
    plot();
}

void MainWindow::on_auto_vert_sc_btn_clicked(bool checked)
{
    if(checked)
    {
        vertical_scaling();
    }
}

void MainWindow::on_const_vert_sc_btn_clicked(bool checked)
{
    if(checked)
    {
        vertical_scaling();
    }
}

void MainWindow::on_symm_vert_sc_btn_clicked(bool checked)
{
    if(checked)
    {
        vertical_scaling();
    }
}

void MainWindow::on_user_def_vert_sc_btn_clicked(bool checked)
{
    if(checked)
    {
        vertical_scaling();
    }
}





