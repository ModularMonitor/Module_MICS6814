#pragma once

#include <Arduino.h>
// mMICS wraps MICS6814 in a class. This way, less things to worry about.
class mMICS {
    const int m_co, m_nh3, m_no2;
public:
    mMICS(int co_port, int nh3_port, int no2_port)
        : m_co(co_port), m_nh3(nh3_port), m_no2(no2_port)
    {   
    }
    
    float get_co() const { return map(analogRead(m_co),  0, 4095.0f, 1, 1000); }    
    float get_nh3() const { return map(analogRead(m_nh3), 0, 4095.0f, 1, 500); }    
    float get_no2() const { return map(analogRead(m_no2), 0, 4095.0f, 5, 1000) * 0.01f; }
    
    bool has_issues() const { return false; }
    bool has_new_data_autoreset() { return true; }
};