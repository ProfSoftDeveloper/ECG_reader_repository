#include "ecg_data.h"
#include <string>
#include <map>

int str_to_int(const std::string& arg)
{
    int ans = 0;
    std::stringstream(arg) >> ans;
    return ans;
}

//  ECG_data class fields definitions

int read_int_at_last_part_of_dir(const std::string& s, size_t pos)
{
    std::string substr_number;
    //  Iterating through the last part of directory
    //  Symbols before position pos aren't part of file name, don't use them
    std::string::const_iterator it = s.begin() + pos;
    while(it != s.end())
    {
        //  Collecting only digit symbols
        if(std::isdigit(*it))
        {
            substr_number.push_back(*it);
        }
        ++it;
    }
    return str_to_int(substr_number);
}
ECG_data::ECG_data() : t_curr_(0.0), dt_(0.002)
{
}
ECG_data::ECG_data(const QString& dir) : t_curr_(0.0), dt_(0.002)
{
    //  Reading full data files directories
    std::vector<std::string> files_dirs;
    std::map<int, std::string> sorted_dirs;
    for(const auto & entry : std::filesystem::directory_iterator(dir.toStdString()))
    {
        files_dirs.push_back(entry.path().string());
    }
    //  Sorting directories
    size_t pos = dir.toStdString().size();
    for(const auto & entry : files_dirs)
    {
        int num = read_int_at_last_part_of_dir(entry, pos);
        sorted_dirs[num] = entry;
    }
    //  Reading data from sorted set of files
    for(const auto & [key, value] : sorted_dirs)
    {
        read_one_txt_file(QString::fromStdString(value));
    }
}
//  The function to extract int from the std::string after "=" sign
int read_int(const std::string& s)
{
    std::string::size_type p  = s.find('=');
    std::string substr = s.substr(p + 1, s.size());
    return str_to_int(substr);
}
void ECG_data::read_one_txt_file(const QString& full_path_QString)
{
    std::string full_path = full_path_QString.toStdString();
    std::ifstream in_file;
    //qDebug() << "path is " << full_path_QString;
    in_file.open(full_path);
    if (!in_file)
    {
        qDebug() << "Unable to open file" + full_path_QString;
        exit(1);   // call system to stop
    }
    //  Read the file to extract the number of data points
    std::string first_string;
    getline(in_file, first_string);
    int n = read_int(first_string), val;
    //  Read all file and initialize data object
    for(int idx=0; idx < n; ++idx)
    {
        in_file >> val;
        time_.push_back(t_curr_);
        signal_.push_back(double(val));
        //  The same time variable is used for different files
        t_curr_ += dt_;
    }
    //qDebug() << "File was read successfully, n = " << n;
    //  Close input file
    in_file.close();
}
double ECG_data::get_dt() const
{
    return dt_;
}
double ECG_data::get_t_curr() const
{
    return t_curr_;
}
const QVector<double>& ECG_data::get_time() const
{
    return time_;
}
const QVector<double>& ECG_data::get_signal() const
{
    return signal_;
}
size_t ECG_data::get_data_length() const
{
    return time_.size();
}

void ECG_data::clear()
{
    time_.clear();
    signal_.clear();
}

//  ECG_data_show class fields definitions

ECG_data_show::ECG_data_show() :
    pos_i_(0),pos_f_(0)
{
}
ECG_data_show::ECG_data_show(size_t pos_i_arg, size_t pos_f_arg) :
    pos_i_(pos_i_arg), pos_f_(pos_f_arg)
{
    assert(pos_f_ >= 1);
    assert(pos_i_ <= pos_f_);
    pos_i_default_ = pos_i_;
    pos_f_default_ = pos_f_;
}
ECG_data_show& ECG_data_show::operator= (const ECG_data_show& arg)
{
    pos_i_ = arg.pos_i_;
    pos_f_ = arg.pos_f_;
    pos_i_default_ = arg.pos_i_default_;
    pos_f_default_ = arg.pos_f_default_;
    t_min_ = arg.t_min_;
    t_max_ = arg.t_max_;
    s_min_ = arg.s_min_;
    s_max_ = arg.s_max_;
    s_min_show_ = arg.s_min_show_;
    s_max_show_ = arg.s_max_show_;
    t_min_default_ = arg.t_min_default_;
    t_max_default_ = arg.t_max_default_;
    s_min_default_ = arg.s_min_default_;
    s_max_default_ = arg.s_max_default_;
    vert_spacing_ = arg.vert_spacing_;
    return *this;
}
void ECG_data_show::setup_pos_i(size_t pos_i_arg)
{
    pos_i_ = pos_i_arg;
}
void ECG_data_show::setup_pos_f(size_t pos_f_arg)
{
    pos_f_ = pos_f_arg;
}
void ECG_data_show::setup_default_positions_i_f()
{
    pos_i_ = pos_i_default_;
    pos_f_ = pos_f_default_;
}
void ECG_data_show::setup_limits(const ECG_data& arg)
{
    t_min_ = arg.get_time()[pos_i_];
    t_max_ = arg.get_time()[pos_f_ - 1];
    const QVector<double>& signal_to_show = arg.get_signal();
    s_min_ = *std::min_element(signal_to_show.begin() + pos_i_, signal_to_show.begin() + pos_f_);
    s_max_ = *std::max_element(signal_to_show.begin() + pos_i_, signal_to_show.begin() + pos_f_);
    if (t_min_default_ == std::nullopt)
    {
        t_min_default_ = t_min_;
    }
    if (t_max_default_ == std::nullopt)
    {
        t_max_default_ = t_max_;
    }
    if (s_min_default_ == std::nullopt)
    {
        s_min_default_ = s_min_;
    }
    if (s_max_default_ == std::nullopt)
    {
        s_max_default_ = s_max_;
    }
}
void ECG_data_show::setup_s_min_show_s_max_show_manually(double val_min, double val_max)
{
    s_min_show_ = val_min;
    s_max_show_ = val_max;
}
void ECG_data_show::setup_s_show_limits(bool constant_s_scale, bool symm_s_scale, bool user_def_s_scale)
{
    if (constant_s_scale)
    {   //  the default scale for entire time range
        s_min_show_ = *s_min_default_;
        s_max_show_ = *s_max_default_;
    }
    else if(symm_s_scale)
    {   //  symmetrical
        double max_val = std::max(fabs(s_min_), fabs(s_max_));
        s_min_show_ = -max_val;
        s_max_show_ = max_val;
    }
    else if (user_def_s_scale)
    {   //  user defined
        //  Don't do anything, must be defined before
    }
    else
    {   //auto
        s_min_show_ = s_min_;
        s_max_show_ = s_max_;
    }
}
size_t ECG_data_show::pos_i() const
{
    return pos_i_;
}
size_t ECG_data_show::pos_f() const
{
    return pos_f_;
}
double ECG_data_show::t_min() const
{
    return t_min_;
}
double ECG_data_show::t_max() const
{
    return t_max_;
}
double ECG_data_show::s_min() const
{
    return s_min_;
}
double ECG_data_show::s_max() const
{
    return s_max_;
}
double ECG_data_show::t_min_default() const
{
    return *t_min_default_;
}
double ECG_data_show::t_max_default() const
{
    return *t_max_default_;
}
double ECG_data_show::s_min_default() const
{
    return *s_min_default_;
}
double ECG_data_show::s_max_default() const
{
    return *s_max_default_;
}
double ECG_data_show::s_min_show() const
{
    return s_min_show_;
}
double ECG_data_show::s_max_show() const
{
    return s_max_show_;
}
void ECG_data_show::set_vert_spacing(double arg1, bool constant_s_scale,
                                     bool symm_s_scale, bool user_def_s_scale)
{
    setup_s_show_limits(constant_s_scale, symm_s_scale, user_def_s_scale);
    vert_spacing_ = arg1;
    double diff = fabs(s_max_show_ - s_min_show_), shift = arg1*diff;
    s_min_show_ -= shift;
    s_max_show_ += shift;
}

