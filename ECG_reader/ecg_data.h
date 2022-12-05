#ifndef ECG_DATA_H
#define ECG_DATA_H

#include <vector>
#include <QString>
#include <QDebug>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <optional>

class ECG_data
{
public:
    ECG_data();
    ECG_data(const QString& dir);
    void read_one_txt_file(const QString& full_path);
    double get_dt() const;
    double get_t_curr() const;
    const QVector<double>& get_time() const;
    const QVector<double>& get_signal() const;
    size_t get_data_length() const;
    void clear();
private:
    double t_curr_, dt_;
    QVector<double> time_, signal_;
};

class ECG_data_show : public ECG_data
{
public:
    ECG_data_show();
    ECG_data_show(size_t pos_i_arg, size_t pos_f_arg);
    ECG_data_show& operator= (const ECG_data_show& arg);
    void setup_pos_i(size_t pos_i_arg);
    void setup_pos_f(size_t pos_f_arg);
    void setup_default_positions_i_f();
    void setup_limits(const ECG_data& arg);
    void setup_s_min_show_s_max_show_manually(double val_min, double val_max);
    size_t pos_i() const;
    size_t pos_f() const;
    double t_min() const;
    double t_max() const;
    double s_min() const;
    double s_max() const;
    double t_min_default() const;
    double t_max_default() const;
    double s_min_default() const;
    double s_max_default() const;
    double s_min_show() const;
    double s_max_show() const;
    void set_vert_spacing(double arg1, bool constant_s_scale, bool symm_s_scale, bool user_def_s_scale);
    void setup_s_show_limits(bool constant_s_scale, bool symm_s_scale, bool user_def_s_scale);
private:
    size_t pos_i_, pos_f_, pos_i_default_, pos_f_default_;
    double t_min_, t_max_;
    double s_min_, s_max_;
    double s_min_show_, s_max_show_;
    std::optional<double> t_min_default_, t_max_default_;
    std::optional<double> s_min_default_, s_max_default_;
    double vert_spacing_;
};

#endif // ECG_DATA_H
