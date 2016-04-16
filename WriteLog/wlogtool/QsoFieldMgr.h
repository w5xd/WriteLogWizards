#pragma once
#include <winnt.h>
#include <ObjIdl.h>
#include <vector>
#include <string>
#include <Exfsym.h>
#include <Condefs.h>

/* WriteLog uses an array of exfa_stru to describe the fields in the contest exchange.
** A table of them is required at GetLayout time, but that is only called on File/New.
** On File/Open, we get called on our Load() instead.
*/
class CQsoFieldMgr{
    friend class CQsoField;
    friend class CQsoCallField;
public:
    CQsoFieldMgr() : m_version(0){}

    // initialize for File/New
    void StaticInit(const struct exfa_stru *pExfStatic)
    {   // on File/New we get to say what the fields are
        m_fields.clear();
        int curPos = 0;
        while (pExfStatic->hdr)
        {
            int thisPos = curPos;
            if (pExfStatic->at & A_CALL)
                thisPos = -1;
            m_fields.push_back(Field(pExfStatic->hdr, pExfStatic->wid, curPos, pExfStatic->at));
            if (thisPos >= 0)
                curPos += pExfStatic->wid;
            pExfStatic++;
        }
    }

    // used for File/New
    ConstExfPtr_t GetLayout()
    {
        m_Layout.clear();
        for (unsigned i = 0; i < m_fields.size(); i++)
            m_Layout.push_back(Exf(m_fields[i]));
        m_Layout.push_back(Exf());
        return &m_Layout[0];
    }

    /* The module should set a version number and then increment
    ** it when it changes the data it gives us in StaticInit.
    ** That is, "changes" means a recompile of a new version.
    ** We'll save the version, and make it available at Load()
    ** time, but this class cannot help the module know what
    ** to do about changes */
    HRESULT Save(unsigned version, IStream *pStream)const
    {
        ULONG written;
        HRESULT hr = pStream->Write(&version, sizeof(version), &written);
        if (SUCCEEDED(hr))
        {
            ULONG count = m_fields.size();
            hr = pStream->Write(&count, sizeof(count), &written);
        }
        if (SUCCEEDED(hr))
        {
            for (unsigned i = 0; i < m_fields.size(); i++)
            {
                hr = pStream->Write(&m_fields[i].m_position, sizeof(m_fields[i].m_position), &written);
                if (FAILED(hr))
                    break;
                hr = pStream->Write(&m_fields[i].m_width, sizeof(m_fields[i].m_width), &written);
                if (FAILED(hr))
                    break;
            }
        }
        return hr;
    }

    // used at File/Open
    HRESULT Load(IStream *pStream)
    {   
        // what we Load might be from an OLDER version of ourself
        ULONG bytesread;
        unsigned loadedVersion;
        HRESULT hr = pStream->Read(&loadedVersion, sizeof(m_version), &bytesread);
        unsigned count = 0;
        if (SUCCEEDED(hr))
            hr = pStream->Read(&count, sizeof(count), &bytesread);
        std::vector<Field> fields;
        if (SUCCEEDED(hr))
        {
            while (count-- > 0)
            {
                int wid;
                int pos;
                hr = pStream->Read(&pos, sizeof(pos), &bytesread);
                if (FAILED(hr))
                    break;
                hr = pStream->Read(&wid, sizeof(wid), &bytesread);
                if (FAILED(hr))
                    break;
                fields.push_back(Field("", wid, pos, 0));
            }
        }
        if (SUCCEEDED(hr))
        {
            m_fields = fields;
            m_version = loadedVersion;
        }
        return hr;
    }
    unsigned VersionLoaded() const { return m_version; }

protected:
    struct  Field {
        Field(const std::string &name, int width, int position, int attributes)
            : m_name(name), m_width(width), m_position(position), m_attributes(attributes)
        { }
        int m_width;
        int m_position;
        std::string m_name; // will be blank after Load
        int m_attributes;   // ditto
    };

    // need some initializers for exfa_stru
    struct Exf : public exfa_stru{
        Exf(){  ZeroMemory(this, sizeof(*this));  }
        Exf(const Field &f)
        {
            hdr = const_cast<char *>(f.m_name.c_str());
            wid = f.m_width;
            at = f.m_attributes;
            pl = f.m_position;
        };
    };
    unsigned PosFromIdx(unsigned idx) const
    {        return m_fields[idx].m_position;    }
    unsigned WidFromIdx(unsigned idx) const
    {        return m_fields[idx].m_width;    }
    std::vector<Field> m_fields;
    std::vector<Exf> m_Layout;
    unsigned m_version;
};

/* CQsoField is a helper class that interfaces a single field (column)
** in the log. The module should instance once of these for every
** column it wants to read or write.
*/
class CQsoField {
public:
    CQsoField(CQsoFieldMgr &mgr, unsigned which)
        : m_mgr(mgr)
        , m_which(which)
    { }
   
    // is this field offset in the QSO matching?
    bool operator == (int pos) const
    {  return pos == m_mgr.PosFromIdx(m_which);  }

    unsigned width() const {  return m_mgr.WidFromIdx(m_which);  }

    // given a QsoPtr_t we can read/write the characters of the field.
    class accessor {
    public:
        // assign into this field
        const char * operator =(const char *pSrc)
        {
            char *pDest = m_dest;
            for (unsigned c = m_wid; c > 0; c -= 1)
            {
                if (!(*pDest++ = *pSrc++))
                    break; // stop at null
                *pDest = 0; // ensure trailing null
            }
            return m_dest;
        }

        // read this field
        operator const char *() const { return m_dest; }

    protected:
        friend class CQsoField;
        accessor(unsigned wid, char *dest) :m_wid(wid), m_dest(dest){}
        const unsigned m_wid;
        char * const m_dest;
    };

    // accessor for a particular qso_stru
    accessor operator()(qso_stru *q)
    { return accessor(m_mgr.WidFromIdx(m_which), buffer(q));  }
    
protected:
    friend class accessor;
    // virtual so A_CALL can be different
    virtual char *buffer(qso_stru *q)
    {  return &q->var_part[m_mgr.PosFromIdx(m_which)]; }

    CQsoFieldMgr &m_mgr;
    const unsigned m_which;
};

// The A_CALL field is special--its buffer is elsewhere.
class CQsoCallField : public CQsoField
{
public:
    CQsoCallField(CQsoFieldMgr &mgr, unsigned which) : CQsoField(mgr, which){}

    char *buffer(qso_stru *q)  { return &q->stn[0]; }
};