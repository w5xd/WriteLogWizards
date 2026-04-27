#pragma once
#include "PROFILE.h"
#ifdef __cplusplus
struct LogFontSaveRestoreHelper : private LogFontWithPointSize
{
    class PointSize_t 
    {   // PointSize_t simply makes PointSizeX10 accessible without parens
        public:
            PointSize_t(unsigned &p) : psX10(p) {}
            operator unsigned &() { return psX10;}
            unsigned &operator=(unsigned x) { psX10 = x; return psX10;}
            PointSize_t& operator=(const PointSize_t &x) { psX10 = x.psX10; return *this; }
    protected:
            unsigned &psX10;
    };
    LogFontSaveRestoreHelper() : PointSizeX10(pointsizeX10)
    {
        memset(static_cast<LogFontWithPointSize*>(this), 0, sizeof(LogFontWithPointSize));
    }
    LogFontSaveRestoreHelper(const LogFontSaveRestoreHelper &other) : PointSizeX10(pointsizeX10)
    {
        lf=other.lf;
        pointsizeX10 = other.pointsizeX10;
    }
    LogFontSaveRestoreHelper &operator = (const LogFontSaveRestoreHelper& other)
    {
        lf = other.lf;
        pointsizeX10 = other.pointsizeX10;
        return *this;
    }
    PointSize_t PointSizeX10;
    LOGFONT &LogFont() { return lf; }
    void SaveFontToProfile(const char *section) { SaveFontToProfile2(this, section);}
    int RestoreFontFromProfile(const char *section) { 
        auto ret = RestoreFontFromProfile2(this, section); 
        if (ret)
            AssignDefaultPointSize();
        return ret;
    }
    int RestoreFontFromProfile(HWND hWnd, const char* section) { 
        auto ret = RestoreFontFromProfile(section);
        auto dpi = wl_GetDpiForHWND(hWnd);
        AdjustToDpi(dpi);
        return ret;
    }
    void AssignDefaultPointSize(unsigned dpi = 96)
    { // assign a default point size. Assume 96 dpi
        if (pointsizeX10 == 0 && (lf.lfHeight < 0) && dpi != 0)
            pointsizeX10 = static_cast<unsigned>(0.5 + -lf.lfHeight * 10 * 96.0 / dpi);
    }
    void AdjustFontSizeForPrintHdc()
    {
        if (pointsizeX10 == 0)
            lf.lfHeight = (lf.lfHeight * 60) / 25;
        else
        {
            static const double mmPerPoint = 0.376;
            lf.lfHeight = static_cast<int>(0.5 + (pointsizeX10 * mmPerPoint));
        }
        lf.lfWidth = 0;
    }
    void AdjustToDpi(int dpi)
    {
        if (dpi > 0 && pointsizeX10 > 0)
        {
            lf.lfHeight = -static_cast<int>(0.5 + (pointsizeX10 * dpi / (96.0 * 10.0)));
        }
        lf.lfWidth = 0;
    }
    void AdjustForMetafileHdc()
    {
        if (pointsizeX10 > 0)
            lf.lfHeight = (lf.lfHeight * 35277L) / 1500;
        else
            lf.lfHeight = -static_cast<int>(pointsizeX10 * 35277.0 / (10.0 * 1500));
        lf.lfWidth = 0;
    }
};
#endif
