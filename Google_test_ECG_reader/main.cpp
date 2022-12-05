#include <gtest/gtest.h>
#include <string>
#include "..\ECG_reader\Google_test_main_cpp_header.h"
#include <QApplication>

#define DATA_PATH "C:/ECG_data_reader/Google_test_ECG_reader/data"

struct ECG_data_test : testing::Test
{
public:
    ECG_data_test()
    {
        data_ = new ECG_data;
    }
    ECG_data_test(const std::string& path)
    {
        data_ = new ECG_data(QString::fromStdString(path));
    }
    virtual ~ECG_data_test()
    {
        delete data_;
    }
    ECG_data * data()
    {
        return data_;
    }
protected:
    ECG_data * data_;
};

TEST_F(ECG_data_test, ZeroTimeAtStart)
{
    EXPECT_EQ(0.0, data()->get_t_curr());
}
TEST_F(ECG_data_test, CorrectTimeStepAtStart)
{
    EXPECT_EQ(0.002, data()->get_dt());
}
TEST_F(ECG_data_test, EmptyTimeArrayAtStart)
{
    EXPECT_EQ(true, data()->get_time().empty());
}
TEST_F(ECG_data_test, EmptySignalArrayAtStart)
{
    EXPECT_EQ(true, data()->get_signal().empty());
}

struct ECG_data_state
{
    std::string data_path;
    double time_step;
    friend std::ostream& operator<<(std::ostream& os, const ECG_data_state& obj)
    {
        return os
                << "Data path = " << obj.data_path
                << "time step = " << obj.time_step;
    }
};

//  Reading data test

struct DataReadTest : ECG_data_test, testing::WithParamInterface<ECG_data_state>
{
    DataReadTest()
    {
        data_ = new ECG_data(QString::fromStdString(GetParam().data_path));
    }
};

TEST_P(DataReadTest, ReadingDataFile)
{
    auto as = GetParam();
    EXPECT_EQ(data_->get_dt(), as.time_step);
    EXPECT_EQ(size_t(data_->get_t_curr()/data_->get_dt())+1, size_t(data_->get_time().size()));
    EXPECT_EQ(data_->get_time().size(), data_->get_signal().size());
    EXPECT_EQ(data_->get_data_length(), data_->get_signal().size());
    data_->clear();
    EXPECT_EQ(data_->get_time().size(), 0);
    EXPECT_EQ(data_->get_signal().size(), 0);

}

INSTANTIATE_TEST_SUITE_P(Default, DataReadTest,
    testing::Values(
        ECG_data_state{DATA_PATH, 0.002}
    )
);

//  ECG_data_show basic functioning test

struct ECG_data_show_test : ECG_data_test, testing::WithParamInterface<ECG_data_state>
{
public:
    ECG_data_show_test()
    {
        data_ = new ECG_data(QString::fromStdString(GetParam().data_path));
        data_show_test_ = new ECG_data_show(0, data_->get_data_length());
        data_show_test_->setup_limits(*data_);
    }
    virtual ~ECG_data_show_test()
    {
        delete data_show_test_;
    }
    ECG_data_show * data_show()
    {
        return data_show_test_;
    }
protected:
    ECG_data_show * data_show_test_;
};

TEST_P(ECG_data_show_test, BasicECGDataShowFunctioningTest)
{
    EXPECT_EQ(data_->get_data_length() > 0, true);
    EXPECT_EQ(data_show_test_->t_min(), 0.0);
    EXPECT_EQ(data_show_test_->t_max(), data_->get_time()[data_->get_data_length()-1]);
    EXPECT_EQ(data_show_test_->t_min_default(), 0.0);
    EXPECT_EQ(data_show_test_->t_max_default(), data_show_test_->t_max());
    EXPECT_EQ(data_show_test_->s_min_default(),
              *std::min_element(data_->get_signal().begin(), data_->get_signal().end()));
    EXPECT_EQ(data_show_test_->s_max_default(),
              *std::max_element(data_->get_signal().begin(), data_->get_signal().end()));
}

INSTANTIATE_TEST_SUITE_P(Default, ECG_data_show_test,
    testing::Values(
        ECG_data_state{DATA_PATH, 0.002}
    )
);

//  ECG data show setup_s_show_limits test

struct setup_s_show_limits_state
{
    std::string data_path;
    bool constant;
    bool symmetrical;
    bool user_defined;
    double vert_scaling_value;
};

struct setup_s_show_limits_test : ECG_data_test, testing::WithParamInterface<setup_s_show_limits_state>
{
public:
    setup_s_show_limits_test()
    {
        auto as = GetParam();
        data_ = new ECG_data(QString::fromStdString(as.data_path));
        data_show_test_ = new ECG_data_show(0, data_->get_data_length());
        data_show_test_->setup_limits(*data_);
        data_show_test_->setup_s_show_limits(as.constant, as.symmetrical, as.user_defined);
    }
    virtual ~setup_s_show_limits_test()
    {
        delete data_show_test_;
    }
    ECG_data_show * data_show()
    {
        return data_show_test_;
    }
protected:
    ECG_data_show * data_show_test_;
};

TEST_P(setup_s_show_limits_test, SetupSignalShowLimitsTest)
{
    auto as = GetParam();
    if(as.constant)
    {
        EXPECT_EQ(data_show_test_->s_min_show(), data_show_test_->s_min_default());
        EXPECT_EQ(data_show_test_->s_max_show(), data_show_test_->s_max_default());
    }
    else if(as.symmetrical)
    {
        EXPECT_EQ(data_show_test_->s_min_show(), -data_show_test_->s_max_show());
    }
    else if(as.user_defined)
    {
        //  Not tested here
    }
    else
    {
        EXPECT_EQ(data_show_test_->s_min_show(), data_show_test_->s_min());
        EXPECT_EQ(data_show_test_->s_max_show(), data_show_test_->s_max());
    }
}

INSTANTIATE_TEST_SUITE_P(Default, setup_s_show_limits_test,
    testing::Values(
    setup_s_show_limits_state{DATA_PATH, false, false, false, 0.0},
    setup_s_show_limits_state{DATA_PATH, true, false, false, 0.0},
    setup_s_show_limits_state{DATA_PATH, false, true, false, 0.0},
    setup_s_show_limits_state{DATA_PATH, false, false, true, 0.0}
    )
);

//  ECG_data_show vertical scaling test

struct vertical_scaling_test : setup_s_show_limits_test, testing::WithParamInterface<setup_s_show_limits_state>
{
public:
    vertical_scaling_test()
    {
        auto as=GetParam();
        data_show_test_->set_vert_spacing(as.vert_scaling_value,
                                     as.constant, as.symmetrical, as.user_defined);
    }
};

TEST_P(vertical_scaling_test, SetVerticalSpacingTest)
{
    auto as = GetParam();
    //  It is enough to check calculation for only one logical case
    //  For this case auto scaling works, s_min and s_max define show signal limits
    if(!as.constant && !as.symmetrical && !as.user_defined)
    {
        double s_min = data_show_test_->s_min();
        double s_max = data_show_test_->s_max();
        double diff = fabs(s_max - s_min);
        double addition = diff*as.vert_scaling_value;
        EXPECT_EQ(s_min - addition, data_show_test_->s_min_show());
        EXPECT_EQ(s_max + addition, data_show_test_->s_max_show());
    }
}

INSTANTIATE_TEST_SUITE_P(Default, vertical_scaling_test,
    testing::Values(
    setup_s_show_limits_state{DATA_PATH, false, false, false, 0.05}
    )
);

//  Adjust scale test

struct AdjustScaleTest : setup_s_show_limits_test, public MainWindow, testing::WithParamInterface<setup_s_show_limits_state>
{
public:
    AdjustScaleTest()
    {
        window_ = new MainWindow();
        window_->adjust_scale(*data_show_test_);
    }
    virtual ~ AdjustScaleTest()
    {
        delete window_;
    }
    MainWindow * window_;
};

TEST_P(AdjustScaleTest, AdjustingScaleCheck)
{
    double t_min = data_show_test_->t_min(), t_max = data_show_test_->t_max();
    double s_min_show = data_show_test_->s_min_show();
    double s_max_show = data_show_test_->s_max_show();
    EXPECT_EQ(window_->scale_range_x().size(), t_max - t_min);
    EXPECT_EQ(window_->scale_range_x().center(), 0.5*(t_max + t_min));
    EXPECT_EQ(window_->scale_range_y().size(), s_max_show - s_min_show);
    EXPECT_EQ(window_->scale_range_y().center(), 0.5*(s_max_show + s_min_show));
}

INSTANTIATE_TEST_SUITE_P(Default, AdjustScaleTest,
    testing::Values(
    setup_s_show_limits_state{DATA_PATH, false, false, false, 0.05}
    )
);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
