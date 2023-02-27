#pragma once

namespace WinRt_cppcx
{
    public ref class ClassCppCx sealed
    {
    public:
        ClassCppCx();

        property int GetIntValue
        {
            int get()
            {
                return m_int;
            }

            void set(int val)
            {
                m_int = val;
            }
        }

        property Platform::String^ GetStringValue
        {
            Platform::String^ get()
            {
                return m_string;
            }

            void set(Platform::String^ val)
            {
                m_string = val;
            }
        }

    private:
        int m_int = 7;
        Platform::String^ m_string = "my string value";
    };
}
